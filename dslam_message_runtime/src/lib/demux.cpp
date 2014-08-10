/**
 * @file /dslam_message_runtime/src/lib/demux.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include "../../include/dslam_message_runtime/demux.hpp"
#include "../../include/dslam_message_runtime/header.hpp"
#include "../../include/dslam_message_runtime/core_messages.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {
namespace impl {

// scope - only visible in this source file
//typedef dslam::MessageDemux Demux;
//typedef dslam::MessageDemux::DemuxMapConstIterator DemuxMapConstIterator;

/*****************************************************************************
** Implementation
*****************************************************************************/

MessageDemux::MessageDemux(const std::string& name, const std::string& url) :
    name(name),
    socket(0),
    thread() // defer start of the thread
{
  socket = nn_socket (AF_SP, NN_SUB);
  nn_setsockopt (socket, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
  nn_setsockopt (socket, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
  int result = nn_connect(socket, url.c_str());
  // TODO check the result and throw exceptions
  std::cout << "New Demux:"<< std::endl;
  std::cout << "  socket: " << socket << std::endl;
  std::cout << "  url: " << url << std::endl;
  thread = std::thread(&MessageDemux::spin, this);
}

MessageDemux::MessageDemux(const MessageDemux& other) {
  socket = other.socket;
  name = other.name;
  // this bugger forced us to write the copy constructor...get 'use of deleted function' otherwise
  std::move(other.thread);
}

MessageDemux::~MessageDemux()
{
  mutex.lock();
  for(auto& pair : subscribers) {
    delete pair.second;
  }
  subscribers.clear();
  mutex.unlock();

  if ( socket > 0 ) {
    nn_shutdown (socket, 0);
  }
  thread.join();
}

void MessageDemux::spin() {
  std::cout << "Demux spinning..."<< std::endl;
  while (1)
  {
    unsigned char *buffer = NULL;
    int bytes = nn_recv (socket, &buffer, NN_MSG, 0);
    // assert (bytes >= 0);
    std::cout << "Demux : [" << bytes << "] ";
    std::cout << std::hex;
    for(unsigned int i=0; i < bytes; ++i ) {
      std::cout << static_cast<unsigned int>(*(buffer+i)) << " ";
    }
    std::cout << std::dec;
    std::cout << std::endl;
    dslam::PacketHeader header = dslam::Message<dslam::PacketHeader>::decode(buffer, PacketHeader::size);
    mutex.lock();
    SubscriberMapIterator iter = subscribers.find(header.id());
    if (iter != subscribers.end()) {
      std::cout << "Demux : relay data"<< std::endl;
      (*(iter->second))(buffer + PacketHeader::size, bytes - PacketHeader::size);
    }
    mutex.unlock();
    nn_freemsg (buffer);
  }
}

void MessageDemux::unregisterSubscriber(const unsigned int& id)
{
  mutex.lock();
  subscribers.erase(id);
  mutex.unlock();
}

} // namespace impl
} // namespace dslam

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Globals
*****************************************************************************/

void MessageDemux::registerDemux(const std::string& name,
                                                 const std::string& url)
{
  DemuxMapConstIterator iter = demultiplexers().find(name);
  if ( iter == demultiplexers().end() ) {
    std::pair<DemuxMapIterator,bool> result;
    result = demultiplexers().insert(
        DemuxMapPair(name, std::make_shared<impl::MessageDemux>(name, url)));
  }
}

void MessageDemux::unregisterDemux(const std::string& name) {

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



} // namespace dslam
