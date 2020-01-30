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
    ListColoring::Constraint constraintEq = {
        {0, 0},
        {1, 1}
    };
    ListColoring::Constraint constraintNotEq = {
        {0, 1},
        {1, 0}
    };
    ListColoring::ProblemInstance instance(3, 5);
    instance.verticalConstraint(1, 0) = constraintEq;
    instance.verticalConstraint(0, 4) = constraintNotEq;
    instance.verticalConstraint(1, 4) = constraintEq;
    EXPECT_EQ(constraintEq, instance.verticalConstraint(1, 0));
    EXPECT_EQ(constraintNotEq, instance.verticalConstraint(0, 4));
    EXPECT_EQ(constraintEq, instance.verticalConstraint(1, 4));
    EXPECT_TRUE(instance.verticalConstraint(0, 0).empty());
}

TEST(ListColoringClassTest, horizontalConstraint) {
    ListColoring::Constraint constraintEq = {
        {0, 0},
        {1, 1}
    };
    ListColoring::Constraint constraintNotEq = {
        {0, 1},
        {1, 0}
    };
    ListColoring::ProblemInstance instance(3, 5);
    instance.horizontalConstraint(2, 0) = constraintEq;
    instance.horizontalConstraint(0, 3) = constraintNotEq;
    instance.horizontalConstraint(2, 3) = constraintEq;
    EXPECT_EQ(constraintEq, instance.horizontalConstraint(2, 0));
    EXPECT_EQ(constraintNotEq, instance.horizontalConstraint(0, 3));
    EXPECT_EQ(constraintEq, instance.horizontalConstraint(2, 3));
    EXPECT_TRUE(instance.horizontalConstraint(0, 0).empty());
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
