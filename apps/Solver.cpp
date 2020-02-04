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

#include <ListColoring/ListColoring.h>
#include <ListColoring/LegacyReader.h>
#include <ListColoring/Solver.h>

#include <cassert>
#include <iostream>

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
        int minWidth;
    };

    SolverStats() = default;
    virtual ~SolverStats() = default;

    virtual void onRawODD(const ODDs::ODD& odd) override {
        rawWidths_.push_back(width(odd));
    }

    virtual void onMinimizedODD(const ODDs::ODD& odd) override {
        minWidths_.push_back(width(odd));
    }

    std::vector<RowStat> collect() const {
        assert(rawWidths_.size() == minWidths_.size());
        std::vector<RowStat> ret(rawWidths_.size());
        for (int i = 0; i < (int)rawWidths_.size(); i++) {
            ret[i] = {rawWidths_[i], minWidths_[i]};
        }
        return ret;
    }

private:
    std::vector<int> rawWidths_;
    std::vector<int> minWidths_;
};

}

int main() {
    ListColoring::ProblemInstance pi = ListColoring::Legacy::read(std::cin);
    SolverStats stats;
    ListColoring::Solver solver(pi, stats);
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
    auto rows = stats.collect();
    std::cout << std::endl
              << "Solver stats: " << std::endl;
    for (int i = 0; i < (int)rows.size(); i++) {
        std::cout << "Row " << i << ": "
                  << "rawWidth = " << rows[i].rawWidth << " "
                  << "minWidth = " << rows[i].minWidth << std::endl;
    }
    return 0;
}
