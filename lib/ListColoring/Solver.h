// Copyright (c) 2019-2020 Mateus de Oliveira Oliveira and Contributors. 
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
