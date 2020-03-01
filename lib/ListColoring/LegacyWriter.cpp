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

#include <ListColoring/LegacyWriter.h>

#include <sstream>

namespace ListColoring {
namespace Legacy {
namespace {

void writeAlphabetMap(std::ostream& os, const ODDs::ODD::AlphabetMap& a) {
    os << a.symbols().size() << std::endl;
    for (int i = 0; i < (int)a.symbols().size(); i++) {
        os << i << " " << a.numberToSymbol(i) << std::endl;
    }
}

void writeColorMap(std::ostream& os, const ColorMap& map) {
    os << map.size() << std::endl;
    for (int i = 0; i < (int)map.size(); i++) {
        os << i << " " << map[i] << std::endl;
    }
}

void writeColorLists(std::ostream& os,
                     const ProblemInstance& instance,
                     int i, int j) {
    os << "COLOR_LISTS " << i << " " << j << std::endl;
    os << "INTERMEDIATE_COLORS ";
    writeAlphabetMap(os, instance.intermediateColors(i, j));
    os << "FINAL_COLORS ";
    writeAlphabetMap(os, instance.finalColors(i, j));
    os << "COLOR_MAP ";
    writeColorMap(os, instance.colorMap(i, j));
}

enum class ConstraintType {
    VERTICAL,
    HORIZONTAL
};

void writeConstraint(std::ostream& os,
                     const ProblemInstance& instance,
                     ConstraintType type,
                     int i, int j) {
    std::string prefix =
        type == ConstraintType::VERTICAL
      ? "VERTICAL_CONSTRAINTS"
      : "HORIZONTAL_CONSTRAINTS";
    const ConstraintContainer& constraints =
        type == ConstraintType::VERTICAL
      ? instance.verticalConstraints(i, j)
      : instance.horizontalConstraints(i, j);
    os << prefix << " " << i << " " << j << std::endl;
    for (const Constraint& option : constraints) {
        os << option.first << " " << option.second << std::endl;
    }
}

}

void write(std::ostream& os, const ProblemInstance& instance) {
    os << "LIST_COLORING "
       << instance.height() << " "
       << instance.width() << std::endl;
    for (int i = 0; i < instance.height(); i++) {
        for (int j = 0; j < instance.width(); j++) {
            writeColorLists(os, instance, i, j);
        }
    }
    for (int i = 0; i < instance.height() - 1; i++) {
        for (int j = 0; j < instance.width(); j++) {
            writeConstraint(os, instance, ConstraintType::VERTICAL, i, j);
        }
    }
    for (int i = 0; i < instance.height(); i++) {
        for (int j = 0; j < instance.width() - 1; j++) {
            writeConstraint(os, instance, ConstraintType::HORIZONTAL, i, j);
        }
    }
}

std::string write(const ProblemInstance& instance) {
    std::ostringstream os;
    write(os, instance);
    return os.str();
}

}
}
