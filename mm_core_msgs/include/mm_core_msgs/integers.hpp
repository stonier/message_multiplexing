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

namespace mm_core_msgs {

typedef unsigned char uint8;
typedef signed char int8;

} // namespace mm_core_msgs

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

template<>
struct Message<mm_core_msgs::uint8> {
  static void encode(const mm_core_msgs::uint8& i, ByteArray& buffer);
  static mm_core_msgs::uint8 decode(const unsigned char* buffer, const unsigned int& size = 1);
};

} // namespace mm_messages

#endif /* mm_core_msgs_STRING_HPP_ */
