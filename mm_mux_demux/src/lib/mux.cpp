/**
 * @file /mm_mux_demux/src/lib/mux.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <ecl/time/timestamp.hpp>
#include <mm_messages.hpp>
#include <nanomsg/pubsub.h>
#include <errno.h>
#include <nanomsg/utils/err.h>
#include "../../include/mm_mux_demux/mux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {
namespace impl {

/*****************************************************************************
 ** Implementation
 *****************************************************************************/

MessageMux::MessageMux(const std::string& name,
                       const std::string& url,
                       const mm_messages::Verbosity::Level& verbosity,
                       const bool bind) :
  name(name),
  url(url),
  verbosity(verbosity)
{
  socket = nn_socket (AF_SP, NN_PUB);
  if ( socket < 0 ) {
    // TODO : throw exception
    std::cout << "Mux socket error: " << nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
  }
  nn_setsockopt (socket, NN_SOL_SOCKET, NN_SOCKET_NAME, name.c_str(), name.size());
  if ( bind ) {
    endpoint_id = nn_bind(socket, url.c_str());
  } else {
    endpoint_id = nn_connect(socket, url.c_str());
  }
  if (endpoint_id < 0) {
    // TODO : throw exception
    std::cout << "Mux bind error: " << nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
  }
  if (verbosity > mm_messages::Verbosity::QUIET) {
    std::cout << "[" << ecl::TimeStamp() << "] MessageMux : [" << name << "][" << url << "][" << socket << "][" << endpoint_id << "]";
    if ( bind ) {
      std::cout << "[bind]" << std::endl;
    } else {
      std::cout << "[connect]" << std::endl;
    }
  }
}

MessageMux::~MessageMux() {
  if ( socket > 0 ) {
    nn_shutdown (socket, endpoint_id);
  }
}

int MessageMux::send(const unsigned int& id, const mm_messages::ByteArray& msg_buffer) {
  // this is identical to radio's send, could be collapsed
  mm_messages::ByteArray buffer;
  mm_messages::Message<mm_messages::PacketHeader>::encode(mm_messages::PacketHeader(), buffer);
  mm_messages::Message<mm_messages::SubPacketHeader>::encode(mm_messages::SubPacketHeader(id, msg_buffer.size()), buffer);
  buffer.insert(buffer.end(), msg_buffer.begin(), msg_buffer.end());

  if (verbosity > mm_messages::Verbosity::LOW) {
    std::cout << "[" << ecl::TimeStamp() << "] Mux: [" << id << "][" << buffer.size() << "][";
    std::cout << std::hex;
    for(unsigned int i=0; i < buffer.size(); ++i ) {
      std::cout << static_cast<unsigned int>(buffer[i]) << " ";
    }
    std::cout << std::dec;
    std::cout << "]" << std::endl;
  }
  int result = ::nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
  // TODO : lots of error flags to check here
  return 0;
}

} // namespace impl
} // mm_mux_demux


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

/*****************************************************************************
** Global Statics
*****************************************************************************/
/**
 * @brief Pre-establish named connections.
 * @param name
 * @param url
 */
void MessageMux::start(const std::string& name,
                             const std::string& url,
                             const mm_messages::Verbosity::Level& verbosity,
                             const bool bind) {
  MuxMapConstIterator iter = multiplexers().find(name);
  if ( iter == multiplexers().end() ) {
    if (url.empty()) {
      // TODO : throw an exception
    } else {
      std::pair<MuxMapIterator,bool> result;
      result = multiplexers().insert(
          MuxMapPair(name, std::make_shared<impl::MessageMux>(name, url, verbosity, bind)));
    }
  } else if ( !url.empty() ) {
    // TODO : throw an exception, name-url already present.
  }
}

void MessageMux::shutdown(const std::string& name) {
  multiplexers().erase(name);
}

void MessageMux::shutdown() {
  multiplexers().clear();
}

MessageMux::MuxMap& MessageMux::multiplexers() {
  static MessageMux::MuxMap map;
  return map;
}

int MessageMux::send(const std::string& name, const unsigned int& id, const mm_messages::ByteArray& msg_buffer) {
  MuxMapIterator iter = multiplexers().find(name);
  if ( iter != multiplexers().end() ) {
    return (iter->second)->send(id, msg_buffer);
  } else {
    // exceptions exceptions...
    std::cout << "Mux : no mux by that name found (while trying to send)"<< std::endl;
    return NotAvailable;
  }
}

} // namespace mm_mux_demux
