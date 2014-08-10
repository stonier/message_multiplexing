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
  // This is the header...the buffer should be clear anyway.
  buffer.resize(PacketHeader::size);

  ecl::Converter<ByteStencil, unsigned int> to_byte_array;
  unsigned int size = sizeof(header.signature());
  ByteStencil stencil(buffer, buffer.begin(), buffer.begin() + size); to_byte_array(stencil, header.signature());
  stencil = ByteStencil(buffer, buffer.begin() +   size, buffer.begin() + 2*size); to_byte_array(stencil, header.id());
  stencil = ByteStencil(buffer, buffer.begin() + 2*size, buffer.begin() + 3*size); to_byte_array(stencil, header.reserved());
  stencil = ByteStencil(buffer, buffer.begin() + 3*size, buffer.begin() + 4*size); to_byte_array(stencil, header.length());
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
