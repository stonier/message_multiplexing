/**
 * @file /mm_messages/include/mm_messages/messages.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_messages_MESSAGES_HPP_
#define mm_messages_MESSAGES_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/containers.hpp>
#include <iostream>
#include <vector>
#include "exceptions.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

//typedef ecl::Array<unsigned char> ByteArray;
typedef std::vector<unsigned char> ByteArray;
typedef ecl::Stencil<ByteArray> ByteStencil;
typedef ecl::Stencil<unsigned char*> RawByteStencil;
typedef ecl::Stencil<const unsigned char*> ConstRawByteStencil;

/*****************************************************************************
** Interfaces
*****************************************************************************/

/**
 * @brief Template parent and default pass-through encoding/decoding.
 *
 * Implement serialisations in specialisations of this class.
 *
 * @tparam T : the data type (e.g. std::string)
 */
template <typename T>
struct Message {
  static void encode(const T& msg, ByteArray& buffer) {
    throw FailedToFindMessageSpecialisation("Failed to find the specialisation to encode this message type, did you include the message header?");
  }
  static T decode(const unsigned char* buffer, const unsigned int& size) {
    throw FailedToFindMessageSpecialisation("Failed to find the specialisation to decode this message type, did you include the message header?");
    //return T();
  }
};

} // namespace mm_messages

#endif /* mmmessages_MESSAGES_HPP_ */
