/**
 * @file /mm_mux_demux/include/mm_mux_demux/verbosity.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_mux_demux_VERBOSITY_HPP_
#define mm_mux_demux_VERBOSITY_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

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

} // namespace mm_mux_demux

#endif /* mm_mux_demux_VERBOSITY_HPP_ */
