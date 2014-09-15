/**
 * @file /mm_mux_demux/src/mm_radio.cpp
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
#include <mm_messages.hpp>
#include <string>
#include "../../include/mm_mux_demux/publisher.hpp"
#include "../../include/mm_mux_demux/radio.hpp"
#include "../../include/mm_mux_demux/subscriber.hpp"

/*****************************************************************************
** Methods
*****************************************************************************/

void oldtimer_foo(std::string data) {
  std::cout << "oldtimer_foo : processing data: " << data << std::endl;
}

void oldman_foo(std::string data) {
  std::cout << "oldman_foo : processing data: " << data << std::endl;
}

/*****************************************************************************
** Main
*****************************************************************************/

int main(int argc, char **argv)
{
  /****************************************
   ** Args
   ****************************************/
  ecl::CmdLine cmd("Radio multiplexing, by default tests both sides on inproc, use the options to choose otherwise.");
  ecl::SwitchArg oldman("m", "oldman", "Run the publisher only", false);
  ecl::SwitchArg oldtimer("t", "oldtimer", "Run the subscriber only", false);
  ecl::SwitchArg both("b", "both", "Run both the oldman and oldtimer inproc (instead of iproc)", false);
  std::vector<ecl::Arg*> xorlist;
  xorlist.push_back(&oldman);
  xorlist.push_back(&oldtimer);
  xorlist.push_back(&both);
  cmd.xorAdd(xorlist);
  cmd.parse(argc, argv);

  const std::string inproc_address = "inproc://radio";
  const std::string ipc_address = "ipc:///tmp/radio.ipc";
  //bool result = mm_messages::MessageRegistry::add<mm_core_msgs::TestStrings, std::string>("mm_radio teststrings packet");
  if ( oldman.isSet() ) {
    mm_mux_demux::RadioMuxDemux::startClient("oldman", ipc_address, mm_messages::Verbosity::HIGH);
//    mm_mux_demux::MessageDemux::start("oldman", ipc_address, mm_messages::Verbosity::HIGH, false);
//    mm_mux_demux::MessageMux::start("oldman", ipc_address, mm_messages::Verbosity::HIGH, false); // bind on this one
    mm_mux_demux::Subscriber<mm_core_msgs::TestStrings, std::string> oldman_subscriber("oldman", oldman_foo);
    mm_mux_demux::Publisher oldman_publisher("oldman");
    while(true) {
      std::cout << "Oldman publishing : 'aye curamba'" << std::endl;
      oldman_publisher.publish(mm_core_msgs::TestStrings, std::string("aye carumba"));
      ecl::MilliSleep()(500);
    }
  } else if ( oldtimer.isSet() ) {
    mm_mux_demux::RadioMuxDemux::startServer("oldtimer", ipc_address, mm_messages::Verbosity::HIGH);
//    mm_mux_demux::MessageMux::start("oldtimer", ipc_address, mm_messages::Verbosity::HIGH, true);//    mm_mux_demux::Subscriber<mm_core_msgs::TestStrings, std::string> oldtimer_subscriber("oldtimer", oldtimer_foo);
    mm_mux_demux::Publisher oldtimer_publisher("oldtimer");
    mm_mux_demux::Subscriber<mm_core_msgs::TestStrings, std::string> oldtimer_subscriber("oldtimer", oldtimer_foo);
    while(true) {
      std::cout << "Oldtimer publishing : 'hello old man'" << std::endl;
      oldtimer_publisher.publish(mm_core_msgs::TestStrings, std::string("hello old man"));
      ecl::MilliSleep()(500);
    }
  } else if ( both.isSet() ) {
    mm_mux_demux::RadioMuxDemux::startServer("oldman", inproc_address);
    mm_mux_demux::RadioMuxDemux::startClient("oldtimer", inproc_address);
    mm_mux_demux::Subscriber<mm_core_msgs::TestStrings, std::string> oldtimer_subscriber("oldtimer", oldtimer_foo);
    mm_mux_demux::Subscriber<mm_core_msgs::TestStrings, std::string> oldman_subscriber("oldman", oldman_foo);
    mm_mux_demux::Publisher oldtimer_publisher("oldtimer");
    mm_mux_demux::Publisher oldman_publisher("oldman");
    ecl::MilliSleep()(200); // let the connection establish itself
    while(true) {
      oldtimer_publisher.publish(mm_core_msgs::TestStrings, std::string("hello old man"));
      ecl::MilliSleep()(500);
      oldman_publisher.publish(mm_core_msgs::TestStrings, std::string("aye carumba"));
      ecl::MilliSleep()(500);
    }
    mm_mux_demux::RadioMuxDemux::shutdown(); // shutdown all named radios
//  } else {
//    // should never get here because tclap will throw up the usage page.
  }
  return 0;
}
