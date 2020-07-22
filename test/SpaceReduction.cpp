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
#include <ListColoring/Instances.h>
#include <ListColoring/SpaceReduction.h>
#include <ListColoring/Solver.h>
#include <ListColoring/LegacyWriter.h>

namespace {

void checkPigeonholeSolution(int n , int m,
                             const ListColoring::Solution& solution) {
    ASSERT_TRUE(n <= m);
    for (int i = 0; i < n; i++) {
        EXPECT_EQ("b", solution.get(i, 0));
        EXPECT_EQ("g", solution.get(i, m + 1));
    }
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < m + 2; j++) {
            if (solution.get(i, j)[0] == 'b') {
                EXPECT_EQ('b', solution.get(i, j - 1)[0]);
            } else if (solution.get(i, j)[0] == 'r') {
                EXPECT_EQ('b', solution.get(i, j - 1)[0]);
            } else {
                ASSERT_EQ('g', solution.get(i, j)[0]);
                EXPECT_TRUE('r' == solution.get(i, j - 1)[0]
                         || 'g' == solution.get(i, j - 1)[0]);
            }
        }
    }
    for (int j = 1; j <= m; j++) {
        for (int i = 1; i < n; i++) {
            if (solution.get(i, j)[1] == 'b') {
                EXPECT_EQ('b', solution.get(i - 1, j)[1]);
            } else if (solution.get(i, j)[1] == 'r') {
                EXPECT_EQ('b', solution.get(i - 1, j)[1]);
            } else {
                ASSERT_EQ('g', solution.get(i, j)[1]);
                EXPECT_TRUE('r' == solution.get(i - 1, j)[1]
                         || 'g' == solution.get(i - 1, j)[1]);
            }
        }
    }
}

std::optional<ListColoring::Solution> solve(const ListColoring::ProblemInstance& instance,
                                            int depth) {
    if (depth == 0) {
        ListColoring::Solver solver(instance);
        if (!solver.isThereSolution())
            return {};
        return solver.restoreSolution();
    }
    ListColoring::SpaceReduction r(instance);
    auto reduced = r.nextInstance();
    while (reduced.has_value()) {
        auto underlying = solve(reduced.value(), depth - 1);
        if (underlying.has_value())
            return r.restoreSolution(underlying.value());
        reduced = r.nextInstance();
    }
    return {};
}

void testPigeonhole(int n, int m, int depth = 1) {
    auto instance = ListColoring::pigeonholeTest(n, m);
    auto solution = solve(instance, depth);
    ASSERT_EQ(n <= m, solution.has_value());
    if (n <= m) {
        checkPigeonholeSolution(n, m, solution.value());
    }
}

}

TEST(SpaceReductionTest, pigeonhole_4_4) {
    testPigeonhole(4, 4);
}

TEST(SpaceReductionTest, pigeonhole_5_5) {
    testPigeonhole(5, 5);
}

TEST(SpaceReductionTest, pigeonhole_6_6) {
    testPigeonhole(6, 6);
}

TEST(SpaceReductionTest, pigeonhole_8_8) {
    testPigeonhole(8, 8);
}

TEST(SpaceReductionTest, pigeonhole_10_10) {
    testPigeonhole(10, 10);
}

TEST(SpaceReductionTest, pigeonhole_6_5) {
    testPigeonhole(6, 5);
}
