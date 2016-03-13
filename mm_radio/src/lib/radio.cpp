/**
 * @file /mm_radio/src/lib/radio.cpp
 *
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/exceptions.hpp>
#include <mm_messages.hpp>
#include <nanomsg/nn.h>
#include <nanomsg/pair.h>
#include <string>
#include "../../include/mm_radio/radio.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_radio {
namespace impl {

/*****************************************************************************
** Implementation
*****************************************************************************/

Radio::Radio(const std::string& name,
             const std::string& url,
             const bool bind,
             const mm_messages::Verbosity::Level& verbosity
            ) :
  name(name),
  url(url),
  verbosity(verbosity),
  shutdown_requested(false)
{
  socket = nn_socket (AF_SP, NN_PAIR);
  if ( socket < 0 ) {
    // TODO : throw exception
    std::cout << "Radio socket error: " << nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
  }
  int result = nn_setsockopt (socket, NN_SOL_SOCKET, NN_SOCKET_NAME, name.c_str(), name.size());
  if ( result < 0 ) {
    // TODO : throw exception
    std::cout << "Radio nn_setsockopt error: " <<  nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
  }
  int send_timeout_ms = 5000;
  result = nn_setsockopt (socket, NN_SOL_SOCKET, NN_SNDTIMEO, &send_timeout_ms, sizeof(send_timeout_ms));
  if ( bind ) {
    endpoint_id = nn_bind(socket, url.c_str());
  } else {
    endpoint_id = nn_connect(socket, url.c_str());
  }
  if (endpoint_id < 0) {
    // TODO : throw exception
    if ( bind ) {
      std::cout << "Radio bind error: " << nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
    } else {
      std::cout << "Radio connect error: " << nn_strerror(errno) << " [" << nn_errno() << "][" << name << "][" << url << "]" << std::endl;
    }
  }
  // TODO : check the result, throw exceptions if necessary
  if (verbosity > mm_messages::Verbosity::QUIET) {

    std::cout << "[" << ecl::TimeStamp() << "] Radio : [" << name << "][" << url << "][" << socket << "][" << endpoint_id << "]";
    if ( bind ) {
      std::cout << "[bind]" << std::endl;
    } else {
      std::cout << "[connect]" << std::endl;
    }
  }
  // std::thread call, need c++11
  // thread = std::thread(&MessageDemux::spin, this);
  // ecl::Thread call
  thread.start(&Radio::spin, *this);
}

Radio::Radio(const Radio& other) {
  socket = other.socket;
  endpoint_id = other.endpoint_id;
  name = other.name;
  verbosity = other.verbosity;
  shutdown_requested = other.shutdown_requested;
  // this bugger forced us to write the copy constructor...get 'use of deleted function' otherwise
  std::move(other.thread);
}

Radio::~Radio() {
  if ( socket >= 0 ) {
    // only possible to have one connection to a pair at any one time
    // so don't worry about using nn_shutdown with endpoint ids.
    nn_close(socket);
  }
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
}

