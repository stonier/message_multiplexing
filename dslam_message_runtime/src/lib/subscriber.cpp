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
#include "../../include/dslam_message_runtime/manager.hpp"
#include "../../include/dslam_message_runtime/subscriber.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Implementation
*****************************************************************************/

Subscriber::Subscriber(const std::string &name, const std::string &url) :
    name(name),
    socket(MessageManager::createSubscriber(name, url))
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
} // dslam
