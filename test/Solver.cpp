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
#include <ListColoring/Solver.h>
#include <ListColoring/LegacyReader.h>
#include <ListColoring/Instances.h>

#include <Instances/Instances.h>

#include <filesystem>
#include <type_traits>

TEST(SolverTest, solverMoveSemanticsStatic) {
    static_assert(std::is_move_constructible_v<ListColoring::Solver>);
    static_assert(std::is_move_assignable_v<ListColoring::Solver>);
}

TEST(SolverTest, solverMoveSemanticsDynamic) {
    ListColoring::ProblemInstance instance(2, 2);
    ListColoring::Solver solver(instance);
    ListColoring::Solver movedSolver(std::move(solver));
    ListColoring::Solver solver2(instance);
    solver2 = std::move(movedSolver);
}

namespace {

ListColoring::ProblemInstance getInstance(std::string name) {
    return ListColoring::Legacy::read(TestInstances::getResource(name));
}

class TempDir {
public:
    TempDir()
        : path_(std::tmpnam(nullptr))
    {
        namespace fs = std::filesystem;
        fs::create_directories(path_);
    }

    ~TempDir() {
        namespace fs = std::filesystem;
        fs::remove_all(path_);
    }

    TempDir(const TempDir&) = delete;
    TempDir& operator=(const TempDir&) = delete;
    TempDir(TempDir&&) = delete;
    TempDir& operator=(TempDir&&) = delete;

    const std::string& path() const {
        return path_;
    }

private:
    std::string path_;
};

}

TEST(SolverTest, isThereSolutionExample) {
    ListColoring::ProblemInstance instance = getInstance("example");
    ASSERT_TRUE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, isThereSolutionExampleDisk) {
    TempDir temp;
    ListColoring::ProblemInstance instance = getInstance("example");
    ListColoring::Solver solver(instance);
    solver.diskMode(temp.path());
    ASSERT_TRUE(solver.isThereSolution());
}

TEST(SolverTest, isThereSolutionSimpleNoSolution) {
    ListColoring::ProblemInstance instance = getInstance("simpleNoSolution");
    ASSERT_FALSE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, isThereSolutionSimpleNoSolutionDisk) {
    TempDir temp;
    ListColoring::ProblemInstance instance = getInstance("simpleNoSolution");
    ListColoring::Solver solver(instance);
    solver.diskMode(temp.path());
    ASSERT_FALSE(solver.isThereSolution());
}

TEST(SolverTest, exampleRestoreSolution) {
    ListColoring::ProblemInstance instance = getInstance("example");
    std::vector<std::vector<std::string>> expected = {
        {"BlueSquare", "BlueCircle", "BlueCircle", "RedSquare"},
        {"BlueCircle", "Blank",      "Blank",      "RedCircle"},
        {"RedSquare",  "RedCircle",  "RedCircle",  "BlueSquare"}
    };
    ListColoring::Solver solver(instance);
    ASSERT_TRUE(solver.isThereSolution());
    ListColoring::Solution solution = solver.restoreSolution();
    for (int i = 0; i < instance.height(); i++) {
        for (int j = 0; j < instance.width(); j++) {
            EXPECT_EQ(expected[i][j], solution.get(i, j));
        }
    }
}

TEST(SolverTest, exampleRestoreSolutionDisk) {
    ListColoring::ProblemInstance instance = getInstance("example");
    std::vector<std::vector<std::string>> expected = {
        {"BlueSquare", "BlueCircle", "BlueCircle", "RedSquare"},
        {"BlueCircle", "Blank",      "Blank",      "RedCircle"},
        {"RedSquare",  "RedCircle",  "RedCircle",  "BlueSquare"}
    };
    ListColoring::Solver solver(instance);
    TempDir tmp;
    solver.diskMode(tmp.path());
    ASSERT_TRUE(solver.isThereSolution());
    ListColoring::Solution solution = solver.restoreSolution();
    for (int i = 0; i < instance.height(); i++) {
        for (int j = 0; j < instance.width(); j++) {
            EXPECT_EQ(expected[i][j], solution.get(i, j));
        }
    }
}

namespace {

class CountSolverStats : public ListColoring::SolverStatsBase {
public:
    CountSolverStats() = default;
    virtual ~CountSolverStats() = default;

    virtual void onRawODD(const ODDs::ODD&) override {
        rawCnt_++;
    }

    virtual void onDeterminateODD(const ODDs::ODD&) override {
        detCnt_++;
    }

    virtual void onMinimizedODD(const ODDs::ODD&) override {
        minCnt_++;
    }

    int rawCnt() const {
        return rawCnt_;
    }

    int detCnt() const {
        return detCnt_;
    }

    int minCnt() const {
        return minCnt_;
    }

private:
    int rawCnt_ = 0;
    int detCnt_ = 0;
    int minCnt_ = 0;
};

}

