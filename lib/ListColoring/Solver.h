#pragma once

#include <ListColoring/ListColoring.h>

#include <functional>

namespace ListColoring {

/**
 * @brief Solves the List Coloring problem.
 */
class Solver {
public:
    /**
     * @brief Constructs a solver from a reference to an instance.
     *
     * A referred instance is stored by reference and should live at least
     * until the Solver is destroyed.
     */
    Solver(const ProblemInstance& pi);

    ~Solver() = default;
    Solver(const Solver&) = delete;
    Solver& operator=(const Solver&) = delete;
    Solver(Solver&&) = default;
    Solver& operator=(Solver&&) = default;

    /**
     * @brief Checks an instance on solution existence.
     */
    bool isThereSolution();

private:
    std::reference_wrapper<const ProblemInstance> instance_;
};

}
