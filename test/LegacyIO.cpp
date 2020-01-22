#include <gtest/gtest.h>

#include <ListColoring/ListColoring.h>
#include <ListColoring/LegacyReader.h>
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

TEST(LegacyIOTest, readerStringExampleNoComments) {
    auto expected = TestInstances::sample();
    std::string test = TestInstances::getResource("exampleNoComments");
    auto actual = ListColoring::Legacy::read(test);
    EXPECT_EQ(ListColoring::Legacy::write(expected), ListColoring::Legacy::write(actual));
}

TEST(LegacyIOTest, readerStreamExampleNoComments) {
    auto expected = TestInstances::sample();
    std::string test = TestInstances::getResource("exampleNoComments");
    std::istringstream ss(test);
    auto actual = ListColoring::Legacy::read(ss);
    EXPECT_EQ(ListColoring::Legacy::write(expected), ListColoring::Legacy::write(actual));
}

TEST(LegacyIOTest, readerStringExample) {
    auto expected = TestInstances::sample();
    std::string test = TestInstances::getResource("example");
    auto actual = ListColoring::Legacy::read(test);
    EXPECT_EQ(ListColoring::Legacy::write(expected), ListColoring::Legacy::write(actual));
}

TEST(LegacyIOTest, readerStreamExample) {
    auto expected = TestInstances::sample();
    std::string test = TestInstances::getResource("example");
    std::istringstream ss(test);
    auto actual = ListColoring::Legacy::read(ss);
    EXPECT_EQ(ListColoring::Legacy::write(expected), ListColoring::Legacy::write(actual));
}

TEST(LegacyIOTest, readerEmptyThrows) {
    std::string test;
    EXPECT_THROW(ListColoring::Legacy::read(test),
                 ListColoring::Legacy::ReaderError);
}

TEST(LegacyIOTest, readerNotClosedCommentThrows) {
    std::string test = "/";
    EXPECT_THROW(ListColoring::Legacy::read(test),
                 ListColoring::Legacy::ReaderError);
}
