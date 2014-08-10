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
#include "../include/dslam_message_runtime/demux.hpp"
#include "../include/dslam_message_runtime/subscriber.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

/*****************************************************************************
** Methods
*****************************************************************************/

void foo(const std::string& data) {
  std::cout << "foo is processing data: " << data << std::endl;
}
/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char **argv)
{
  if (argc > 1 && std::string(argv[1]) == "pub") {
    dslam::MessageMux::registerMux("dude", "ipc:///tmp/pubsub.ipc");
    std::cout << "Creating publisher" << std::endl;
    dslam::Publisher<std::string> publisher("dude", "ipc:///tmp/pubsub.ipc");
    ecl::MilliSleep()(200); // let the connection establish itself
    while(true) {
      std::cout << "Publishing 'dude'" << std::endl;
      publisher.publish(std::string("dude"));
      ecl::MilliSleep()(500);
    }
  } else if (argc > 1 && std::string(argv[1]) == "sub") {
    dslam::MessageDemux::registerDemux("dude", "ipc:///tmp/pubsub.ipc");
    std::cout << "Creating demux"<< std::endl;
    dslam::Subscriber<std::string, 1> subscriber("dude", foo);
    while(true) {
      ecl::MilliSleep()(200);
    }
//    dslam::Subscriber subscriber("dude", "ipc:///tmp/pubsub.ipc");
//    subscriber.spin();
//  } else if (argc > 1 && std::string(argv[1]) == "pubsub") {
//    std::cout << "Creating publisher" << std::endl;
//    dslam::Publisher<std::string> publisher("dude");
//    std::cout << "Creating subscriber"<< std::endl;
//    dslam::Subscriber subscriber("dude");
//    ecl::MilliSleep()(200); // let the connection establish itself
//    std::cout << "Publishing" << std::endl;
//    publisher.publish(std::string("dude"));
//    std::cout << "Receiving" << std::endl;
//    subscriber.spinOnce();
  } else {
    std::cout << "Usage: pubsub pub||sub||pubsub" << std::endl;
  }
  return 0;
}

