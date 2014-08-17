/**
 * @file /mm_mux_demux/include/mm_mux_demux/publisher.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_mux_demux_PUBLISHER_HPP_
#define mm_mux_demux_PUBLISHER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/time.hpp>
#include <iostream>
#include <mm_messages/exceptions.hpp>
#include <mm_messages/registry.hpp>
#include <sstream>
#include <string>
#include <vector>
#include "mux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class Publisher {
public:
  typedef mm_messages::MessageRegistry MessageRegistry;
  typedef mm_messages::ByteArray ByteArray;

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
      throw mm_messages::UnregisteredID(ss.str());
    }
    if ( !MessageRegistry::isRegisteredWithType<T>(id) ) {
      std::stringstream ss;
      ss << "id '" << id << "' is registered, but not with this type";
      throw mm_messages::InvalidIDTypeCombination(ss.str());
    }
    ByteArray buffer;
    //stopwatch.restart();
    mm_messages::Message<T>::encode(msg, buffer);
    //std::cout << static_cast<int>(1000*stopwatch.split()) << std::endl;
    int result = MessageMux::send(name, id, buffer);
    //std::cout << static_cast<int>(1000*stopwatch.split()) << std::endl;
  }

private:
//  ecl::StopWatch stopwatch;
  std::string name;
};

class PseudoPublisher {
public:
  PseudoPublisher(const std::string &name, const std::string &url = "") {}

  template<typename T>
  void publish(const unsigned int& id, const T& msg) {}
};

} // namespace mm_mux_demux

#endif /* mm_mux_demux_PUBLISHER_HPP_ */
