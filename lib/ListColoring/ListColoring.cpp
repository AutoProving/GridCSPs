// Copyright (c) 2019 Vasily Alferov, Mateus de Oliveira Oliveira and Contributors.
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
#include <ListColoring/LegacyWriter.h>

namespace ListColoring {

ProblemInstance::ProblemInstance(int rows, int columns)
    : rows_(rows)
    , columns_(columns)
    , vcolors_(allocate2DArray<VertexColorMapping>(rows_, columns_))
    , vconstraints_(allocate2DArray<ConstraintContainer>(rows_ - 1,  columns_))
    , hconstraints_(allocate2DArray<ConstraintContainer>(rows_, columns_ - 1))
{}

int ProblemInstance::height() const {
    return rows_;
}

int ProblemInstance::width() const {
    return columns_;
}

ODDs::ODD::AlphabetMap& ProblemInstance::intermediateColors(int r, int c) {
    return vcolors_[r][c].intermediateColors;
}

const ODDs::ODD::AlphabetMap& ProblemInstance::intermediateColors(int r, int c) const {
    return vcolors_[r][c].intermediateColors;
}

ODDs::ODD::AlphabetMap& ProblemInstance::finalColors(int r, int c) {
    return vcolors_[r][c].finalColors;
}

const ODDs::ODD::AlphabetMap& ProblemInstance::finalColors(int r, int c) const {
    return vcolors_[r][c].finalColors;
}

ColorMap& ProblemInstance::colorMap(int r, int c) {
    return vcolors_[r][c].colorMap;
}

const ColorMap& ProblemInstance::colorMap(int r, int c) const {
    return vcolors_[r][c].colorMap;
}

ConstraintContainer& ProblemInstance::verticalConstraints(int r, int c) {
    return vconstraints_[r][c];
}

const ConstraintContainer& ProblemInstance::verticalConstraints(int r, int c) const {
    return vconstraints_[r][c];
}

ConstraintContainer& ProblemInstance::horizontalConstraints(int r, int c) {
    return hconstraints_[r][c];
}

const ConstraintContainer& ProblemInstance::horizontalConstraints(int r, int c) const {
    return hconstraints_[r][c];
}

Solution::Solution(int height, int width)
    : solution_(height, std::vector<std::string>(width))
{}

std::string& Solution::get(int i, int j) {
    return solution_[i][j];
}

const std::string& Solution::get(int i, int j) const {
    return solution_[i][j];
}

}
