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
  typedef std::map<std::string, std::string> name_map;
  typedef std::map<std::string, std::string>::iterator name_map_iterator;
  typedef std::map<std::string, std::string>::const_iterator name_map_const_iterator;

  enum {
    DanglingConnection = 0,
  };

  static void registerMultiplexer(const std::string& name, const std::string& url);
  static int createPublisher(const std::string& name, const std::string& url = "");
  static int createSubscriber(const std::string& name, const std::string& url = "");
  static name_map& topics();
};

} // dslam

#endif /* dslam_message_runtime_MUX_HPP_ */
