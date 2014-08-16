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
#include "../../include/message_mux_demux/publisher.hpp"
#include "../../include/message_mux_demux/demux.hpp"
#include "../../include/message_mux_demux/subscriber.hpp"

/*****************************************************************************
** Namespaces
*****************************************************************************/

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ecl/command_line/xor_handler.hpp>
#include <ecl/command_line/cmd_line.hpp>
#include <ecl/command_line/value_arg.hpp>
#include <ecl/command_line/switch_arg.hpp>

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
  /****************************************
   ** Args
   ****************************************/
  ecl::CmdLine cmd("Captures and streams images over tcp.");
  ecl::SwitchArg pub("p", "pub", "Run the publisher only", false);
  ecl::SwitchArg sub("s", "sub", "Run the subscriber only", false);
  ecl::SwitchArg both("b", "both", "Run both the publisher and the subscriber", false);
  std::vector<ecl::Arg*> xorlist;
  xorlist.push_back(&pub);
  xorlist.push_back(&sub);
  xorlist.push_back(&both);
  cmd.xorAdd(xorlist);
  ecl::UnlabeledValueArg<std::string> ip("ip","Ip to use", false,"ipc:///tmp/pubsub.ipc","string", cmd);
  cmd.parse(argc, argv);

  if ( pub.isSet() ) {
    message_mux_demux::MessageMux::registerMux("dude", ip.getValue());
    std::cout << "Creating publisher" << std::endl;
    message_mux_demux::Publisher publisher("dude", "ipc:///tmp/pubsub.ipc");
    ecl::MilliSleep()(200); // let the connection establish itself
    while(true) {
      std::cout << "Publishing 'dude'" << std::endl;
      publisher.publish(1, std::string("dude"));
      ecl::MilliSleep()(500);
    }
  } else if ( sub.isSet() ) {
    message_mux_demux::MessageDemux::registerDemux("dude", ip.getValue());
    std::cout << "Creating demux"<< std::endl;
    message_mux_demux::Subscriber<std::string, 1> subscriber("dude", foo);
    while(true) {
      ecl::MilliSleep()(200);
    }
  } else if ( both.isSet() ) {
    message_mux_demux::MessageMux::registerMux("dude", ip.getValue());
    message_mux_demux::MessageDemux::registerDemux("dude", ip.getValue());
    message_mux_demux::Subscriber<std::string, 1> subscriber("dude", foo);
    message_mux_demux::Publisher publisher("dude", "ipc:///tmp/pubsub.ipc");
    ecl::MilliSleep()(200); // let the connection establish itself
    publisher.publish(1, std::string("dude"));
    ecl::MilliSleep()(500);
    message_mux_demux::MessageDemux::shutdown("dude");
  } else {
    // should never get here because tclap will throw up the usage page.
  }
  return 0;
}
