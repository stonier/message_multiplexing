/**
 * @file /mm_mux_demux/src/nanomsg_pipeline.cpp
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <assert.h>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <nanomsg/nn.h>
#include <nanomsg/pipeline.h>
#include <iostream>

#define SOCKET_ADDRESS "inproc://a"


int main ()
{
    int push1;
    int push2;
    int pull1;
    int pull2;

    /*  Test fan-out. */

    // these were originally test_xxx from https://github.com/nanomsg/nanomsg/blob/master/tests/testutil.h
    // only difference is they had error checking
    push1 = nn_socket(AF_SP, NN_PUSH);
    nn_bind(push1, SOCKET_ADDRESS);
    pull1 = nn_socket(AF_SP, NN_PULL);
    nn_connect (pull1, SOCKET_ADDRESS);
    pull2 = nn_socket (AF_SP, NN_PULL);
    nn_connect (pull2, SOCKET_ADDRESS);

    /*  Wait till both connections are established to get messages spread
        evenly between the two pull sockets. */
    //nn_sleep(10);
    sleep(1);

    int length = strlen("ABC");
    //test_send (push1, "ABC");
    nn_send(push1, "ABC", length, 0);
    //test_send (push1, "DEF");
    nn_send(push1, "DEF", length, 0);

    //test_recv (pull1, "ABC");
    //test_recv (pull2, "DEF");
    char *buf = NULL;
    int bytes;
    bytes = nn_recv(pull1, &buf, NN_MSG, 0);
    assert (bytes >= 0);
    printf ("PULL1: RECEIVED %s\n", buf);
    bytes = nn_recv(pull2, &buf, NN_MSG, 0);
    assert (bytes >= 0);
    printf ("PULL2: RECEIVED %s\n", buf);

    nn_close(push1);
    nn_close(pull1);
    nn_close(pull2);

    /*  Test fan-in. */

    pull1 = nn_socket (AF_SP, NN_PULL);
    nn_bind(pull1, SOCKET_ADDRESS);
    push1 = nn_socket (AF_SP, NN_PUSH);
    nn_connect(push1, SOCKET_ADDRESS);
    push2 = nn_socket(AF_SP, NN_PUSH);
    nn_connect(push2, SOCKET_ADDRESS);

    nn_send (push1, "ABC", length, 0);
    nn_send (push2, "DEF", length, 0);

    //test_recv (pull1, "ABC");
    //test_recv (pull1, "DEF");
    bytes = nn_recv (pull1, &buf, NN_MSG, 0);
    assert (bytes >= 0);
    printf ("PULL1: RECEIVED %s\n", buf);
    bytes = nn_recv (pull1, &buf, NN_MSG, 0);
    assert (bytes >= 0);
    printf ("PULL2: RECEIVED %s\n", buf);

    nn_close (pull1);
    nn_close (push1);
    nn_close (push2);

    return 0;
}