TEST(SolverTest, solverStats) {
    ListColoring::ProblemInstance instance = getInstance("example");
    CountSolverStats stats;
    ListColoring::Solver solver(instance, stats);
    ASSERT_TRUE(solver.isThereSolution());
    EXPECT_EQ(3, stats.rawCnt());
    EXPECT_EQ(3, stats.detCnt());
    EXPECT_EQ(3, stats.minCnt());
}

TEST(SolverTest, pigeonhole_10_10) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(10, 10);
    ASSERT_TRUE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, pigeonhole_10_10_disk) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(10, 10);
    ListColoring::Solver solver(instance);
    TempDir tmp;
    solver.diskMode(tmp.path());
    ASSERT_TRUE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_10_10_interrupted) {
    namespace fs = std::filesystem;
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(10, 10);
    TempDir tmp;
    {
        ListColoring::Solver solver(instance);
        solver.diskMode(tmp.path());
        solver.firstSteps(5);
    }
    ASSERT_TRUE(fs::exists(fs::path(tmp.path()) / "layer4_min"));
    ASSERT_FALSE(fs::exists(fs::path(tmp.path()) / "layer5_min"));
    ListColoring::Solver solver(instance);
    solver.continueInterrupted(tmp.path());
    ASSERT_EQ(5, solver.startFrom());
    ASSERT_TRUE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_10_10_immediatelyInterrupted) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(10, 10);
    ListColoring::Solver solver(instance);
    TempDir tmp;
    solver.continueInterrupted(tmp.path());
    ASSERT_EQ(0, solver.startFrom());
    ASSERT_TRUE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_5_15) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(5, 15);
    ASSERT_TRUE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, pigeonhole_5_15_disk) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(5, 15);
    ListColoring::Solver solver(instance);
    TempDir tmp;
    solver.diskMode(tmp.path());
    ASSERT_TRUE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_5_15_interrupted) {
    namespace fs = std::filesystem;
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(5, 15);
    TempDir tmp;
    {
        ListColoring::Solver solver(instance);
        solver.diskMode(tmp.path());
        solver.firstSteps(2);
    }
    ASSERT_TRUE(fs::exists(fs::path(tmp.path()) / "layer1_min"));
    ASSERT_FALSE(fs::exists(fs::path(tmp.path()) / "layer2_min"));
    ListColoring::Solver solver(instance);
    solver.continueInterrupted(tmp.path());
    ASSERT_EQ(2, solver.startFrom());
    ASSERT_TRUE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_15_5) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(15, 5);
    ASSERT_FALSE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, pigeonhole_15_5_disk) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(15, 5);
    ListColoring::Solver solver(instance);
    TempDir tmp;
    solver.diskMode(tmp.path());
    ASSERT_FALSE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_15_5_interrupted) {
    namespace fs = std::filesystem;
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(15, 5);
    TempDir tmp;
    {
        ListColoring::Solver solver(instance);
        solver.diskMode(tmp.path());
        solver.firstSteps(5);
    }
    ASSERT_TRUE(fs::exists(fs::path(tmp.path()) / "layer4_min"));
    ASSERT_FALSE(fs::exists(fs::path(tmp.path()) / "layer5_min"));
    ListColoring::Solver solver(instance);
    solver.continueInterrupted(tmp.path());
    ASSERT_EQ(5, solver.startFrom());
    ASSERT_FALSE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_10_9) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(10, 9);
    ASSERT_FALSE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, pigeonhole_10_9_disk) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(10, 9);
    ListColoring::Solver solver(instance);
    TempDir tmp;
    solver.diskMode(tmp.path());
    ASSERT_FALSE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_10_9_interrupted) {
    namespace fs = std::filesystem;
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(10, 9);
    TempDir tmp;
    {
        ListColoring::Solver solver(instance);
        solver.diskMode(tmp.path());
        solver.firstSteps(5);
    }
    ASSERT_TRUE(fs::exists(fs::path(tmp.path()) / "layer4_min"));
    ASSERT_FALSE(fs::exists(fs::path(tmp.path()) / "layer5_min"));
    ListColoring::Solver solver(instance);
    solver.continueInterrupted(tmp.path());
    ASSERT_EQ(5, solver.startFrom());
    ASSERT_FALSE(solver.isThereSolution());
}

TEST(SolverTest, pigeonhole_1_1) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(1, 1);
    ASSERT_TRUE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, pigeonhole_1_1_disk) {
    ListColoring::ProblemInstance instance = ListColoring::pigeonholeTest(1, 1);
    ListColoring::Solver solver(instance);
    TempDir tmp;
    solver.diskMode(tmp.path());
    ASSERT_TRUE(solver.isThereSolution());
}

TEST(SolverTest, emptyAlphabet) {
    ListColoring::ProblemInstance instance(1, 1);
    ASSERT_FALSE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, emptyAlphabetDisk) {
    ListColoring::ProblemInstance instance(1, 1);
    TempDir tmp;
    ListColoring::Solver solver(instance);
    solver.diskMode(tmp.path());
    ASSERT_FALSE(solver.isThereSolution());
}
