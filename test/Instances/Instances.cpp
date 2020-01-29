#include <Instances/Instances.h>

#include <fstream>
#include <streambuf>

namespace TestInstances {

ListColoring::ProblemInstance sample() {
    ODDs::ODD::AlphabetMap circle;
    circle.addSymbol("RedCircle");
    circle.addSymbol("BlueCircle");
    ODDs::ODD::AlphabetMap square;
    square.addSymbol("RedSquare");
    square.addSymbol("BlueSquare");
    ODDs::ODD::AlphabetMap blank;
    blank.addSymbol("Blank");
    ListColoring::ColorMap colorMap2 = {0, 1};
    ListColoring::ColorMap colorMap1 = {0};

    ListColoring::ProblemInstance instance(3, 4);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            bool iBorder = i == 0 || i == 2;
            bool jBorder = j == 0 || j == 3;
            int border = iBorder + jBorder;
            if (border == 0) {
                instance.intermediateColors(i, j) = blank;
                instance.finalColors(i, j) = blank;
                instance.colorMap(i, j) = colorMap1;
            } else if (border == 1) {
                instance.intermediateColors(i, j) = circle;
                instance.finalColors(i, j) = circle;
                instance.colorMap(i, j) = colorMap2;
            } else {
                instance.intermediateColors(i, j) = square;
                instance.finalColors(i, j) = square;
                instance.colorMap(i, j) = colorMap2;
            }
        }
    }

    ListColoring::Constraint equal = {{0, 0}, {1, 1}};
    ListColoring::Constraint notEqual = {{0, 1}, {1, 0}};
    ListColoring::Constraint fstZero = {{0, 0}, {0, 1}};
    ListColoring::Constraint sndZero = {{0, 0}, {1, 0}};
    ListColoring::Constraint bothZero = {{0, 0}, {0, 0}};

    instance.verticalConstraint(0, 0) = equal;
    instance.verticalConstraint(0, 1) = sndZero;
    instance.verticalConstraint(0, 2) = sndZero;
    instance.verticalConstraint(0, 3) = fstZero;
    
    instance.verticalConstraint(1, 0) = notEqual;
    instance.verticalConstraint(1, 1) = fstZero;
    instance.verticalConstraint(1, 2) = fstZero;
    instance.verticalConstraint(1, 3) = notEqual;

    instance.horizontalConstraint(0, 0) = equal;
    instance.horizontalConstraint(0, 1) = equal;
    instance.horizontalConstraint(0, 2) = notEqual;

    instance.horizontalConstraint(1, 0) = sndZero;
    instance.horizontalConstraint(1, 1) = bothZero;
    instance.horizontalConstraint(1, 2) = fstZero;

    instance.horizontalConstraint(2, 0) = equal;
    instance.horizontalConstraint(2, 1) = equal;
    instance.horizontalConstraint(2, 2) = notEqual;

    return instance;
}

std::string getResource(std::string resourceName) {
    std::ifstream fin(RESOURCE_DIR "/" + resourceName + ".txt");
    return std::string(std::istreambuf_iterator<char>(fin),
                       std::istreambuf_iterator<char>());
}

}
