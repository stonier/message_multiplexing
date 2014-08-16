/**
 * @file /mm_mux_demux/src/registry.cpp
 * 
 * @brief Short description of this file.
 **/
/*****************************************************************************
** Includes
*****************************************************************************/

#include <gtest/gtest.h>
#include <iostream>
#include "../../include/mm_mux_demux/registry.hpp"

/*****************************************************************************
 ** Namespace
 *****************************************************************************/

using namespace mm_mux_demux;

/*****************************************************************************
** Registrations
*****************************************************************************/

enum {
  Description,
  UnMatchedBeanCount,
  MatchedBeanCount,
  Reserved
};

MM_REGISTER_PACKET_INFO(Description, std::string, "Description of the current state of the system.");
MM_REGISTER_PACKET_INFO(UnMatchedBeanCount, int, "Number of beans that went unmatched");
//MM_REGISTER_PACKET_INFO(1, int);  // creates a compile time error

/*****************************************************************************
** Tests
*****************************************************************************/

TEST(MessageMuxDemux,registryAdd) {
  int result = MessageRegistry::add<MatchedBeanCount, int>("unused_description");
  EXPECT_TRUE(result);
  result = MessageRegistry::add<UnMatchedBeanCount, unsigned int>();
  EXPECT_FALSE(result);
  result = MessageRegistry::add<UnMatchedBeanCount, std::string>();
  EXPECT_FALSE(result);
}

TEST(MessageMuxDemux,isRegistered) {
  EXPECT_TRUE(MessageRegistry::isRegistered(UnMatchedBeanCount));
  EXPECT_FALSE(MessageRegistry::isRegistered(Reserved));
}

TEST(MessageMuxDemux,isRegisteredWithType) {
  EXPECT_TRUE(MessageRegistry::isRegisteredWithType<int>(UnMatchedBeanCount));
  EXPECT_FALSE(MessageRegistry::isRegisteredWithType<std::string>(UnMatchedBeanCount));
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
