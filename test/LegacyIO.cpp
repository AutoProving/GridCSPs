// Copyright (c) 2019-2020 Vasily Alferov, Mateus de Oliveira Oliveira and Contributors. 
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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
