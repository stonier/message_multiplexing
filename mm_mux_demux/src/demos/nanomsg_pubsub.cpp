/**
 * @file /mm_mux_demux/src/nanomsg_pubsub.cpp
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
#include <nanomsg/pubsub.h>
#include <iostream>

#define SERVER "server"
#define CLIENT "client"
#define URL "ipc:///tmp/nanomsg_pubsub.ipc"

char *date ()
{
  time_t raw = time (&raw);
  struct tm *info = localtime (&raw);
  char *text = asctime (info);
  text[strlen(text)-1] = '\0'; // remove '\n'
  return text;
}

int server ()
{
  int sock = nn_socket (AF_SP, NN_PUB);
  assert (sock >= 0);
  int result;
  result = nn_bind (sock, URL);
  assert (result >= 0);
  std::cout << "Asserted bind on " << URL << std::endl;
  while (1)
    {
      char *d = date();
      int sz_d = strlen(d) + 1; // '\0' too
      printf ("SERVER: PUBLISHING DATE %s\n", d);
      int bytes = nn_send (sock, d, sz_d, 0);
      assert (bytes == sz_d);
      sleep(1);
    }
  return nn_shutdown (sock, 0);
}

int client ()
{
  int sock = nn_socket (AF_SP, NN_SUB);
  std::cout << "Socket: " << sock << std::endl;
  assert (sock >= 0);
  // TODO learn more about publishing/subscribe keys
  int result;
  result = nn_setsockopt (sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
  assert(result >= 0);
  std::cout << "Asserted nn_setsockopt" << std::endl;
  result = nn_connect (sock, URL);
  assert (result >= 0);
  std::cout << "Asserted nn_connect on " << URL << std::endl;
  std::cout << "Receiving"<< std::endl;
  while (1)
    {
      char *buf = NULL;
      int bytes = nn_recv (sock, &buf, NN_MSG, 0);
      assert (bytes >= 0);
      printf ("CLIENT: RECEIVED %s\n", buf);
      nn_freemsg (buf);
    }
  return nn_shutdown (sock, 0);
}

int main (const int argc, const char **argv)
{
  if (argc != 2) {
    fprintf (stderr, "Usage: pubsub %s|%s \n", SERVER, CLIENT);
    return 1;
  }
  if (strncmp(SERVER, argv[1], strlen(SERVER)) == 0)
    return server();
  else if (strncmp (CLIENT, argv[1], strlen (CLIENT)) == 0)
    return client();
}
