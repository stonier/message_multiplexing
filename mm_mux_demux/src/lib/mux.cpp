/**
 * @file /mm_mux_demux/src/lib/mux.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <mm_messages/headers.hpp>
#include <mm_messages/message.hpp>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
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
                       const bool verbose) :
  name(name),
  url(url),
  verbose(verbose)
{
  socket = nn_socket (AF_SP, NN_PUB);
  // TODO check >= 0, what does 0 mean?
  nn_setsockopt (socket, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
  int unused_result = nn_bind(socket, url.c_str());
  // TODO : check the result, throw exceptions if necessary
}

MessageMux::~MessageMux() {
  if ( socket > 0 ) {
    nn_shutdown (socket, 0);
  }
}

int MessageMux::send(const unsigned int& id, const mm_messages::ByteArray& msg_buffer) {
    mm_messages::ByteArray buffer;
    mm_messages::Message<mm_messages::PacketHeader>::encode(mm_messages::PacketHeader(), buffer);
    mm_messages::Message<mm_messages::SubPacketHeader>::encode(mm_messages::SubPacketHeader(id, msg_buffer.size()), buffer);
    buffer.insert(buffer.end(), msg_buffer.begin(), msg_buffer.end());

    if (verbose) {
      std::cout << "Mux : [" << id << "][" << buffer.size() << "] ";
      std::cout << std::hex;
      for(unsigned int i=0; i < buffer.size(); ++i ) {
        std::cout << static_cast<unsigned int>(buffer[i]) << " ";
      }
      std::cout << std::dec;
      std::cout << std::endl;
    }
    int result = nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
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
void MessageMux::registerMux(const std::string& name, const std::string& url, const bool verbose) {
  MuxMapConstIterator iter = multiplexers().find(name);
  if ( iter == multiplexers().end() ) {
    if (url.empty()) {
      // TODO : throw an exception
    } else {
      std::pair<MuxMapIterator,bool> result;
      result = multiplexers().insert(
          MuxMapPair(name, std::make_shared<impl::MessageMux>(name, url, verbose)));
    }
  } else if ( !url.empty() ) {
    // TODO : throw an exception, name-url already present.
  }
}

void MessageMux::unregisterMux(const std::string& name) {
  // TODO
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
    return SpecifiedMuxNotAvailable;
  }
}

} // namespace mm_mux_demux
