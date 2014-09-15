/**
 * @file /mm_mux_demux/include/mm_mux_demux/radio.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_mux_demux_RADIO_HPP_
#define mm_mux_demux_RADIO_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <string>
#include <memory>
#include <mm_messages/verbosity.hpp>
#include "mux.hpp"
#include "demux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {
namespace impl {

class RadioMuxDemux {
public:
private:

};
} // namespace impl
} // namespace mm_mux_demux

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

class RadioMuxDemux {
public:
  /**
   * @brief Register a mux/demux for a radio server of the specified name.
   * @param name
   * @param url
   * @param verbosity
   */
  static void startServer(const std::string& name,
                            const std::string& url,
                            const mm_messages::Verbosity::Level& verbosity=mm_messages::Verbosity::QUIET);
  /**
   * @brief Register a mux/demux for a radio client of the specified name.
   * @param name
   * @param url
   * @param verbosity
   */
  static void startClient(const std::string& name,
                            const std::string& url,
                            const mm_messages::Verbosity::Level& verbosity=mm_messages::Verbosity::QUIET);
  /**
   * @brief Shutdown only the radio (i.e. it's mux/demux) of the specified name.
   * @param name
   */
  static void shutdown(const std::string& name);
  /**
   * @brief Shutdown all named radios.
   */
  static void shutdown();

};

} // namespace mm_mux_demux

#endif /* mm_mux_demux_RADIO_HPP_ */
