/**
 * @file /mm_core_msgs/src/lib/messages/empty.cpp
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/mm_core_msgs/empty.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Implementation
*****************************************************************************/

void Message<mm_core_msgs::Empty>::encode(const mm_core_msgs::Empty& nothing_to_see_here, ByteArray& buffer) {
  // publishers always send empty buffers, but in case others use this
  if(!buffer.empty()) {
    buffer.clear();
  }
}

mm_core_msgs::Empty Message<mm_core_msgs::Empty>::decode(const unsigned char* buffer, const unsigned int& size) {
  return mm_core_msgs::Empty();
}

} // namespace mm_messages
