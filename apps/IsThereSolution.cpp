#include <ListColoring/ListColoring.h>
#include <ListColoring/LegacyReader.h>
#include <ListColoring/Solver.h>

#include <iostream>

int main() {
    ListColoring::ProblemInstance pi = ListColoring::Legacy::read(std::cin);
    if (ListColoring::Solver(pi).isThereSolution()) {
        std::cout << "Yes" << std::endl;
    } else {
        std::cout << "No" << std::endl;
    }
}
