/**
 * @file /dslam_message_runtime/src/lib/header.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/dslam_message_runtime/header.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Statics
*****************************************************************************/

const unsigned int PacketHeader::size = 16;

PacketHeader::PacketHeader(const unsigned int& id, const unsigned int& length)
  : header0(0xaa55aa55) // signature
  , header1(id)         // id
  , header2(0xffffffff) // reserved1
  , header3(length)     // length
{}

} // dslam
