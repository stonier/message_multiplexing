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
#include <sstream>
#include <string>
#include <vector>
#include "exceptions.hpp"
#include "registry.hpp"
#include "mux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class Publisher {
public:
  Publisher(const std::string &name, const std::string &url = "");
  virtual ~Publisher() {}

  /**
   * @brief Shunt a message off to be published.
   * @param id
   * @param msg
   * @tparam T the data type for the message.
   * @exception UnregisteredID : if this packet id hasn't been registered by the mux.
   * @exception InvalidIDTypeCombination : if this packet id doesn't correspond to the template msg type.
   */
  template<typename T>
  void publish(const unsigned int& id, const T& msg) {
    if ( !MessageRegistry::isRegistered(id) ) {
      std::stringstream ss;
      ss << "id '" << id << "' has not been registered";
      throw UnregisteredID(ss.str());
    }
    if ( !MessageRegistry::isRegisteredWithType<T>(id) ) {
      std::stringstream ss;
      ss << "id '" << id << "' is registered, but not with this type";
      throw InvalidIDTypeCombination(ss.str());
    }
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

} // namespace message_mux_demux

#endif /* message_mux_demux_PUBLISHER_HPP_ */
