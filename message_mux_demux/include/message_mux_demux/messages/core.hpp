/**
 * @file /message_mux_demux/include/message_mux_demux/core_messages.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_CORE_MESSAGES_HPP_
#define message_mux_demux_CORE_MESSAGES_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <string>
#include "../header.hpp"
#include "../messages.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_multiplexing {

/*****************************************************************************
** Header
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

/*****************************************************************************
** String
*****************************************************************************/

template<>
struct Message<std::string> {
  static void encode(const std::string& s, ByteArray& buffer);
  static std::string decode(const unsigned char* buffer, const unsigned int& size);
};

} // namespace message_multiplexing

#endif /* message_mux_demux_CORE_MESSAGES_HPP_ */
