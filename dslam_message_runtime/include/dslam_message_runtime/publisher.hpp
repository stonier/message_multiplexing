/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/publisher.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_PUBLISHER_HPP_
#define dslam_message_runtime_PUBLISHER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <nanomsg/nn.h>
#include <vector>
#include "multiplexer.hpp"
#include "message.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

template <typename T>
class Publisher {
public:
  Publisher(const std::string &name, const std::string &url = "") :
    socket(MessageMultiplexer::createPublisher(name, url))
  {
    std::cout << "Created socket" << std::endl;
  }
  ~Publisher() {
    if ( socket > 0 ) {
      nn_shutdown (socket, 0);
    }
  }

  void publish(const T& msg) {
    std::cout << "Socket: " << socket << std::endl;
    if ( socket >= 0 ) {
      std::vector<char> buffer = Message<T>::encode(msg);
      std::cout << "Sending: " << buffer.size() << std::endl;
      const char* data = "dude";
      // int result = nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
      int result = nn_send(socket, data, 4, 0); // last option is flags, only NN_DONTWAIT available
      std::cout << "Publishing result: " << result << std::endl;
      // lots of error flags to check here
    }
  }

private:
  int socket;
};

template <typename T>
class PseudoPublisher {
public:
  PseudoPublisher(const std::string &name, const std::string &url = "") {}

  void publish(const T& msg) {}
};

} // dslam

#endif /* dslam_message_runtime_PUBLISHER_HPP_ */
