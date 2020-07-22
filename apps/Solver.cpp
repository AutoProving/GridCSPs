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

#include <cxxopts.hpp>

#include <ListColoring/ListColoring.h>
#include <ListColoring/LegacyReader.h>
#include <ListColoring/Solver.h>
#include <ListColoring/SpaceReduction.h>

#include <cassert>
#include <cstring>
#include <functional>
#include <iomanip>
#include <iostream>
#include <set>

namespace {

int width(const ODDs::ODD& odd) {
    int ret = 0;
    for (int i = 0; i < odd.countLayers(); i++) {
        ret = std::max(ret, odd.getLayer(i).width());
    }
    return ret;
}

class SolverStats : public ListColoring::SolverStatsBase {
public:
    struct RowStat {
        int rawWidth;
        int detWidth;
        int minWidth;
    };

    SolverStats() = default;
    virtual ~SolverStats() = default;

    virtual void onRawODD(const ODDs::ODD& odd) override {
        rawWidths_.push_back(width(odd));
    }

    virtual void onDeterminateODD(const ODDs::ODD& odd) override {
        detWidths_.push_back(width(odd));
    }

    virtual void onMinimizedODD(const ODDs::ODD& odd) override {
        minWidths_.push_back(width(odd));
    }

    std::vector<RowStat> collect() const {
        assert(rawWidths_.size() == detWidths_.size());
        assert(rawWidths_.size() == minWidths_.size());
        std::vector<RowStat> ret(rawWidths_.size());
        for (int i = 0; i < (int)rawWidths_.size(); i++) {
            ret[i] = {rawWidths_[i], detWidths_[i], minWidths_[i]};
        }
        return ret;
    }

private:
    std::vector<int> rawWidths_;
    std::vector<int> detWidths_;
    std::vector<int> minWidths_;
};

int justSolve(const ListColoring::ProblemInstance& pi,
              bool quiet,
              const std::string& dirName,
              bool continueExecution) {
    SolverStats stats;
    ListColoring::Solver solver(pi, stats);
    if (!dirName.empty()) {
        if (continueExecution) {
            solver.continueInterrupted(dirName);
            if (!quiet) {
                std::cerr << "Continuing interrupted execution from row " << solver.startFrom() << std::endl;
            }
        } else {
            solver.diskMode(dirName);
        }
    }
    if (!solver.isThereSolution()) {
        std::cout << "No solution" << std::endl;
    } else {
        ListColoring::Solution solution = solver.restoreSolution();
        for (int i = 0; i < pi.height(); i++) {
            for (int j = 0; j < pi.width(); j++)
                std::cout << solution.get(i, j) << " ";
            std::cout << std::endl;
        }
    }
    if (!quiet) {
        auto rows = stats.collect();
        std::cout << std::endl
                  << "Solver stats: " << std::endl;
        for (int i = 0; i < (int)rows.size(); i++) {
            std::cout << "Row " << i << ": "
                      << "rawWidth = " << rows[i].rawWidth << " "
                      << "detWidth = " << rows[i].detWidth << " "
                      << "minWidth = " << rows[i].minWidth << std::endl;
        }
    }
    return 0;
}

class SolverStatsAggregator {
public:
    struct RowStat {
        double rawWidth = 0;
        double detWidth = 0;
        double minWidth = 0;
    };

    struct Stats {
        int calls;
        std::vector<RowStat> rows;
    };

    void addStats(const SolverStats& stats) {
        calls_++;
        auto rows = stats.collect();
        rows_.resize(rows.size());
        for (int i = 0; i < (int)rows_.size(); i++) {
            rows_[i].rawWidth += rows[i].rawWidth;
            rows_[i].detWidth += rows[i].detWidth;
            rows_[i].minWidth += rows[i].minWidth;
        }
    }

    Stats collect() {
        Stats ret { calls_, rows_ };
        for (auto& stat : ret.rows) {
            stat.rawWidth /= calls_;
            stat.detWidth /= calls_;
            stat.minWidth /= calls_;
        }
        return ret;
    }

private:
    int calls_ = 0;
    std::vector<RowStat> rows_;
};

int reduceAndSolve(const ListColoring::ProblemInstance& pi, bool quiet) {
    ListColoring::SpaceReduction sr(pi);
    SolverStatsAggregator statsAgg;
    std::optional<ListColoring::Solution> solution;
    while (!solution.has_value()) {
        auto instance = sr.nextInstance();
        if (!instance.has_value()) {
            break;
        }
        SolverStats stats;
        ListColoring::Solver solver(instance.value(), stats);
        if (solver.isThereSolution()) {
            solution = sr.restoreSolution(solver.restoreSolution());
        }
        statsAgg.addStats(stats);
    }
    if (!solution.has_value()) {
        std::cout << "No solution" << std::endl;
    } else {
        for (int i = 0; i < pi.height(); i++) {
            for (int j = 0; j < pi.width(); j++)
                std::cout << solution.value().get(i, j) << " ";
            std::cout << std::endl;
        }
    }
    if (!quiet) {
        auto stats = statsAgg.collect();
        std::cout << std::endl
                  << "Average solver stats over " << stats.calls << " launches: " << std::endl;
        for (int i = 0; i < (int)stats.rows.size(); i++) {
            std::cout << std::setprecision(3)
                      << std::fixed
                      << "Row " << i << ": "
                      << "rawWidth = " << stats.rows[i].rawWidth << " "
                      << "detWidth = " << stats.rows[i].detWidth << " "
                      << "minWidth = " << stats.rows[i].minWidth << std::endl;
        }
    }
    return 0;
}

}

int main(int argc, char* argv[]) {
    cxxopts::Options options("solver", "A List Coloring solver");

    options.add_options()
        ("q,quiet", "Quiet mode")
        ("r,reduce-space", "Use space reductions")
        ("d,dir", "Directory for disk mode ODDs",
                  cxxopts::value<std::string>()->default_value(""))
        ("c,continue", "Continue interrupted execution in the same directory")
        ("h,help", "Print help")
    ;
    auto args = options.parse(argc, argv);
    if (args.count("help")) {
        std::cout << options.help() << std::endl;
        return 0;
    }

    ListColoring::ProblemInstance pi = ListColoring::Legacy::read(std::cin);

    bool quiet = args.count("quiet");
    bool continueExecution = args.count("continue");
    std::string dirName = args["dir"].as<std::string>();
    if (continueExecution && dirName.empty()) {
        std::cerr << "Directory name must not be empty in the continue-execution mode" << std::endl;
        return 2;
    }
    if (args.count("reduce-space")) {
        return reduceAndSolve(pi, quiet);
    } else {
        return justSolve(pi, quiet, dirName, continueExecution);
    }
}
