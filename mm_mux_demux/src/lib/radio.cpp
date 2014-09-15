/**
 * @file /mm_mux_demux/src/lib/radio.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include "../../include/mm_mux_demux/radio.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

/*****************************************************************************
** Implementation
*****************************************************************************/

void RadioMuxDemux::startServer(const std::string& name,
                          const std::string& url,
                          const mm_messages::Verbosity::Level& verbosity) {
  MessageMux::start(name, url, verbosity, true);  // bind on this one
  MessageDemux::start(name, url, verbosity, false);
}

void RadioMuxDemux::startClient(const std::string& name,
                          const std::string& url,
                          const mm_messages::Verbosity::Level& verbosity) {
  MessageMux::start(name, url, verbosity, false);
  MessageDemux::start(name, url, verbosity, false);
}

void RadioMuxDemux::shutdown(const std::string& name) {
  MessageMux::shutdown(name);
  MessageDemux::shutdown(name);
}

void RadioMuxDemux::shutdown() {
  MessageMux::shutdown();
  MessageDemux::shutdown();
}
} // mm_mux_demux
