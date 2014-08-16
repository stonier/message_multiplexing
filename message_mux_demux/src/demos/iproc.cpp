
#include <nanomsg/nn.h>
#include <nanomsg/pair.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/inproc.h>
#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#define SOCKET_ADDRESS "inproc://test"

static int test_socket (int family, int protocol)
{
    int sock;

    sock = nn_socket (family, protocol);
    if (sock == -1) {
      std::cout << "Error opening socket" << std::endl;
    }
    return sock;
}

static int test_connect (int sock, const char *address)
{
    int rc;
    rc = nn_connect (sock, address);
    if(rc < 0) {
      std::cout << "Error on connect"<< std::endl;
    }
    return rc;
}
static int test_bind(int sock, const char *address)
{
    int rc;

    rc = nn_bind (sock, address);
    if(rc < 0) {
      std::cout << "Error on bind"<< std::endl;
    }
    return rc;
}
static void test_send (int sock, const char *data)
{
    size_t data_len;
    int rc;

    data_len = strlen(data);

    std::cout << "Send: " << data << std::endl;
    rc = nn_send (sock, data, data_len, 0);
    if (rc < 0) {
      std::cout << "Error on send"<< std::endl;
    }
    if (rc != (int)data_len) {
      std::cout << "error on data to send"<< std::endl;
    }
}
static void test_recv (int sock, const char *data)
{
    size_t data_len;
    int rc;
    char *buf;

    data_len = strlen(data);
    /*  We allocate plus one byte so that we are sure that message received
        has corrent length and not truncated  */
    buf = (char*)malloc(data_len+1);
    //alloc_assert (buf);

    rc = nn_recv (sock, buf, data_len+1, 0);
    if (rc < 0) {
      std::cout << "Error on receive"<< std::endl;
    }
    if (rc != (int)data_len) {
      std::cout << "Error on data wrong length"<< std::endl;
    }
    if (memcmp (data, buf, data_len) != 0) {
      std::cout << "Error on received data is wrong"<< std::endl;
    }
    std::cout << "Receive: " << buf<< std::endl;

    free (buf);
}

static void test_close (int sock)
{
    int rc;

    rc = nn_close (sock);
    if (rc != 0) {
      std::cout << "Error on close"<< std::endl;
    }
}

void test_pair() {
  int rc;
  int sb;
  int sc;
  int i;
  char buf [256];
  int val;

  /*  Create a simple topology. */
  sc = test_socket (AF_SP, NN_PAIR);
  test_connect (sc, SOCKET_ADDRESS);
  sb = test_socket (AF_SP, NN_PAIR);
  test_bind (sb, SOCKET_ADDRESS);

  /*  Try a duplicate bind. It should fail. */
  rc = nn_bind (sc, SOCKET_ADDRESS);
  //nn_assert (rc < 0 && errno == EADDRINUSE);

  /*  Ping-pong test. */
  for (i = 0; i != 100; ++i) {

      test_send (sc, "ABC");
      test_recv (sb, "ABC");
      test_send (sb, "DEFG");
      test_recv (sc, "DEFG");
  }

  /*  Batch transfer test. */
  for (i = 0; i != 100; ++i) {
      test_send (sc, "XYZ");
  }
  for (i = 0; i != 100; ++i) {
      test_recv (sb, "XYZ");
  }

  test_close (sc);
  test_close (sb);

  /*  Test whether queue limits are observed. */
  sb = test_socket (AF_SP, NN_PAIR);
  val = 200;
  rc = nn_setsockopt (sb, NN_SOL_SOCKET, NN_RCVBUF, &val, sizeof (val));
  //errno_assert (rc == 0);
  test_bind (sb, SOCKET_ADDRESS);
  sc = test_socket (AF_SP, NN_PAIR);
  test_connect (sc, SOCKET_ADDRESS);

  val = 200;
  rc = nn_setsockopt (sc, NN_SOL_SOCKET, NN_SNDTIMEO, &val, sizeof (val));
  //errno_assert (rc == 0);
  i = 0;
  while (1) {
      rc = nn_send (sc, "0123456789", 10, 0);
      if (rc < 0 && nn_errno () == EAGAIN)
          break;
      //errno_assert (rc >= 0);
      //nn_assert (rc == 10);
      ++i;
  }
  //nn_assert (i == 20);
  test_recv (sb, "0123456789");
  test_send (sc, "0123456789");
  rc = nn_send (sc, "0123456789", 10, 0);
  //nn_assert (rc < 0 && nn_errno () == EAGAIN);
  for (i = 0; i != 20; ++i) {
      test_recv (sb, "0123456789");
  }

  /*  Make sure that even a message that doesn't fit into the buffers
      gets across. */
  for (i = 0; i != sizeof (buf); ++i)
      buf [i] = 'A';
  rc = nn_send (sc, buf, 256, 0);
  //errno_assert (rc >= 0);
  //nn_assert (rc == 256);
  rc = nn_recv (sb, buf, sizeof (buf), 0);
  //errno_assert (rc >= 0);
  //nn_assert (rc == 256);

  test_close (sc);
  test_close (sb);
}

void test_pubsub() {
  int rc;
      int pub1;
      int pub2;
      int sub1;
      int sub2;
      char buf [8];
      size_t sz;

      pub1 = test_socket (AF_SP, NN_PUB);
      test_bind (pub1, SOCKET_ADDRESS);
      sub1 = test_socket (AF_SP, NN_SUB);
      rc = nn_setsockopt (sub1, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
      //errno_assert (rc == 0);
      sz = sizeof (buf);
      rc = nn_getsockopt (sub1, NN_SUB, NN_SUB_SUBSCRIBE, buf, &sz);
      //nn_assert (rc == -1 && nn_errno () == ENOPROTOOPT);
      test_connect (sub1, SOCKET_ADDRESS);
      sub2 = test_socket (AF_SP, NN_SUB);
      rc = nn_setsockopt (sub2, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
      //errno_assert (rc == 0);
      test_connect (sub2, SOCKET_ADDRESS);

      /*  Wait till connections are established to prevent message loss. */
      sleep(1); //nn_sleep (10);

      test_send (pub1, "0123456789012345678901234567890123456789");
      test_recv (sub1, "0123456789012345678901234567890123456789");
      test_recv (sub2, "0123456789012345678901234567890123456789");

      test_close (pub1);
      test_close (sub1);
      test_close (sub2);

      /*  Check receiving messages from two publishers. */

      sub1 = test_socket (AF_SP, NN_SUB);
      rc = nn_setsockopt (sub1, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
      //rrno_assert (rc == 0);
      test_bind (sub1, SOCKET_ADDRESS);
      pub1 = test_socket (AF_SP, NN_PUB);
      test_connect (pub1, SOCKET_ADDRESS);
      pub2 = test_socket (AF_SP, NN_PUB);
      test_connect (pub2, SOCKET_ADDRESS);
      sleep(1); //nn_sleep (100);

      test_send (pub1, "0123456789012345678901234567890123456789");
      test_send (pub2, "0123456789012345678901234567890123456789");
      test_recv (sub1, "0123456789012345678901234567890123456789");
      test_recv (sub1, "0123456789012345678901234567890123456789");

      test_close (pub2);
      test_close (pub1);
      test_close (sub1);
}

int main ()
{
  test_pubsub();
    return 0;
}
