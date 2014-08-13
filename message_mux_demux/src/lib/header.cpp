/**
 * @file /message_mux_demux/src/lib/header.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <vector>
#include "../../include/message_mux_demux/header.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_multiplexing {

/*****************************************************************************
** Implementation
*****************************************************************************/

const unsigned int PacketHeader::size = sizeof(unsigned int);
const unsigned int SubPacketHeader::size = 2*sizeof(unsigned int);

SubPacketHeader::SubPacketHeader(const unsigned int& id, const unsigned int& length)
  : id(id)
  , length(length)
{}

} // namespace message_multiplexing
