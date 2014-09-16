/**
 * @file /mm_radio/src/pubsub.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <gtest/gtest.h>
#include <iostream>
#include <ecl/time.hpp>
#include <mm_core_msgs/string.hpp>
#include "../../../include/mm_radio/radio.hpp"
#include "../../../include/mm_radio/publisher.hpp"
#include "../../../include/mm_radio/subscriber.hpp"

/*****************************************************************************
 ** Namespace
 *****************************************************************************/

using namespace mm_radio;

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

TEST(mm_radio, pubsub) {
  Foo old_timer_foo, old_man_foo;
  const std::string inproc_address = "inproc://radio";
  Radio::startServer("oldman", inproc_address);
  Radio::startClient("oldtimer", inproc_address);
  Subscriber<TestPubSub, std::string> oldtimer_subscriber("oldtimer", &Foo::foo_cb, old_timer_foo);
  Subscriber<TestPubSub, std::string> oldman_subscriber("oldman", &Foo::foo_cb, old_man_foo);
  Publisher oldtimer_publisher("oldtimer");
  Publisher oldman_publisher("oldman");
  ecl::MilliSleep()(200); // let the connection establish itself
  oldtimer_publisher.publish(TestPubSub, std::string("hey old man"));
  oldman_publisher.publish(TestPubSub, std::string("aye carumba"));
  ecl::MilliSleep()(500);
  EXPECT_EQ(std::string("hey old man"), old_man_foo.foo_msg);
  EXPECT_EQ(std::string("aye carumba"), old_timer_foo.foo_msg);
  Radio::shutdown();
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
