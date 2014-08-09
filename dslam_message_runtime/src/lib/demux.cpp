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
  if ( socket > 0 ) {
    nn_shutdown (socket, 0);
  }
  thread.join();
}

void MessageDemux::spin() {
  while (1)
  {
    char *buf = NULL;
    std::cout << "Receiving..."<< std::endl;
    int bytes = nn_recv (socket, &buf, NN_MSG, 0);
    // assert (bytes >= 0);
    std::cout << "CLIENT " << name << ": RECEIVED " << buf << "\n" << std::endl;
    nn_freemsg (buf);
  }
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
        DemuxMapPair(name,
                     std::make_shared<impl::MessageDemux>(name, url)));
  }
}

void MessageDemux::unregisterDemux(const std::string& name) {

}

MessageDemux::DemuxMap& MessageDemux::demultiplexers()
{
  static MessageDemux::DemuxMap map;  // string - demux storage in an invisible location
  return map;
}


} // namespace dslam
