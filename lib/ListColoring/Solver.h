#pragma once

#include <ListColoring/ListColoring.h>

#include <memory>

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

    ~Solver();
    Solver(const Solver&) = delete;
    Solver& operator=(const Solver&) = delete;
    Solver(Solver&&);
    Solver& operator=(Solver&&);

    /**
     * @brief Checks an instance on solution existence.
     */
    bool isThereSolution();

    /**
     * @brief Restores a solution after the check if it exists.
     *
     * The behaviour is undefined in case it's called before isThereSolution.
     */
    Solution restoreSolution() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
