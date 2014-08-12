/**
 * @file /dslam_message_runtime/src/lib/messages/core.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../../include/dslam_message_runtime/messages/core.hpp"
#include <ecl/containers/stencil.hpp>

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
  unsigned int window_size = sizeof(header.signature());
  ByteStencil stencil(buffer, buffer.begin(), buffer.begin() + window_size); to_byte_array(stencil, header.signature());
  stencil.resettle(window_size, window_size);   to_byte_array(stencil, header.id());
  stencil.resettle(2*window_size, window_size); to_byte_array(stencil, header.reserved());
  stencil.resettle(3*window_size, window_size); to_byte_array(stencil, header.length());
}

PacketHeader Message<PacketHeader>::decode(const unsigned char* buffer, const unsigned int& size) {
  // TODO : check PacketHeader::size == size, throw exception
  unsigned int signature, id, reserved, length;
  ecl::Converter<unsigned int, ConstRawByteStencil> from_byte_array;
  unsigned int window_size = sizeof(signature);
  ConstRawByteStencil stencil(buffer, size, buffer, buffer+window_size); signature = from_byte_array(stencil);
  stencil.resettle(  window_size, window_size); id = from_byte_array(stencil);
  stencil.resettle(2*window_size, window_size); reserved = from_byte_array(stencil);
  stencil.resettle(3*window_size, window_size); length = from_byte_array(stencil);
  return PacketHeader(id, length);
}

void Message<std::string>::encode(const std::string& s, ByteArray& buffer) {
  buffer.insert(buffer.end(), s.begin(), s.end());
}

std::string Message<std::string>::decode(const unsigned char* buffer, const unsigned int& size) {
  return std::string(buffer, buffer + size);  // c++0x buffer+size -> std::end(buffer)?
}

} // namespace dslam
