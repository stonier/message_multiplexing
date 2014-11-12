/**
 * @file /mm_messages/include/mm_messages/publisher.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_messages_PUBLISHER_HPP_
#define mm_messages_PUBLISHER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <ecl/exceptions.hpp>
#include "exceptions.hpp"
#include "registry.hpp"
#include <sstream>
#include <string>
#include <vector>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Interfaces
*****************************************************************************/
/**
 * @brief Template parent class for specific publishers.
 *
 * This gets typedef'd for specific publisher types in each package (e.g.
 * MessageDemux, RadioMuxDemux)
 */
template<typename Multiplexer>
class Publisher {
public:
  typedef mm_messages::MessageRegistry MessageRegistry;
  typedef mm_messages::ByteArray ByteArray;

  Publisher(const std::string &name)
  : name(name)
  {
    // might want to check 'name' is actually up on Multiplexer
  }

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
    mm_messages::Message<T>::encode(msg, buffer);
    try {
      int result = Multiplexer::send(name, id, buffer);  // this throws ecl::StandardException(ecl::TimeOutError)
    } catch (ecl::StandardException &e) {
      if ( e.flag() == ecl::TimeOutError ) {
        // quiet...could use this to update internally so that it knows there is a client or not.
        // could pass a boolean to this flag so that it knows to be verbose.
      } else {
        throw ecl::StandardException(e);
      }
    }
  }

private:
  std::string name;
};

//class PseudoPublisher {
//public:
//  PseudoPublisher(const std::string &name, const std::string &url = "") {}
//
//  template<typename T>
//  void publish(const unsigned int& id, const T& msg) {}
//};

} // namespace mm_messages

#endif /* mm_messages_PUBLISHER_HPP_ */
