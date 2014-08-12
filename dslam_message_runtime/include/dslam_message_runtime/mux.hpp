/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/mux.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_MUX_HPP_
#define dslam_message_runtime_MUX_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <map>
#include <memory>
#include <string>
#include "messages.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {
namespace impl {

/*****************************************************************************
 ** Interfaces
 *****************************************************************************/

class MessageMux {
public:
  MessageMux(const std::string& name, const std::string& url);
  ~MessageMux();
  int send(const unsigned int& id, const ByteArray& msg_buffer);

private:
  std::string name, url;
  int socket;
};

} // namespace impl
} // namespace dslam

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class MessageMux {
public:
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>> MuxMap;
  typedef std::pair<std::string, std::shared_ptr<impl::MessageMux>> MuxMapPair;
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>>::iterator MuxMapIterator;
  typedef std::map<std::string, std::shared_ptr<impl::MessageMux>>::const_iterator MuxMapConstIterator;

  enum MessageMuxErrors {
    SpecifiedMuxNotAvailable = -1  // when you try to connect to <name>, but <name> has not yet been started
  };
  static void registerMux(const std::string& name, const std::string& url);
  static void unregisterMux(const std::string& name);
  static MuxMap& multiplexers();
  static int send(const std::string& name, const unsigned int& id, const ByteArray& msg_buffer);
//  static int createPublisher(const std::string& name, const std::string& url = "");
};

} // dslam

#endif /* dslam_message_runtime_MUX_HPP_ */
