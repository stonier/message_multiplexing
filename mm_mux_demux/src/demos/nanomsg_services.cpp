/**
 * @file /mm_mux_demux/src/demos/nanomsg_services.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <nanomsg/nn.h>
#include <nanomsg/reqrep.h>
#include <iostream>

#define SOCKET_ADDRESS "inproc://test"

void test_recv(int sock, const char *data) {
  size_t data_length;
  void* buf = NULL;
  data_length = strlen(data);
  int bytes;
  bytes = nn_recv(sock, &buf, NN_MSG, 0);
  printf ("Socket %d EXPECTED %s RECEIVED %s [%d bytes]\n", sock, data, (char*)buf, bytes);
  nn_freemsg(buf);
}

int main ()
{
    int rc;
    int rep1;
    int rep2;
    int req1;
    int req2;
    int resend_ivl;
    char buf [7];
    int timeo;

    /*  Test req/rep with full socket types. */
    rep1 = nn_socket (AF_SP, NN_REP);
    nn_bind (rep1, SOCKET_ADDRESS);
    req1 = nn_socket (AF_SP, NN_REQ);
    nn_connect (req1, SOCKET_ADDRESS);
    req2 = nn_socket (AF_SP, NN_REQ);
    nn_connect (req2, SOCKET_ADDRESS);

    /*  Check invalid sequence of sends and recvs. */
    rc = nn_send (rep1, "ABC", 3, 0);
    //nn_assert (rc == -1 && nn_errno () == EFSM);
    rc = nn_recv (req1, buf, sizeof (buf), 0);
    //nn_assert (rc == -1 && nn_errno () == EFSM);

    /*  Check fair queueing the requests. */
    nn_send (req2, "ABC", 3, 0);
    test_recv (rep1, "ABC");
    nn_send (rep1, "ABC", 3, 0);
    test_recv (req2, "ABC");

    nn_send (req1, "ABC", 3, 0);
    test_recv (rep1, "ABC");
    nn_send (rep1, "ABC", 3, 0);
    test_recv (req1, "ABC");

    nn_close (rep1);
    nn_close (req1);
    nn_close (req2);

    /*  Check load-balancing of requests. */
    req1 = nn_socket (AF_SP, NN_REQ);
    nn_bind (req1, SOCKET_ADDRESS);
    rep1 = nn_socket (AF_SP, NN_REP);
    nn_connect (rep1, SOCKET_ADDRESS);
    rep2 = nn_socket (AF_SP, NN_REP);
    nn_connect (rep2, SOCKET_ADDRESS);

    nn_send (req1, "ABC", 3, 0);
    test_recv (rep1, "ABC");
    nn_send (rep1, "ABC", 3, 0);
    test_recv (req1, "ABC");

    nn_send (req1, "ABC", 3, 0);
    test_recv (rep2, "ABC");
    nn_send (rep2, "ABC", 3, 0);
    test_recv (req1, "ABC");

    nn_close (rep2);
    nn_close (rep1);
    nn_close (req1);

    /*  Test re-sending of the request. */
    rep1 = nn_socket (AF_SP, NN_REP);
    nn_bind (rep1, SOCKET_ADDRESS);
    req1 = nn_socket (AF_SP, NN_REQ);
    nn_connect (req1, SOCKET_ADDRESS);
    resend_ivl = 100;
    rc = nn_setsockopt (req1, NN_REQ, NN_REQ_RESEND_IVL,
        &resend_ivl, sizeof (resend_ivl));

    nn_send (req1, "ABC", 3, 0);
    test_recv (rep1, "ABC");
    /*  The following waits for request to be resent  */
    test_recv (rep1, "ABC");

    nn_close (req1);
    nn_close (rep1);

    /*  Check sending a request when the peer is not available. (It should
        be sent immediatelly when the peer comes online rather than relying
        on the resend algorithm. */
    req1 = nn_socket (AF_SP, NN_REQ);
    nn_connect (req1, SOCKET_ADDRESS);
    nn_send (req1, "ABC", 3, 0);

    rep1 = nn_socket (AF_SP, NN_REP);
    nn_bind (rep1, SOCKET_ADDRESS);
    timeo = 200;
    rc = nn_setsockopt (rep1, NN_SOL_SOCKET, NN_RCVTIMEO,
       &timeo, sizeof (timeo));

    nn_close (req1);
    nn_close (rep1);

    /*  Check removing socket request sent to (It should
        be sent immediatelly to other peer rather than relying
        on the resend algorithm). */
    req1 = nn_socket (AF_SP, NN_REQ);
    nn_bind (req1, SOCKET_ADDRESS);
    rep1 = nn_socket (AF_SP, NN_REP);
    nn_connect (rep1, SOCKET_ADDRESS);
    rep2 = nn_socket (AF_SP, NN_REP);
    nn_connect (rep2, SOCKET_ADDRESS);

    timeo = 200;
    rc = nn_setsockopt (rep1, NN_SOL_SOCKET, NN_RCVTIMEO,
       &timeo, sizeof (timeo));
    rc = nn_setsockopt (rep2, NN_SOL_SOCKET, NN_RCVTIMEO,
       &timeo, sizeof (timeo));

    nn_send (req1, "ABC", 3, 0);
    /*  We got request through rep1  */
    test_recv (rep1, "ABC");
    /*  But instead replying we simulate crash  */
    nn_close (rep1);
    /*  The rep2 should get request immediately  */
    test_recv (rep2, "ABC");
    /*  Let's check it's delivered well  */
    nn_send (rep2, "REPLY", 5, 0);
    test_recv (req1, "REPLY");


    nn_close (req1);
    nn_close (rep2);

    /*  Test cancelling delayed request  */

    req1 = nn_socket (AF_SP, NN_REQ);
    nn_connect (req1, SOCKET_ADDRESS);
    nn_send (req1, "ABC", 3, 0);
    nn_send (req1, "DEF", 3, 0);

    rep1 = nn_socket (AF_SP, NN_REP);
    nn_bind (rep1, SOCKET_ADDRESS);
    timeo = 100;
//    rc = nn_setsockopt (rep1, NN_SOL_SOCKET, NN_RCVTIMEO,
//       &timeo, sizeof (timeo));
//    errno_assert (rc == 0);
    test_recv (rep1, "DEF");

    nn_close (req1);
    nn_close (rep1);

    return 0;
}

