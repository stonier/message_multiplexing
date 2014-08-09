/**
 * @file /dslam_message_runtime/src/lib/subscriber.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <nanomsg/nn.h>
#include "../../include/dslam_message_runtime/multiplexer.hpp"
#include "../../include/dslam_message_runtime/subscriber.hpp"

// for debugging
//#include <cstring>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Implementation
*****************************************************************************/

Subscriber::Subscriber(const std::string &name, const std::string &url) :
    name(name),
    socket(MessageMultiplexer::createSubscriber(name, url))
{}

Subscriber::~Subscriber() {
  if ( socket > 0 ) {
    nn_shutdown (socket, 0);
  }
}
void Subscriber::spin() {
  while (1)
  {
    char *buf = NULL;
    int bytes = nn_recv (socket, &buf, NN_MSG, 0);
    // assert (bytes >= 0);
    std::cout << "CLIENT " << name << ": RECEIVED " << buf << "\n" << std::endl;
    nn_freemsg (buf);
  }
}
void Subscriber::spinOnce() {
  char *buf = NULL;
  int bytes = nn_recv (socket, &buf, NN_MSG, 0);
  // assert (bytes >= 0);
  std::cout << "CLIENT " << name << ": RECEIVED " << buf << "\n" << std::endl;
  nn_freemsg (buf);
//  char *buf = NULL;
//  size_t data_len = 4; //strlen('dude');
//  buf = (char*) malloc(data_len + 1);
//  //int bytes = nn_recv (socket, &buf, NN_MSG, 0);
//  int bytes = nn_recv (socket, &buf, data_len + 1, 0);
//  // assert (bytes >= 0);
//  std::cout << "Bytes received: " << bytes << std::endl;
//  std::cout << "First byte: " << static_cast<int>(buf[0]) << std::endl;
//  std::cout << "CLIENT " << name << ": RECEIVEDD " << buf << "\n" << std::endl;
//  free(buf);
  //nn_freemsg (buf);
}

} // dslam
