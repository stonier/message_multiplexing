/**
 * @file /mm_core_msgs/include/mm_core_msgs/integers.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_core_msgs_INTEGERS_HPP_
#define mm_core_msgs_INTEGERS_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <mm_messages/message.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

typedef unsigned char uint8;
typedef signed char int8;

/*****************************************************************************
** Integers
*****************************************************************************/

template<>
struct Message<uint8> {
  static void encode(const uint8& i, ByteArray& buffer);
  static uint8 decode(const unsigned char* buffer, const unsigned int& size = 1);
};

} // namespace mm_messages

#endif /* mm_core_msgs_STRING_HPP_ */
