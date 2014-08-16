/**
 * @file /mm_messages/include/mm_messages/core_messages.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_messages_HEADERS_HPP_
#define mm_messages_HEADERS_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <string>
#include "message.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Definitions
*****************************************************************************/

struct PacketHeader {
  static const unsigned int size;
  PacketHeader(const unsigned int& signature=0xaa55aa55) : signature(signature) {}
  unsigned int signature;
};

struct SubPacketHeader {
  static const unsigned int size;
  SubPacketHeader(const unsigned int& id, const unsigned int& length);
  unsigned int id;
  unsigned int length;
};

/*****************************************************************************
** Serialisations
*****************************************************************************/

template<>
struct Message<PacketHeader> {
  /** Clears and adds the signature to the front of the buffer **/
  static void encode(const PacketHeader& header, ByteArray& buffer);
  static PacketHeader decode(const unsigned char* buffer, const unsigned int& size);
};

template<>
struct Message<SubPacketHeader> {
  /** @brief Appends the sub-packet id, length to the buffer **/
  static void encode(const SubPacketHeader& header, ByteArray& buffer);
  static SubPacketHeader decode(const unsigned char* buffer, const unsigned int& size);
};

} // namespace mm_messages

#endif /* mm_messages_HEADERS_HPP_ */
