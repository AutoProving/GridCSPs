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
using ConstraintOption = std::pair<Color, Color>;

/**
 * @brief A constraint on edge ends' colors.
 */
using Constraint = std::set<ConstraintOption>;

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
    Constraint& verticalConstraint(int r, int c);

    /**
     * @brief Access a vertical constraint on an edge.
     * @param r Row of top vertex of an edge.
     * @param c Column of top vertex of an edge.
     */
    const Constraint& verticalConstraint(int r, int c) const;

    /**
     * @brief Access a horisontal constraint on an edge.
     * @param r Row of left vertex of an edge.
     * @param c Column of left vertex of an edge.
     */
    Constraint& horizontalConstraint(int r, int c);

    /**
     * @brief Access a horisontal constraint on an edge.
     * @param r Row of left vertex of an edge.
     * @param c Column of left vertex of an edge.
     */
    const Constraint& horizontalConstraint(int r, int c) const;

    /**
     * @brief Compares two instances on equality.
     *
     * Useful for tests.
     */
    bool operator==(const ProblemInstance& rhs) const;

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
    DynArray2D<Constraint> vconstraints_;
    DynArray2D<Constraint> hconstraints_;
};

}
