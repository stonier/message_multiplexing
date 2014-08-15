/**
 * @file /message_mux_demux/src/pubsub.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <gtest/gtest.h>
#include <iostream>
#include <ecl/time.hpp>
#include "../../include/message_mux_demux/mux.hpp"
#include "../../include/message_mux_demux/demux.hpp"
#include "../../include/message_mux_demux/publisher.hpp"
#include "../../include/message_mux_demux/subscriber.hpp"

/*****************************************************************************
 ** Namespace
 *****************************************************************************/

using namespace message_mux_demux;

/*****************************************************************************
** Registrations
*****************************************************************************/

// We use 1000's by convention for test packet id's
enum {
  TestPubSub = 1001,
};

MESSAGE_MULTIPLEXING_REGISTER_PACKET_INFO(TestPubSub, std::string, "Foo");

struct Foo {
  void foo_cb(const std::string& msg) {
    foo_msg = msg;
  }
  std::string foo_msg;
};
/*****************************************************************************
** Tests
*****************************************************************************/

TEST(MessageMuxDemux,pubsub) {
  Foo foo;
  message_mux_demux::MessageMux::registerMux("dude", "ipc:///tmp/pubsub.ipc");
  message_mux_demux::MessageDemux::registerDemux("dude", "ipc:///tmp/pubsub.ipc");
  message_mux_demux::Subscriber<std::string, TestPubSub> subscriber("dude", &Foo::foo_cb, foo);
  message_mux_demux::Publisher publisher("dude", "ipc:///tmp/pubsub.ipc");
  ecl::MilliSleep()(200); // let the connection establish itself
  publisher.publish(TestPubSub, std::string("dude"));
  ecl::MilliSleep()(500);
  EXPECT_EQ(std::string("dude"), foo.foo_msg);
  message_mux_demux::MessageDemux::shutdown("dude");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
