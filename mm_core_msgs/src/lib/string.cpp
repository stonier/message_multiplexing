/**
 * @file /mm_core_msgs/src/lib/messages/core.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <mm_messages/registry.hpp>
#include <ecl/containers/stencil.hpp>
#include "../../include/mm_core_msgs/string.hpp"
#include "../../include/mm_core_msgs/test_identifiers.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Implementation
*****************************************************************************/

void Message<std::string>::encode(const std::string& s, ByteArray& buffer) {
  // publishers always send empty buffers, but in case others use this
  if(!buffer.empty()) {
    buffer.clear();
  }
  buffer.insert(buffer.end(), s.begin(), s.end());
}

std::string Message<std::string>::decode(const unsigned char* buffer, const unsigned int& size) {
  return std::string(buffer, buffer + size);  // c++0x buffer+size -> std::end(buffer)?
}

} // namespace mm_messages

//namespace mm_messages_registry_entries {
//    const mm_messages::RegistryEntry<1, std::string> unused_packet_info_dude("Core official test message packet id for strings");
//}
MM_REGISTER_PACKET_INFO(mm_core_msgs::TestStrings, std::string, "Core official test message packet id for strings");
