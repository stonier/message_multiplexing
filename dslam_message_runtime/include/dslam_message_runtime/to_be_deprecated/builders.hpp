/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/to_be_deprecated/builders.hpp
 * 
 * @brief Converters for basic types.
 *
 * TODO : switch to ecl byte array converters
 * TODO : set these up as proper Message class types with encode/decode and restricted specialisations
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_BUILDERS_HPP_
#define dslam_message_runtime_BUILDERS_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../message.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {
namespace to_be_deprecated {

/*****************************************************************************
** Interfaces
*****************************************************************************/

/**
 * @brief Move bytes from a variable into the back end of a buffer.
 *
 * This covers just the fundamental types.
 *
 * @param value
 * @param buffer
 */
template<typename T>
void buildBytes(const T & value, ByteArray& buffer) {
  unsigned int size_value(sizeof (T));
  for (unsigned int i = 0; i < size_value; i++) {
    buffer.push_back(static_cast<unsigned char> ((value >> (i * 8)) & 0xff));
  }
}

/**
 * @brief Build fundamental variables from a received buffer (decode).
 *
 * This is the reverse of the buildBytes function for simple variables.
 * @param value
 * @param buffer
 */
template<typename T>
void buildVariable(T & value, ByteArray& buffer) {
  if (buffer.size() < sizeof (T))
    return;
  value = static_cast<unsigned char> (buffer[0]);

  unsigned int size_value(sizeof (T));
  for (unsigned int i = 1; i < size_value; i++) {
    value |= ((static_cast<unsigned char> (buffer[i])) << (8 * i));
  }
  buffer.erase(buffer.begin(), buffer.begin() + size_value);
}

/**
 * @brief C version of the buildVariable method.
 *
 * Make your checks on buffer size outside this call!
 *
 * @param value
 * @param buffer
 */
template<typename T>
void buildVariable(T & value, const unsigned char* buffer) {
  value = buffer[0];
  unsigned int size_value(sizeof (T));
  for (unsigned int i = 1; i < size_value; i++) {
    value |= ((static_cast<unsigned char> (buffer[i])) << (8 * i));
  }
}

} // namespace to_be_deprecated
} // namespace dslam

#endif /* dslam_message_runtime_BUILDERS_HPP_ */
