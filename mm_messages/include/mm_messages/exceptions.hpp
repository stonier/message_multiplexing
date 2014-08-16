/**
 * @file /mm_messages/include/mm_messages/exceptions.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_messages_EXCEPTIONS_HPP_
#define mm_messages_EXCEPTIONS_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <stdexcept>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

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

struct FailedToFindMessageSpecialisation : public std::logic_error {
  explicit FailedToFindMessageSpecialisation(const std::string& s) : logic_error(s) {}
};

} // mm_messages

#endif /* mm_messages_EXCEPTIONS_HPP_ */
