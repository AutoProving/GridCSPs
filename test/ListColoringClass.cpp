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

TEST(ListColoringClassTest, heightWidth) {
    ListColoring::ProblemInstance instance(3, 5);
    EXPECT_EQ(3, instance.height());
    EXPECT_EQ(5, instance.width());
}

TEST(ListColoringClassTest, intermediateColors) {
    ODDs::ODD::AlphabetMap alphabet;
    alphabet.addSymbol("b");
    alphabet.addSymbol("a");
    ListColoring::ProblemInstance instance(3, 5);
    instance.intermediateColors(2, 3) = alphabet;
    {
        std::vector<std::string> expected = {"b", "a"};
        EXPECT_EQ(expected, instance.intermediateColors(2, 3).symbols());
    }
    {
        std::vector<std::string> expected;
        const auto& actual = instance.intermediateColors(0, 0);
        EXPECT_EQ(expected, actual.symbols());
    }
    {
        std::vector<std::string> expected;
        const auto& actual = instance.intermediateColors(2, 4);
        EXPECT_EQ(expected, actual.symbols());
    }
}

TEST(ListColoringClassTest, finalColors) {
    ODDs::ODD::AlphabetMap alphabet;
    alphabet.addSymbol("b");
    alphabet.addSymbol("a");
    ListColoring::ProblemInstance instance(3, 5);
    instance.finalColors(2, 3) = alphabet;
    {
        std::vector<std::string> expected = {"b", "a"};
        EXPECT_EQ(expected, instance.finalColors(2, 3).symbols());
    }
    {
        std::vector<std::string> expected;
        const auto& actual = instance.finalColors(0, 0);
        EXPECT_EQ(expected, actual.symbols());
    }
    {
        std::vector<std::string> expected;
        const auto& actual = instance.finalColors(2, 4);
        EXPECT_EQ(expected, actual.symbols());
    }
}

TEST(ListColoringClassTest, colorMap) {
    ListColoring::ColorMap map = {1, 1, 0};
    ListColoring::ProblemInstance instance(3, 5);
    instance.colorMap(2, 3) = map;
    EXPECT_EQ(map, instance.colorMap(2, 3));
    {
        std::vector<ListColoring::Color> expected;
        const auto& actual = instance.colorMap(0, 0);
        EXPECT_EQ(expected, actual);
    }
    {
        std::vector<ListColoring::Color> expected;
        const auto& actual = instance.colorMap(2, 4);
        EXPECT_EQ(expected, actual);
    }
}

TEST(ListColoringClassTest, verticalConstraint) {
    ListColoring::ConstraintContainer constraintEq = {
        {0, 0},
        {1, 1}
    };
    ListColoring::ConstraintContainer constraintNotEq = {
        {0, 1},
        {1, 0}
    };
    ListColoring::ProblemInstance instance(3, 5);
    instance.verticalConstraints(1, 0) = constraintEq;
    instance.verticalConstraints(0, 4) = constraintNotEq;
    instance.verticalConstraints(1, 4) = constraintEq;
    EXPECT_EQ(constraintEq, instance.verticalConstraints(1, 0));
    EXPECT_EQ(constraintNotEq, instance.verticalConstraints(0, 4));
    EXPECT_EQ(constraintEq, instance.verticalConstraints(1, 4));
    EXPECT_TRUE(instance.verticalConstraints(0, 0).empty());
}

TEST(ListColoringClassTest, horizontalConstraint) {
    ListColoring::ConstraintContainer constraintEq = {
        {0, 0},
        {1, 1}
    };
    ListColoring::ConstraintContainer constraintNotEq = {
        {0, 1},
        {1, 0}
    };
    ListColoring::ProblemInstance instance(3, 5);
    instance.horizontalConstraints(2, 0) = constraintEq;
    instance.horizontalConstraints(0, 3) = constraintNotEq;
    instance.horizontalConstraints(2, 3) = constraintEq;
    EXPECT_EQ(constraintEq, instance.horizontalConstraints(2, 0));
    EXPECT_EQ(constraintNotEq, instance.horizontalConstraints(0, 3));
    EXPECT_EQ(constraintEq, instance.horizontalConstraints(2, 3));
    EXPECT_TRUE(instance.horizontalConstraints(0, 0).empty());
}

TEST(ListColoringClassTest, solutionConstructor) {
    ListColoring::Solution s(2, 3);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_TRUE(s.get(i, j).empty());
        }
    }
}

namespace {

std::string commaSep(int i, int j) {
    std::ostringstream ss;
    ss << i << "," << j;
    return ss.str();
}

}

TEST(ListColoringClassTest, solutionSetter) {
    ListColoring::Solution s(2, 3);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            s.get(i, j) = commaSep(i, j);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            EXPECT_EQ(commaSep(i, j), s.get(i, j));
        }
    }
}
