/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/core_messages.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_CORE_MESSAGES_HPP_
#define dslam_message_runtime_CORE_MESSAGES_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <string>
#include "header.hpp"
#include "message.hpp"
#include "to_be_deprecated/builders.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Header
*****************************************************************************/

template<>
struct Message<PacketHeader> {
  static void encode(const PacketHeader& header, ByteArray& buffer);
  static PacketHeader decode(const unsigned char* buffer, const unsigned int& size);
};

/*****************************************************************************
** String
*****************************************************************************/

template<>
struct Message<std::string> {
  static void encode(const std::string& s, ByteArray& buffer);
  static std::string decode(const unsigned char* buffer, const unsigned int& size);
};

} // dslam

#endif /* dslam_message_runtime_CORE_MESSAGES_HPP_ */
