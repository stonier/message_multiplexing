/**
 * @file /message_mux_demux/include/message_mux_demux/impl/header.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_HEADER_HPP_
#define message_mux_demux_HEADER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <vector>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {

/*****************************************************************************
** Interfaces
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

} // message_mux_demux

#endif /* message_mux_demux_HEADER_HPP_ */
