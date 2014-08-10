/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/message.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_MESSAGE_HPP_
#define dslam_message_runtime_MESSAGE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <vector>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

typedef std::vector<unsigned char> ByteArray;

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

} // dslam

#endif /* dslam_message_runtime_MESSAGE_HPP_ */
