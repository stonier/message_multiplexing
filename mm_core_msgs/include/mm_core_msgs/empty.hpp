/**
 * @file /mm_core_msgs/include/mm_core_msgs/empty.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_core_msgs_EMPTY_HPP_
#define mm_core_msgs_EMPTY_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <cstddef>
#include <mm_messages/message.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
 ** Typedefs
 *****************************************************************************/

typedef std::nullptr Empty;

/*****************************************************************************
** Interfaces
*****************************************************************************/

template<>
struct Message<Empty> {
  static void encode(const Empty& nothing_to_see_here, ByteArray& buffer);
  static std::string decode(const unsigned char* buffer, const unsigned int& size = 0);
};


} // namespace mm_messages

#endif /* mm_core_msgs_EMPTY_HPP_ */
