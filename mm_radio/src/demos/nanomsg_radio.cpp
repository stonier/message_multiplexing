/**
 * @file /mm_mux_demux/src/nanomsg_radio.cpp
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
#include <nanomsg/pair.h>
#include <iostream>


#define SOCKET_ADDRESS "inproc://a"

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
    int sb;
    int sc;

    sb = nn_socket (AF_SP, NN_PAIR);
    nn_bind (sb, SOCKET_ADDRESS);
    sc = nn_socket (AF_SP, NN_PAIR);
    nn_connect (sc, SOCKET_ADDRESS);

    nn_send (sc, "ABC", 3, 0);
    test_recv (sb, "ABC");
    nn_send (sb, "DEF", 3, 0);
    test_recv (sc, "DEF");

    nn_close (sc);
    nn_close (sb);

    return 0;
}