void Radio::spin() {
  // this is almost identical to the Demux spin (only Subscriber type is different)
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
      std::cout << "[" << ecl::TimeStamp() << "] RadioDemux: [" << subheader.id << "]";
      std::cout << "[" << bytes << "]";
      if ( verbosity > mm_messages::Verbosity::LOW ) {
        std::cout << "[" << std::hex;
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
void Radio::shutdown() {
  if ( socket > 0 ) {
    int result = close (socket);
  }
  if ( !shutdown_requested ) {
    shutdown_requested = true;
    thread.join();
  }
}

void Radio::unregisterSubscriber(const unsigned int& id)
{
  mutex.lock();
  subscribers.erase(id);
  mutex.unlock();
}

int Radio::send(const unsigned int& id, const mm_messages::ByteArray& msg_buffer) {
  // this is identical to MessageMux's send (could be collapsed)
  mm_messages::ByteArray buffer;
  mm_messages::Message<mm_messages::PacketHeader>::encode(mm_messages::PacketHeader(), buffer);
  mm_messages::Message<mm_messages::SubPacketHeader>::encode(mm_messages::SubPacketHeader(id, msg_buffer.size()), buffer);
  buffer.insert(buffer.end(), msg_buffer.begin(), msg_buffer.end());

  if (verbosity > mm_messages::Verbosity::LOW) {
    std::cout << "[" << ecl::TimeStamp() << "] RadioMux: [" << id << "][" << buffer.size() << "][";
    std::cout << std::hex;
    for(unsigned int i=0; i < buffer.size(); ++i ) {
      std::cout << static_cast<unsigned int>(buffer[i]) << " ";
    }
    std::cout << std::dec;
    std::cout << "]" << std::endl;
  }
  int result = ::nn_send(socket, buffer.data(), buffer.size(), 0); // last option is flags, only NN_DONTWAIT available
  if ( result == -1 ) {
    int error_number = nn_errno();
    // TODO : lots of error flags to check here
    if ( ( error_number == EAGAIN ) || ( error_number == ETIMEDOUT ) ) {
      //std::cout << "[" << ecl::TimeStamp() << "] RadioMux : timed out trying to send a message [" << name << "][" << url << "]" << std::endl;
      throw ecl::StandardException(LOC, ecl::TimeOutError, std::string("timed out trying to send a message [") + name + std::string("][") + url + std::string("]"));
    }
  }
  return 0;
}

} // namespace impl
} // namespace mm_radio

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace mm_radio {

/*****************************************************************************
** Global Statics
*****************************************************************************/

/**
 * @brief Pre-establish named connections for a server.
 * @param name
 * @param url
 */
void Radio::startServer(const std::string& name,
                          const std::string& url,
                          const mm_messages::Verbosity::Level& verbosity) {
  RadioMapConstIterator iter = radios().find(name);
  if ( iter == radios().end() ) {
    if (url.empty()) {
      // TODO : throw an exception
    } else {
      std::pair<RadioMapIterator,bool> result;
      result = radios().insert(
          RadioMapPair(name, std::make_shared<impl::Radio>(name, url, true, verbosity)));
    }
  } else if ( !url.empty() ) {
    // TODO : throw an exception, name-url already present.
  }
}

/**
 * @brief Pre-establish named connections for a client
 * @param name
 * @param url
 */
void Radio::startClient(const std::string& name,
                          const std::string& url,
                          const mm_messages::Verbosity::Level& verbosity) {
  RadioMapConstIterator iter = radios().find(name);
  if ( iter == radios().end() ) {
    if (url.empty()) {
      std::cout << "mm::Radio::startClient : url is empty" << std::endl;
      // TODO : throw an exception
    } else {
      std::pair<RadioMapIterator,bool> result;
      result = radios().insert(
          RadioMapPair(name, std::make_shared<impl::Radio>(name, url, false, verbosity)));
    }
  } else if ( !url.empty() ) {
    std::cout << "mm::Radio::startClient : already radio in the map [" << name << "][" << url << "]" << std::endl;
    // TODO : throw an exception, name-url already present.
  }
}

Radio::RadioMap& Radio::radios() {
  static Radio::RadioMap map;
  return map;
}

int Radio::send(const std::string& name, const unsigned int& id, const mm_messages::ByteArray& msg_buffer) {
  RadioMapIterator iter = radios().find(name);
  if ( iter != radios().end() ) {
    return (iter->second)->send(id, msg_buffer);
  } else {
    // exceptions exceptions...
    std::cout << "Radio : no mux by that name found (while trying to send)"<< std::endl;
    return NotAvailable;
  }
}

void Radio::shutdown(const std::string& name) {
  radios().erase(name);
}

void Radio::shutdown() {
  radios().clear();
}

void Radio::unregisterSubscriber(const std::string& name, const unsigned int& id)
{
  RadioMapIterator iter = radios().find(name);
  if ( iter != radios().end() ) {
    (iter->second)->unregisterSubscriber(id);
  } else {
    // quietly pass - this will only occur if a subscriber self destructs after the radio is shut down
  }
}

} // namespace mm_radio
