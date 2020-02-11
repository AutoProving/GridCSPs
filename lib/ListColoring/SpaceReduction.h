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
#include <optional>

namespace ListColoring {

/**
 * @brief Transforms an instance of size n * m to a series of instances of size
 * ⌈n / 2⌉ * (2m).
 *
 * The initial instance has a solution if and only if one of the produced
 * instances has.
 */
class SpaceReduction {
public:
    /**
     * @brief Constructs a reduction for a given instance.
     */
    SpaceReduction(const ProblemInstance& instance);

    ~SpaceReduction();
    SpaceReduction(const SpaceReduction&) = delete;
    SpaceReduction& operator=(const SpaceReduction&) = delete;
    SpaceReduction(SpaceReduction&&);
    SpaceReduction& operator=(SpaceReduction&&);

    /**
     * @brief Suggests a next instance to process.
     *
     * @return Nothing in case there're no instances left, next instance
     * otherwise.
     */
    std::optional<ProblemInstance> nextInstance();

    /**
     * @brief Generates a solution for the initial instance by the solution for
     * the last produced.
     *
     * Behaviour is undefined in case the given solution is not a solution.
     */
    Solution restoreSolution(const Solution& reducedSolution) const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
