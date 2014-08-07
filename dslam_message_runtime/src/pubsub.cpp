/**
 * @file /dslam_message_runtime/src/pubsub.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/time.hpp>
#include <string>
#include "../include/dslam_message_runtime/publisher.hpp"
#include "../include/dslam_message_runtime/subscriber.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

/*****************************************************************************
** Implementation
*****************************************************************************/

int main(int argc, char **argv)
{
  dslam::MessageManager::registerTopic("dude", "ipc:///tmp/pubsub.ipc");
  if (argc > 1 && std::string(argv[1]) == "pub") {
    dslam::Publisher<std::string> publisher("dude", "ipc:///tmp/pubsub.ipc");
    publisher.publish(std::string("dude"));
    ecl::MilliSleep()(2000);
  } else {
    std::cout << "Creating subscriber"<< std::endl;
    dslam::Subscriber subscriber("dude", "ipc:///tmp/pubsub.ipc");
    subscriber.spin();

  }
  return 0;
}

