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

namespace mm_core_msgs {

/*****************************************************************************
 ** Typedefs
 *****************************************************************************/

struct Empty {};

} // namespace mm_core_msgs

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Interfaces
*****************************************************************************/

template<>
struct Message<mm_core_msgs::Empty> {
  static void encode(const mm_core_msgs::Empty& nothing_to_see_here, ByteArray& buffer);
  static std::string decode(const unsigned char* buffer, const unsigned int& size = 0);
};


} // namespace mm_messages

#endif /* mm_core_msgs_EMPTY_HPP_ */
