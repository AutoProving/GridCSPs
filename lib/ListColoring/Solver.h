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

#pragma once

#include <ListColoring/ListColoring.h>

#include <ODDs/ODDs.h>

#include <memory>

namespace ListColoring {

/**
 * @brief A base class for solver statistics collectors.
 *
 * A descendant of this class can be appointed to a Solver to be trigerred on
 * solver events. By default, a trivial mocked object that does nothing is
 * appointed.
 */
class SolverStatsBase {
public:
    virtual ~SolverStatsBase() = default;

    /**
     * @brief Trigger for a raw ODD.
     *
     * Triggered every time a solver produces a raw (not yet minimized) ODD.
     */
    virtual void onRawODD(const ODDs::ODD& rawODD) = 0;

    /**
     * @brief Trigger for a determinate ODD.
     *
     * Triggered every time an ODD is determinized but not yet minimized.
     */
    virtual void onDeterminateODD(const ODDs::ODD& powsetODD) = 0;

    /**
     * @brief Trigger for a minimal ODD.
     *
     * Triggered every time a solver produses a minimized ODD.
     */
    virtual void onMinimizedODD(const ODDs::ODD& minimizedODD) = 0;
};

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
     *
     * Sets the stats reference to a fake mock object.
     */
    Solver(const ProblemInstance& pi);

    /**
     * @brief Constructs a solver from a reference to an instance and a stats
     * handle.
     *
     * The referred instance and stats handle are stored by reference and
     * should live at least until the Solver is destroyed.
     */
    Solver(const ProblemInstance& pi, SolverStatsBase& stats);

    ~Solver();
    Solver(const Solver&) = delete;
    Solver& operator=(const Solver&) = delete;
    Solver(Solver&&);
    Solver& operator=(Solver&&);

    /**
     * @brief Switch solver to disk mode.
     *
     * In disk mode, the solver operates disk-mode ODDs, which are much slower
     * but much less memory-consuming, as only one layer of one ODD is stored
     * in memory.
     *
     * @param dirName Path to the working directory.
     */
    void diskMode(const std::string& dirName);

    /**
     * @brief Continue execution from an interrupted disk-mode process.
     *
     * Load saved complete ODDs for the intermediate layers. The execution for
     * isThereSolution() will start from building the ODD that was under
     * construction at the moment of interuption.
     *
     * Switches solver to disk mode in case no execution data can be found in
     * the provided dir (for instance, if the process was interrupted before
     * the first ODD was constructed).
     */
    void continueInterrupted(const std::string& dirName);

    /**
     * @brief Get the first row from which the execution will start.
     *
     * Needed for testing purposes to ensure that the execution context from
     * an interrupted process is restored.
     */
    int startFrom() const;

    /**
     * @brief Checks an instance on solution existence.
     */
    bool isThereSolution();

    /**
     * @brief Build first k ODDs in the isThereSolution() algorithm.
     *
     * Detaches ODDs after performing the steps.
     *
     * Needed for testing purposes.
     */
    void firstSteps(int k);

    /**
     * @brief Restores a solution after the check if it exists.
     *
     * The behaviour is undefined in case it's called before isThereSolution().
     */
    Solution restoreSolution() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}
