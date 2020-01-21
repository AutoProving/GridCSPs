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
    const Constraint& constraint =
        type == ConstraintType::VERTICAL
      ? instance.verticalConstraint(i, j)
      : instance.horizontalConstraint(i, j);
    os << prefix << " " << i << " " << j << std::endl;
    for (const ConstraintOption& option : constraint) {
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
