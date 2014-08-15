/**
 * @file /message_mux_demux/include/message_mux_demux/exceptions.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef message_mux_demux_EXCEPTIONS_HPP_
#define message_mux_demux_EXCEPTIONS_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <stdexcept>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace message_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

struct MessageRegistrationFailed : public std::logic_error {
  explicit MessageRegistrationFailed(const std::string& s) : logic_error(s) {}
};

struct UnregisteredID : public std::runtime_error {
  explicit UnregisteredID(const std::string& s) : std::runtime_error(s) {
  }
};

struct InvalidIDTypeCombination : public std::runtime_error {
  explicit InvalidIDTypeCombination(const std::string& s) : std::runtime_error(s) {
  }
};

} // message_mux_demux

#endif /* message_mux_demux_EXCEPTIONS_HPP_ */
