/**
 * @file /mm_messages/include/mm_messages/verbosity.hpp
 *
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_messages_VERBOSITY_HPP_
#define mm_messages_VERBOSITY_HPP_

/*****************************************************************************
** Macros
*****************************************************************************/

// Used to print new packet type registrations to stdout as libraries get loaded
// #define MM_DEBUG_REGISTRY

/*****************************************************************************
** Includes
*****************************************************************************/

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_messages {

/*****************************************************************************
** Interfaces
*****************************************************************************/

struct Verbosity {
  enum Level {
    QUIET,
    LOW,  // just show packet statistics
    HIGH  // show raw bytes of incoming packets
  };
};

} // namespace mm_messages

#endif /* mm_messages_VERBOSITY_HPP_ */
