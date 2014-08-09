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
#include <string>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class MessageMux {
public:
  typedef std::map<std::string, std::string> MuxMap;
  typedef std::map<std::string, std::string>::iterator MuxMapIterator;
  typedef std::map<std::string, std::string>::const_iterator MuxMapConstIterator;

  enum {
    DanglingConnection = 0,
  };

  static void registerMux(const std::string& name, const std::string& url);
  static int createPublisher(const std::string& name, const std::string& url = "");
  static int createSubscriber(const std::string& name, const std::string& url = "");
  static MuxMap& multiplexers();
};

} // dslam

#endif /* dslam_message_runtime_MUX_HPP_ */
