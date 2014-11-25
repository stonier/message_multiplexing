/**
 * @file /mm_core_msgs/src/lib/integers.cpp
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <mm_messages/registry.hpp>
#include "../../include/mm_core_msgs/integers.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Implementation
*****************************************************************************/

void Message<uint8>::encode(const uint8& s, ByteArray& buffer) {
  // publishers always send empty buffers, but in case others use this
  if(!buffer.empty()) {
    buffer.clear();
  }
  buffer.push_back(s);
}

uint8 Message<uint8>::decode(const unsigned char* buffer, const unsigned int& size) {
  return *buffer;
}
} // namespace mm_messages

