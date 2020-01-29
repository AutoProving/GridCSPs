#include <gtest/gtest.h>
#include <ListColoring/Solver.h>

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
