#include <gtest/gtest.h>
#include <ListColoring/Solver.h>
#include <ListColoring/LegacyReader.h>

#include <Instances/Instances.h>

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

}

TEST(SolverTest, isThereSolutionExample) {
    ListColoring::ProblemInstance instance = getInstance("example");
    ASSERT_TRUE(ListColoring::Solver(instance).isThereSolution());
}

TEST(SolverTest, isThereSolutionSimpleNoSolution) {
    ListColoring::ProblemInstance instance = getInstance("simpleNoSolution");
    ASSERT_FALSE(ListColoring::Solver(instance).isThereSolution());
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
