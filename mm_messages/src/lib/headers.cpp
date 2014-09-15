/**
 * @file /mm_messages/src/lib/messages/core.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/mm_messages/headers.hpp"
#include <ecl/containers/stencil.hpp>
#include <ecl/converters.hpp>
//#include "../../../include/mm_messages/registry.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Definitions
*****************************************************************************/

const unsigned int PacketHeader::size = sizeof(unsigned int);
const unsigned int SubPacketHeader::size = 2*sizeof(unsigned int);

SubPacketHeader::SubPacketHeader(const int& id, const unsigned int& length)
  : id(id)
  , length(length)
{}

/*****************************************************************************
** Serialisations
*****************************************************************************/

void Message<PacketHeader>::encode(const PacketHeader& header, ByteArray& buffer) {
  // This is the header...the buffer should be clear anyway.
  buffer.resize(PacketHeader::size);
  ecl::Converter<ByteArray, unsigned int> to_byte_array;
  to_byte_array(buffer, header.signature);
}

PacketHeader Message<PacketHeader>::decode(const unsigned char* buffer, const unsigned int& size) {
  // TODO : check PacketHeader::size == size, throw exception
  unsigned int signature;
  ecl::Converter<unsigned int, ConstRawByteStencil> from_byte_array;
  unsigned int window_size = sizeof(unsigned int);
  ConstRawByteStencil stencil(buffer, size, buffer, buffer+window_size);
  signature = from_byte_array(stencil);
  return PacketHeader(signature);
}

void Message<SubPacketHeader>::encode(const SubPacketHeader& header, ByteArray& buffer) {
  ByteArray sub_packet_header_buffer(SubPacketHeader::size);
  unsigned int window_size = sizeof(unsigned int);
  ecl::Converter<ByteStencil, unsigned int> to_byte_array;
  ByteStencil stencil(sub_packet_header_buffer, sub_packet_header_buffer.begin(), buffer.end());
  stencil.resettle(0, window_size);           to_byte_array(stencil, header.id);
  stencil.resettle(window_size, window_size); to_byte_array(stencil, header.length);
  buffer.insert(buffer.end(), sub_packet_header_buffer.begin(), sub_packet_header_buffer.end());
}

SubPacketHeader Message<SubPacketHeader>::decode(const unsigned char* buffer, const unsigned int& size) {
  // TODO : check PacketHeader::size == size, throw exception
  unsigned int signature;
  ecl::Converter<unsigned int, ConstRawByteStencil> from_byte_array;
  unsigned int window_size = sizeof(unsigned int);
  unsigned int id, length;
  ConstRawByteStencil stencil(buffer, size, buffer, buffer+size);
  stencil.resettle(0,           window_size); id = from_byte_array(stencil);
  stencil.resettle(window_size, window_size); length = from_byte_array(stencil);
  return SubPacketHeader(id, length);
}

} // mm_messages
