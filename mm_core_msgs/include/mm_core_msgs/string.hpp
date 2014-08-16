/**
 * @file /mm_core_msgs/include/mm_core_msgs/string.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_core_msgs_STRING_HPP_
#define mm_core_msgs_STRING_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <string>
#include <mm_messages/message.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** String
*****************************************************************************/

template<>
struct Message<std::string> {
  static void encode(const std::string& s, ByteArray& buffer);
  static std::string decode(const unsigned char* buffer, const unsigned int& size);
};

} // namespace mm_messages

#endif /* mm_core_msgs_STRING_HPP_ */
