#include <gtest/gtest.h>

#include <ListColoring/ListColoring.h>
#include <ListColoring/LegacyWriter.h>

#include <Instances/Instances.h>

#include <sstream>

TEST(LegacyIOTest, writerStringSample) {
    auto instance = TestInstances::sample();
    std::string expected = TestInstances::getResource("exampleNoComments");
    std::string actual = ListColoring::Legacy::write(instance);
    EXPECT_EQ(expected, actual);
}

TEST(LegacyIOTest, writerStreamSample) {
    auto instance = TestInstances::sample();
    std::string expected = TestInstances::getResource("exampleNoComments");
    std::ostringstream ss;
    ListColoring::Legacy::write(ss, instance);
    EXPECT_EQ(expected, ss.str());
}
