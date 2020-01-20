#include <ListColoring/ListColoring.h>

namespace ListColoring {

ProblemInstance::ProblemInstance(int rows, int columns)
    : rows_(rows)
    , columns_(columns)
    , vcolors_(allocate2DArray<VertexColorMapping>(rows_, columns_))
    , vconstraints_(allocate2DArray<Constraint>(rows_ - 1,  columns_))
    , hconstraints_(allocate2DArray<Constraint>(rows_, columns_ - 1))
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

Constraint& ProblemInstance::verticalConstraint(int r, int c) {
    return vconstraints_[r][c];
}

const Constraint& ProblemInstance::verticalConstraint(int r, int c) const {
    return vconstraints_[r][c];
}

Constraint& ProblemInstance::horizontalConstraint(int r, int c) {
    return hconstraints_[r][c];
}

const Constraint& ProblemInstance::horizontalConstraint(int r, int c) const {
    return hconstraints_[r][c];
}

}
