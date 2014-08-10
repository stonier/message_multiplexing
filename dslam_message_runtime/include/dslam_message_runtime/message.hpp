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
#include <string>
#include <vector>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

template <typename T>
class Message {
public:
  const std::vector<char>& encode(const T& msg) {
    std::cout << "Psuedo-Encoding"<< std::endl;
    return std::vector<char>();
  }

  const T& decode(const std::vector<char>& buffer) {
    std::cout << "Pseudo-Decoding" << std::endl;
    return T();
  }
};

template<>
class Message<std::string> {
public:

  static std::vector<char> encode(const std::string& s) {
    std::cout << "Pseudo-Encoding strings" << std::endl;
    return std::vector<char>(s.begin(), s.end());
  }

  static std::string decode(const char* buffer) {
    std::cout << "Pseudo-Decoding strings" << std::endl;
    return std::string(buffer);
  }
};

} // dslam

#endif /* dslam_message_runtime_MESSAGE_HPP_ */
