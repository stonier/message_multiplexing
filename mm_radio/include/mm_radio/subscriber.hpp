/**
 * @file /mm_radio/include/mm_radio/subscriber.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_radio_SUBSCRIBER_HPP_
#define mm_radio_SUBSCRIBER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/utilities/function_objects.hpp>
#include <mm_messages.hpp>
#include <sstream>
#include <string>
#include "radio.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_radio {

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
    Radio::registerSubscriber(name, id, &Subscriber<ID, DataType>::processPacket, (*this));
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
    Radio::registerSubscriber(name, id, &Subscriber<ID, DataType>::processPacket, (*this));
  }
  virtual ~Subscriber() {
    Radio::unregisterSubscriber(name, id);
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

} // namespace mm_radio

#endif /* mm_radio_SUBSCRIBER_HPP_ */
