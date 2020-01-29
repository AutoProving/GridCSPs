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
