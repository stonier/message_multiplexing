/**
 * @file /dslam_message_runtime/src/lib/core_messages.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/dslam_message_runtime/core_messages.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Implementation
*****************************************************************************/

void Message<PacketHeader>::encode(const PacketHeader& header, ByteArray& buffer) {
  to_be_deprecated::buildBytes(header.signature(), buffer);
  to_be_deprecated::buildBytes(header.id(), buffer);
  to_be_deprecated::buildBytes(header.reserved(), buffer);
  to_be_deprecated::buildBytes(header.length(), buffer);
}

PacketHeader Message<PacketHeader>::decode(const unsigned char* buffer, const unsigned int& size) {
  unsigned int signature, id, reserved, length;
  to_be_deprecated::buildVariable(signature, buffer);
  to_be_deprecated::buildVariable(id, buffer+4);
  to_be_deprecated::buildVariable(reserved, buffer+8);
  to_be_deprecated::buildVariable(length, buffer+12);
  return PacketHeader(id, length);
}

void Message<std::string>::encode(const std::string& s, ByteArray& buffer) {
  buffer.insert(buffer.end(), s.begin(), s.end());
}

std::string Message<std::string>::decode(const unsigned char* buffer, const unsigned int& size) {
  return std::string(buffer, buffer + size);  // c++0x buffer+size -> std::end(buffer)?
}

} // namespace dslam
