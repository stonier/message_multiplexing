/**
 * @file /dslam_message_runtime/src/lib/mux.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include "../../include/dslam_message_runtime/mux.hpp"
#include "../../include/dslam_message_runtime/header.hpp"
#include "../../include/dslam_message_runtime/messages.hpp"
#include "../../include/dslam_message_runtime/messages/core.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {
namespace impl {

/*****************************************************************************
 ** Implementation
 *****************************************************************************/

MessageMux::MessageMux(const std::string& name, const std::string& url) :
  name(name),
  url(url)
{
  socket = nn_socket (AF_SP, NN_PUB);
  std::cout << "Socket: " << socket << std::endl;
  // TODO check >= 0, what does 0 mean?
  nn_setsockopt (socket, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
  std::cout << "URL: " << url << std::endl;
  int unused_result = nn_bind(socket, url.c_str());
  // TODO check the result, throw exceptions if necessary
}

MessageMux::~MessageMux() {
  if ( socket > 0 ) {
    nn_shutdown (socket, 0);
  }
}

int MessageMux::send(const unsigned int& id, const ByteArray& msg_buffer) {
    ByteArray buffer;
    unsigned int length = msg_buffer.size();
    std::cout << "Msg Buffer length: " << length<< std::endl;
    PacketHeader header(id, length);
    Message<PacketHeader>::encode(header, buffer);
    buffer.insert(buffer.end(), msg_buffer.begin(), msg_buffer.end());
    std::cout << "Sending: " << buffer.size() << std::endl;
    // const char* data = "dude";
    // int result = nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
    int result = nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
    std::cout << "Publishing result: " << result << std::endl;

    // lots of error flags to check here
    return 0;
}

} // namespace impl
} // namespace dslam


/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Global Statics
*****************************************************************************/
/**
 * @brief Pre-establish named connections.
 * @param name
 * @param url
 */
void MessageMux::registerMux(const std::string& name, const std::string& url) {
  MuxMapConstIterator iter = multiplexers().find(name);
  if ( iter == multiplexers().end() ) {
    if (url.empty()) {
      // TODO : throw an exception
    } else {
      std::pair<MuxMapIterator,bool> result;
      result = multiplexers().insert(
          MuxMapPair(name, std::make_shared<impl::MessageMux>(name, url)));
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

int MessageMux::send(const std::string& name, const unsigned int& id, const ByteArray& msg_buffer) {
  MuxMapIterator iter = multiplexers().find(name);
  if ( iter != multiplexers().end() ) {
    return (iter->second)->send(id, msg_buffer);
  } else {
    // exceptions exceptions...
    std::cout << "Mux : no mux by that name found (while trying to send)"<< std::endl;
    return SpecifiedMuxNotAvailable;
  }
}

//void MessageMux::registerPublisher() {
//
//}
///**
// * @brief Create a publisher for a specified multiplexer.
// *
// * As a convenience, you can optionally initialise a multiplexer for the specified url
// * at this name if it doesn't already exist. This saves from having to call
// * `registerMux` in advance.
// *
// * Note, the guts of this is just creating a socket at the mutliplexer url
// * which can do nanomsg style publishing.
// *
// * @param name
// * @param url
// * @return
// */
//void MessageMux::createPublisher(const std::string& name, const std::string& url) {
//  std::cout << "Creating publisher" << std::endl;
//  if ( !url.empty()) {
//    registerMux(name, url);
//  }
//  MuxMapConstIterator iter = multiplexers().find(name);
//  if ( iter != multiplexers().end() ) {
//    int sock = nn_socket (AF_SP, NN_PUB);
//    std::cout << "Socket: " << sock << std::endl;
//    // TODO check >= 0, what does 0 mean?
//    nn_setsockopt (sock, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
//    std::string url = iter->second;
//    std::cout << "URL: " << url << std::endl;
//    int result = nn_bind(sock, url.c_str());
//    // TODO check the result
//    return sock;
//  } else {
//    return MessageMux::SpecifiedMuxNotAvailable;
//  }
//}

} // dslam
