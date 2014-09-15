/**
 * @file /mm_mux_demux/include/mm_mux_demux/mux.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef mm_mux_demux_MUX_HPP_
#define mm_mux_demux_MUX_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <map>
#include <memory>
#include <string>
#include <mm_messages.hpp>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {
namespace impl {

/*****************************************************************************
 ** Interfaces
 *****************************************************************************/

class MessageMux {
public:
  MessageMux(const std::string& name,
             const std::string& url,
             const mm_messages::Verbosity::Level& verbosity=mm_messages::Verbosity::QUIET,
             const bool bind = true
             );
  ~MessageMux();
  int send(const unsigned int& id, const mm_messages::ByteArray& msg_buffer);

private:
  std::string name, url;
  int socket, endpoint_id;
  mm_messages::Verbosity::Level verbosity;
};

} // namespace impl
} // mm_mux_demux

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class MessageMux {
public:
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>> MuxMap;
  typedef std::pair<std::string, std::shared_ptr<impl::MessageMux>> MuxMapPair;
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>>::iterator MuxMapIterator;
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>>::const_iterator MuxMapConstIterator;

  enum Errors {
    NotAvailable = -1  // when you try to connect to <name>, but <name> has not yet been started
  };
  static void start(const std::string& name,
                    const std::string& url,
                    const mm_messages::Verbosity::Level& verbosity=mm_messages::Verbosity::QUIET,
                    const bool bind = true
                   );
  static void shutdown(const std::string& name);  // shutdown only this mux
  static void shutdown(); // shutdown all muxes
  static MuxMap& multiplexers();
  static int send(const std::string& name, const unsigned int& id, const mm_messages::ByteArray& msg_buffer);
};

} // namespace mm_mux_demux

#endif /* mm_mux_demux_MUX_HPP_ */
