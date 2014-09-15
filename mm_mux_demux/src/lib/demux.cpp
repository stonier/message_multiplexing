/**
 * @file /mm_mux_demux/src/lib/demux.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/formatters.hpp>
#include <iostream>
#include <mm_messages.hpp>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include "../../include/mm_mux_demux/demux.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {
namespace impl {

/*****************************************************************************
** Implementation
*****************************************************************************/

MessageDemux::MessageDemux(const std::string& name,
                           const std::string& url,
                           const mm_messages::Verbosity::Level& verbosity,
                           const bool bind
                          ) :
    name(name),
    url(url),
    socket(0),
    verbosity(verbosity),
    shutdown_requested(false),
    thread() // defer start of the thread
{
  socket = nn_socket (AF_SP, NN_SUB);
  if ( socket < 0 ) {
    // TODO : throw exception
    std::cout << "Demux socket error: " << nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
  }
  nn_setsockopt (socket, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
  nn_setsockopt (socket, NN_SOL_SOCKET, NN_SOCKET_NAME, name.c_str(), name.size());
  int timeout = 100; // timeout of 10ms (facilitates shutdown).
  nn_setsockopt (socket, NN_SOL_SOCKET, NN_RCVTIMEO, &timeout, sizeof(timeout));
  if ( bind ) {
    endpoint_id = nn_bind(socket, url.c_str());
  } else {
    endpoint_id = nn_connect(socket, url.c_str());
  }
  if (endpoint_id < 0) {
    // TODO : throw exception
    std::cout << "Demux connect error: " << nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
  }
  if (verbosity > mm_messages::Verbosity::QUIET) {

    std::cout << "[" << ecl::TimeStamp() << "] MessageDemux : [" << name << "][" << url << "][" << socket << "][" << endpoint_id << "]";
    if ( bind ) {
      std::cout << "[bind]" << std::endl;
    } else {
      std::cout << "[connect]" << std::endl;
    }
  }
  // std::thread call, need c++11
  // thread = std::thread(&MessageDemux::spin, this);
  // ecl::Thread call
  thread.start(&MessageDemux::spin, *this);
}

MessageDemux::MessageDemux(const MessageDemux& other) {
  socket = other.socket;
  name = other.name;
  verbosity = other.verbosity;
  shutdown_requested = other.shutdown_requested;
  endpoint_id = other.endpoint_id;
  // this bugger forced us to write the copy constructor...get 'use of deleted function' otherwise
  std::move(other.thread);
}

MessageDemux::~MessageDemux()
{
  mutex.lock();
  // c11 call
  // for(auto& pair : subscribers) {
  //     delete pair.second;
  // }
  for (SubscriberMapIterator iter = subscribers.begin(); iter != subscribers.end(); ++iter) {
    delete iter->second;
  }
  subscribers.clear();
  mutex.unlock();

  shutdown();
}

void MessageDemux::spin() {
  while (!shutdown_requested)
  {
    unsigned char *buffer = NULL;
    int bytes = ::nn_recv(socket, &buffer, NN_MSG, 0);
    if ( bytes < 0 ) {
      // We set socket options in the constructor to timeout as opposed to default infinite blocking
      if (nn_errno() == EAGAIN) {
        continue;
      }
      // TODO handle errors : http://nanomsg.org/v0.4/nn_recv.3.html
    }
    mm_messages::PacketHeader header = mm_messages::Message<mm_messages::PacketHeader>::decode(buffer, mm_messages::PacketHeader::size);
    mm_messages::SubPacketHeader subheader = mm_messages::Message<mm_messages::SubPacketHeader>::decode(buffer + mm_messages::PacketHeader::size, mm_messages::SubPacketHeader::size);
    if ( verbosity > mm_messages::Verbosity::QUIET ) {
      std::cout << "[" << ecl::TimeStamp() << "] Demux: [" << subheader.id << "]";
      std::cout << "[" << bytes << "][";
      if ( verbosity > mm_messages::Verbosity::LOW ) {
        std::cout << std::hex;
        for(unsigned int i=0; i < bytes; ++i ) {
          std::cout << static_cast<unsigned int>(*(buffer+i)) << " ";
        }
        std::cout << std::dec;
        std::cout << "]";
      }
      std::cout << std::endl;
    }
    mutex.lock();
    SubscriberMapIterator iter = subscribers.find(subheader.id);
    if (iter != subscribers.end()) {
      (*(iter->second))(buffer + mm_messages::PacketHeader::size + mm_messages::SubPacketHeader::size, bytes - mm_messages::PacketHeader::size - mm_messages::SubPacketHeader::size);
    }
    mutex.unlock();
    nn_freemsg (buffer);
  }
}
/**
 * Shutdown the socket. This currently doesn't shutdown the thread as
 * nn_recv will block indefinitely. Even after nn_shutdown is called, it will
 * hang around.
 */
void MessageDemux::shutdown() {
  if ( !shutdown_requested ) {
    shutdown_requested = true;
    thread.join();
  }
  if ( socket > 0 ) {
    int result = nn_shutdown (socket, endpoint_id);
  }
}

void MessageDemux::unregisterSubscriber(const unsigned int& id)
{
  mutex.lock();
  subscribers.erase(id);
  mutex.unlock();
}

} // namespace impl
} // mm_mux_demux

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_mux_demux {

/*****************************************************************************
** Globals
*****************************************************************************/

void MessageDemux::start(const std::string& name,
                         const std::string& url,
                         const mm_messages::Verbosity::Level& verbosity,
                         const bool bind
                        )
{
  DemuxMapConstIterator iter = demultiplexers().find(name);
  if ( iter == demultiplexers().end() ) {
    std::pair<DemuxMapIterator,bool> result;
    result = demultiplexers().insert(
        DemuxMapPair(name, std::make_shared<impl::MessageDemux>(name, url, verbosity, bind))
    );
  } else if ( !url.empty() ) {
    // TODO : throw an exception, name-url already present.
  }
}

void MessageDemux::shutdown() {
  DemuxMapIterator iter = demultiplexers().begin();
  for (iter; iter != demultiplexers().end(); ++iter) {
    (iter->second)->shutdown();
  }
  // TODO : delete from map
}

void MessageDemux::shutdown(const std::string& name) {
  DemuxMapConstIterator iter = demultiplexers().find(name);
  if ( iter != demultiplexers().end() ) {
    (iter->second)->shutdown();
  }
  // TODO : delete from map
}

MessageDemux::DemuxMap& MessageDemux::demultiplexers()
{
  static MessageDemux::DemuxMap map;  // string - demux storage in an invisible location
  return map;
}

void MessageDemux::unregisterSubscriber(const std::string& name, const unsigned int& id)
{
  DemuxMapIterator iter = demultiplexers().find(name);
  if ( iter != demultiplexers().end() ) {
    (iter->second)->unregisterSubscriber(id);
  } else {
    std::cout << "Demux : no demux by that name found (while unregistering subscriber)"<< std::endl;
  }
}



} // mm_mux_demux
