/**
 * @file /message_mux_demux/src/pubsub.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/time.hpp>
#include <string>
#include "../include/message_mux_demux/publisher.hpp"
#include "../include/message_mux_demux/demux.hpp"
#include "../include/message_mux_demux/subscriber.hpp"

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
    message_multiplexing::MessageMux::registerMux("dude", "ipc:///tmp/pubsub.ipc");
    std::cout << "Creating publisher" << std::endl;
    message_multiplexing::Publisher publisher("dude", "ipc:///tmp/pubsub.ipc");
    ecl::MilliSleep()(200); // let the connection establish itself
    while(true) {
      std::cout << "Publishing 'dude'" << std::endl;
      publisher.publish(1, std::string("dude"));
      ecl::MilliSleep()(500);
    }
  } else if (argc > 1 && std::string(argv[1]) == "sub") {
    message_multiplexing::MessageDemux::registerDemux("dude", "ipc:///tmp/pubsub.ipc");
    std::cout << "Creating demux"<< std::endl;
    message_multiplexing::Subscriber<std::string, 1> subscriber("dude", foo);
    while(true) {
      ecl::MilliSleep()(200);
    }
  } else {
    std::cout << "Usage: pubsub pub||sub" << std::endl;
  }
  return 0;
}

