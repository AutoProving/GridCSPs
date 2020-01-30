#include <ListColoring/ListColoring.h>
#include <ListColoring/LegacyReader.h>
#include <ListColoring/Solver.h>

#include <iostream>

int main() {
    ListColoring::ProblemInstance pi = ListColoring::Legacy::read(std::cin);
    ListColoring::Solver solver(pi);
    if (!solver.isThereSolution()) {
        std::cout << "No solution" << std::endl;
        return 0;
    }
    ListColoring::Solution solution = solver.restoreSolution();
    for (int i = 0; i < pi.height(); i++) {
        for (int j = 0; j < pi.width(); j++)
            std::cout << solution.get(i, j) << " ";
        std::cout << std::endl;
    }
    return 0;
}
