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
#include "header.hpp"
#include "messages.hpp"
#include "messages/core.hpp"
#include "mux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class Publisher {
public:
  Publisher(const std::string &name, const std::string &url = "") :
    socket(MessageMux::createPublisher(name, url))
  {
    std::cout << "Created socket" << std::endl;
  }
  ~Publisher() {
    if ( socket > 0 ) {
      nn_shutdown (socket, 0);
    }
  }

  template<typename T>
  void publish(const unsigned int& id, const T& msg) {
    std::cout << "Socket: " << socket << std::endl;
    if ( socket >= 0 ) {
      ByteArray msg_buffer;
      Message<T>::encode(msg, msg_buffer);
      ByteArray buffer;
      PacketHeader header(id, msg_buffer.size());
      Message<PacketHeader>::encode(header, buffer);
      buffer.insert(buffer.end(), msg_buffer.begin(), msg_buffer.end());
      std::cout << "Sending: " << buffer.size() << std::endl;
//      const char* data = "dude";
      // int result = nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
      int result = nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
      std::cout << "Publishing result: " << result << std::endl;
      // lots of error flags to check here
    }
  }

private:
  int socket;
};

class PseudoPublisher {
public:
  PseudoPublisher(const std::string &name, const std::string &url = "") {}

  template<typename T>
  void publish(const unsigned int& id, const T& msg) {}
};

} // dslam

#endif /* dslam_message_runtime_PUBLISHER_HPP_ */
