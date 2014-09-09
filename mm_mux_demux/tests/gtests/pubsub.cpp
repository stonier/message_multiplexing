/**
 * @file /mm_mux_demux/src/pubsub.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <gtest/gtest.h>
#include <iostream>
#include <ecl/time.hpp>
#include "../../include/mm_mux_demux/mux.hpp"
#include "../../include/mm_mux_demux/demux.hpp"
#include "../../include/mm_mux_demux/publisher.hpp"
#include "../../include/mm_mux_demux/subscriber.hpp"
#include <mm_core_msgs/string.hpp>

/*****************************************************************************
 ** Namespace
 *****************************************************************************/

using namespace mm_mux_demux;

/*****************************************************************************
** Registrations
*****************************************************************************/

// We use 1000's by convention for test packet id's
enum {
  TestPubSub = 9990,
};

MM_REGISTER_PACKET_INFO(TestPubSub, std::string, "Foo");

struct Foo {
  void foo_cb(std::string msg) {
    foo_msg = msg;
  }
  std::string foo_msg;
};
/*****************************************************************************
** Tests
*****************************************************************************/

TEST(MessageMuxDemux,pubsub) {
  Foo foo;
  mm_mux_demux::MessageMux::registerMux("dude", "ipc:///tmp/pubsub.ipc");
  mm_mux_demux::MessageDemux::registerDemux("dude", "ipc:///tmp/pubsub.ipc");
  mm_mux_demux::Subscriber<TestPubSub, std::string> subscriber("dude", &Foo::foo_cb, foo);
  mm_mux_demux::Publisher publisher("dude", "ipc:///tmp/pubsub.ipc");
  ecl::MilliSleep()(200); // let the connection establish itself
  publisher.publish(TestPubSub, std::string("dude"));
  ecl::MilliSleep()(500);
  EXPECT_EQ(std::string("dude"), foo.foo_msg);
  mm_mux_demux::MessageDemux::shutdown("dude");
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
