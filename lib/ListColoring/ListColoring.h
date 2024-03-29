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

#pragma once

#include <ODDs/ODDs.h>

#include <set>
#include <utility>

namespace ListColoring {
/**
 * @brief Type for color identifiers
 */
using Color = int;

/**
 * @brief Mapping between intermediate and final color identifiers.
 */
using ColorMap = std::vector<Color>;

/**
 * @brief A pair of colors.
 *
 * Represents a possible option for constraint on edge ends' colors.
 */
using Constraint = std::pair<Color, Color>;

/**
 * @brief A constraint on edge ends' colors.
 */
using ConstraintContainer = std::set<Constraint>;

/**
 * @brief List Coloring instance
 */
class ProblemInstance {
public:
    /**
     * @brief Create problem instance of given size.
     *
     * Allocates memory, without creating any 
     */
    ProblemInstance(int rows, int columns);

    ~ProblemInstance() = default;
    ProblemInstance(const ProblemInstance&) = default;
    ProblemInstance& operator=(const ProblemInstance&) = default;
    ProblemInstance(ProblemInstance&&) = default;
    ProblemInstance& operator=(ProblemInstance&&) = default;

    /**
     * @brief Number of rows in the instance.
     */
    int height() const;

    /**
     * @brief Number of columns in the instance.
     */
    int width() const;

    /**
     * @brief Access the intermediate color map of a vertex.
     */
    ODDs::ODD::AlphabetMap& intermediateColors(int r, int c);

    /**
     * @brief Access the intermediate color map of a vertex.
     */
    const ODDs::ODD::AlphabetMap& intermediateColors(int r, int c) const;

    /**
     * @brief Access the final color map of a vertex.
     */
    ODDs::ODD::AlphabetMap& finalColors(int r, int c);

    /**
     * @brief Access the final color map of a vertex.
     */
    const ODDs::ODD::AlphabetMap& finalColors(int r, int c) const;

    /**
     * @brief Access the color map of a vertex.
     */
    ColorMap& colorMap(int r, int c);

    /**
     * @brief Access the color map of a vertex.
     */
    const ColorMap& colorMap(int r, int c) const;

    /**
     * @brief Access a vertical constraint on an edge.
     * @param r Row of top vertex of an edge.
     * @param c Column of top vertex of an edge.
     */
    ConstraintContainer& verticalConstraints(int r, int c);

    /**
     * @brief Access a vertical constraint on an edge.
     * @param r Row of top vertex of an edge.
     * @param c Column of top vertex of an edge.
     */
    const ConstraintContainer& verticalConstraints(int r, int c) const;

    /**
     * @brief Access a horisontal constraint on an edge.
     * @param r Row of left vertex of an edge.
     * @param c Column of left vertex of an edge.
     */
    ConstraintContainer& horizontalConstraints(int r, int c);

    /**
     * @brief Access a horisontal constraint on an edge.
     * @param r Row of left vertex of an edge.
     * @param c Column of left vertex of an edge.
     */
    const ConstraintContainer& horizontalConstraints(int r, int c) const;

private:
    /**
     * @brief Per-vertex color mapping.
     */
    struct VertexColorMapping {
        ODDs::ODD::AlphabetMap intermediateColors;
        ODDs::ODD::AlphabetMap finalColors;
        ColorMap colorMap;
    };

    template<class T>
    using DynArray2D = std::vector<std::vector<T>>;

    template<class T>
    static DynArray2D<T> allocate2DArray(int height, int width) {
        std::vector<T> row(width);
        row.shrink_to_fit();
        DynArray2D<T> ret(height, std::move(row));
        ret.shrink_to_fit();
        return ret;
    }

    int rows_, columns_;
    DynArray2D<VertexColorMapping> vcolors_;
    DynArray2D<ConstraintContainer> vconstraints_;
    DynArray2D<ConstraintContainer> hconstraints_;
};

/**
 * @brief Represents a solution to the problem as a matrix of final color names.
 */
class Solution {
public:
    /**
     * @brief Constructs a matrix of empty strings of given size.
     */
    Solution(int height, int width);

    /**
     * @brief Non-constant getter for modifying the solution.
     */
    std::string& get(int i, int j);

    /**
     * @brief Constant getter for accessing the solution.
     */
    const std::string& get(int i, int j) const;

private:
    std::vector<std::vector<std::string>> solution_;
};

}
