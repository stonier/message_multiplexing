/**
 * @file /message_mux_demux/include/message_mux_demux/messages.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_MESSAGES_HPP_
#define message_mux_demux_MESSAGES_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/containers.hpp>
#include <iostream>
#include <vector>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {

//typedef ecl::Array<unsigned char> ByteArray;
typedef std::vector<unsigned char> ByteArray;
typedef ecl::Stencil<ByteArray> ByteStencil;
typedef ecl::Stencil<unsigned char*> RawByteStencil;
typedef ecl::Stencil<const unsigned char*> ConstRawByteStencil;

/*****************************************************************************
** Interfaces
*****************************************************************************/

template <typename T>
struct Message {
  static void encode(const T& msg, ByteArray& buffer) {
    std::cout << "Psuedo-Encoding"<< std::endl;
  }

  static T decode(const unsigned char* buffer, const unsigned int& size) {
    std::cout << "Pseudo-Decoding" << std::endl;
    return T();
  }
};

} // namespace message_mux_demux

#endif /* message_mux_demux_MESSAGES_HPP_ */
