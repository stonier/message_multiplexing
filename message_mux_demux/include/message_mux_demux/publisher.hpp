/**
 * @file /message_mux_demux/include/message_mux_demux/publisher.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_PUBLISHER_HPP_
#define message_mux_demux_PUBLISHER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <string>
#include <vector>
#include "mux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_multiplexing {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class Publisher {
public:
  Publisher(const std::string &name, const std::string &url = "") :
    name(name)
  {
    if (!url.empty()) {
      MessageMux::registerMux(name, url);
    }
    // else might want to check name is actually up
  }
  ~Publisher() {
  }

  template<typename T>
  void publish(const unsigned int& id, const T& msg) {
    ByteArray msg_buffer;
    Message<T>::encode(msg, msg_buffer);
    int result = MessageMux::send(name, id, msg_buffer);
  }

private:
  std::string name;
};

class PseudoPublisher {
public:
  PseudoPublisher(const std::string &name, const std::string &url = "") {}

  template<typename T>
  void publish(const unsigned int& id, const T& msg) {}
};

} // namespace message_multiplexing

#endif /* message_mux_demux_PUBLISHER_HPP_ */
