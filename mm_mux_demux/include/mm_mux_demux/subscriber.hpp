/**
 * @file /mm_mux_demux/include/mm_mux_demux/subscriber.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_mux_demux_SUBSCRIBER_HPP_
#define mm_mux_demux_SUBSCRIBER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/utilities/function_objects.hpp>
#include <sstream>
#include <string>
#include "demux.hpp"
#include <mm_messages/exceptions.hpp>
#include <mm_messages/registry.hpp>
#include <mm_messages/headers.hpp>
#include <mm_messages/message.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

template<unsigned int ID, typename DataType>
class Subscriber {
public:
  typedef mm_messages::MessageRegistry MessageRegistry;
  typedef mm_messages::ByteArray ByteArray;

  /**
   * Construct with a callback to a global or static function.
   *
   * @param name : name of the demux to connect to
   * @param id : command id of the packet to associate with
   * @param f : the global/static function.
   */
  Subscriber(const std::string& name,
             void (*f)(DataType)) :
    name(name),
    id(ID),
    function(new ecl::UnaryFreeFunction<DataType>(f))
  {
    if ( !MessageRegistry::isRegistered(ID) ) {
      std::stringstream ss;
      ss << "id '" << id << "' has not been registered";
      throw mm_messages::UnregisteredID(ss.str());
    }
    if ( !MessageRegistry::isRegisteredWithType<DataType>(ID) ) {
      std::stringstream ss;
      ss << "id '" << id << "' is registered, but not with this type";
      throw mm_messages::InvalidIDTypeCombination(ss.str());
    }
    MessageDemux::registerSubscriber(name, id, &Subscriber<ID, DataType>::processPacket, (*this));
  }
  /**
   * Construct with a callback to a member function.
   *
   * @param name : name of the demux to connect to
   * @param id : command id of the packet to associate with
   * @param f : the member function to load.
   * @param c : the instance for the member function's class.
   * @tparam C : the member function's class type.
   */
  template<typename C>
  Subscriber(const std::string& name,
             void (C::*f)(DataType), C &c) :
    name(name),
    id(ID),
    function(new ecl::PartiallyBoundUnaryMemberFunction<C, DataType,void>(f,c))
  {
    MessageDemux::registerSubscriber(name, id, &Subscriber<ID, DataType>::processPacket, (*this));
  }
  virtual ~Subscriber() {
    MessageDemux::unregisterSubscriber(name, id);
    delete function;
  }

  void processPacket(const unsigned char* buffer, const unsigned int& size) {
    DataType data = mm_messages::Message<DataType>::decode(buffer, size);
    (*function)(data);
  }

private:
  std::string name;
  unsigned int id;
  ecl::UnaryFunction<DataType,void> *function;
};

} // namespace mm_mux_demux

#endif /* mm_mux_demux_SUBSCRIBER_HPP_ */
