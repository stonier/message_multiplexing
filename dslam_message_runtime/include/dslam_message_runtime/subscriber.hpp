/**
 * @file /dslam_message_runtime/include/dslam_message_runtime/subscriber.hpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef dslam_message_runtime_SUBSCRIBER_HPP_
#define dslam_message_runtime_SUBSCRIBER_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/

#include <string>

/*****************************************************************************
** Namespaces
*****************************************************************************/

namespace dslam {

/*****************************************************************************
** Interfaces
*****************************************************************************/

class Subscriber {
public:
  Subscriber(const std::string &name, const std::string &url = "");
  ~Subscriber();

  void spin();
  void spinOnce();
private:
  std::string name;
  int socket;
};

} // dslam

#endif /* dslam_message_runtime_SUBSCRIBER_HPP_ */
