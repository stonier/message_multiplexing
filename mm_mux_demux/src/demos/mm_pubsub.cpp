/**
 * @file /mm_mux_demux/src/mm_pubsub.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <ecl/command_line/xor_handler.hpp>
#include <ecl/command_line/cmd_line.hpp>
#include <ecl/command_line/value_arg.hpp>
#include <ecl/command_line/switch_arg.hpp>
#include <ecl/time.hpp>
#include <mm_core_msgs/test_identifiers.hpp>
#include <mm_core_msgs/string.hpp>
#include <string>
#include "../../include/mm_mux_demux/publisher.hpp"
#include "../../include/mm_mux_demux/demux.hpp"
#include "../../include/mm_mux_demux/subscriber.hpp"

/*****************************************************************************
** Methods
*****************************************************************************/

void foo(std::string data) {
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
  ecl::CmdLine cmd("Pub-sub testing with nanomsg.");
  ecl::SwitchArg pub("p", "pub", "Run the publisher only", false);
  ecl::SwitchArg sub("s", "sub", "Run the subscriber only", false);
  ecl::SwitchArg both("b", "both", "Run both the publisher and the subscriber", false);
  std::vector<ecl::Arg*> xorlist;
  xorlist.push_back(&pub);
  xorlist.push_back(&sub);
  xorlist.push_back(&both);
  cmd.xorAdd(xorlist);
  ecl::UnlabeledValueArg<std::string> ip("ip","Ip to use (e.g. ipc:///tmp/pubsub.ipc, tcp://192.168.1.3:5555)", false,"ipc:///tmp/pubsub.ipc","string", cmd);
  cmd.parse(argc, argv);

  //bool result = mm_messages::MessageRegistry::add<mm_core_msgs::TestStrings, std::string>("unused_description");
  if ( pub.isSet() ) {
    mm_mux_demux::MessageMux::start("dude", ip.getValue());
    std::cout << "Creating publisher" << std::endl;
    mm_mux_demux::Publisher publisher("dude");
    ecl::MilliSleep()(200); // let the connection establish itself
    while(true) {
      std::cout << "Publishing 'dude'" << std::endl;
      publisher.publish(mm_core_msgs::TestStrings, std::string("dude"));
      ecl::MilliSleep()(500);
    }
  } else if ( sub.isSet() ) {
    mm_mux_demux::MessageDemux::start("dude", ip.getValue());
    std::cout << "Creating demux"<< std::endl;
    mm_mux_demux::Subscriber<mm_core_msgs::TestStrings, std::string> subscriber("dude", foo);
    while(true) {
      ecl::MilliSleep()(200);
    }
    mm_mux_demux::MessageDemux::shutdown("dude");
  } else if ( both.isSet() ) {
    mm_mux_demux::MessageMux::start("dude", ip.getValue(), mm_messages::Verbosity::HIGH);
    mm_mux_demux::MessageDemux::start("dude", ip.getValue(), mm_messages::Verbosity::HIGH);
    mm_mux_demux::Subscriber<mm_core_msgs::TestStrings, std::string> subscriber("dude", foo);
    mm_mux_demux::Publisher publisher("dude");
    ecl::MilliSleep()(200); // let the connection establish itself
    publisher.publish(mm_core_msgs::TestStrings, std::string("dude"));
    ecl::MilliSleep()(500);
    mm_mux_demux::MessageDemux::shutdown("dude");
  } else {
    // should never get here because tclap will throw up the usage page.
  }
  return 0;
}
