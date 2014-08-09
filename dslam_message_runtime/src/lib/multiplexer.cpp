/**
 * @file /dslam_message_runtime/src/lib/manager.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <iostream>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include "../../include/dslam_message_runtime/multiplexer.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Implementation
*****************************************************************************/
/**
 * @brief Pre-establish named connections.
 * @param name
 * @param url
 */
void MessageMultiplexer::registerTopic(const std::string& name, const std::string& url) {
  name_map_const_iterator iter = topics().find(name);
  if ( iter == topics().end() ) {
    std::pair<name_map_iterator,bool> result;
    result = topics().insert(std::pair<std::string, std::string>(name, url));
  }
}
/**
 * @brief Create a named publisher.
 *
 * As a convenience, you can optionally establish the url for the publisher here
 * to save from having to call `registerTopic` in advance.
 * @param name
 * @param url
 * @return
 */
int MessageMultiplexer::createPublisher(const std::string& name, const std::string& url) {
  std::cout << "Creating publisher" << std::endl;
  if ( !url.empty()) {
    registerTopic(name, url);
  }
  name_map_const_iterator iter = topics().find(name);
  if ( iter != topics().end() ) {
    int sock = nn_socket (AF_SP, NN_PUB);
    std::cout << "Socket: " << sock << std::endl;
    // TODO check >= 0, what does 0 mean?
    nn_setsockopt (sock, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
    std::string url = iter->second;
    std::cout << "URL: " << url << std::endl;
    int result = nn_bind(sock, url.c_str());
    // TODO check the result
    return sock;
  } else {
    return MessageMultiplexer::DanglingConnection;
  }
}

/**
 * @brief Create a named subscriber.
 *
 * As a convenience, you can optionally establish the url for the publisher here
 * to save from having to call `registerTopic` in advance.
 * @param name
 * @param url
 * @return
 */
int MessageMultiplexer::createSubscriber(const std::string& name, const std::string& url) {
  if ( !url.empty()) {
    registerTopic(name, url);
  }
  name_map_const_iterator iter = topics().find(name);
  if ( iter != topics().end() ) {
    int sock = nn_socket (AF_SP, NN_SUB);
    std::cout << "Socket: " << sock << std::endl;
    // TODO check >= 0, what does 0 mean?
    nn_setsockopt (sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0);
    nn_setsockopt (sock, NN_SUB, NN_SOCKET_NAME, name.c_str(), name.size());
    std::string url = iter->second;
    std::cout << "URL: " << url << std::endl;
    int result = nn_connect(sock, url.c_str());
    // TODO check the result
    return sock;
  } else {
    return MessageMultiplexer::DanglingConnection;
  }
}


MessageMultiplexer::name_map& MessageMultiplexer::topics() {
  static MessageMultiplexer::name_map topic_relations;  // human consumable string - url relations
  return topic_relations;
}

} // dslam
