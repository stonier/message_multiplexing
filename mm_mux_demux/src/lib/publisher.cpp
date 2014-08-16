/**
 * @file /mm_mux_demux/src/lib/publisher.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/mm_mux_demux/publisher.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

/*****************************************************************************
** Implementation
*****************************************************************************/

Publisher::Publisher(const std::string &name, const std::string &url) :
  name(name)
{
  if (!url.empty()) {
    MessageMux::registerMux(name, url);
  }
  // else might want to check name is actually up
}

} // namespace mm_mux_demux
