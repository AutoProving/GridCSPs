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

    ListColoring::ConstraintContainer equal = {{0, 0}, {1, 1}};
    ListColoring::ConstraintContainer notEqual = {{0, 1}, {1, 0}};
    ListColoring::ConstraintContainer fstZero = {{0, 0}, {0, 1}};
    ListColoring::ConstraintContainer sndZero = {{0, 0}, {1, 0}};
    ListColoring::ConstraintContainer bothZero = {{0, 0}, {0, 0}};

    instance.verticalConstraints(0, 0) = equal;
    instance.verticalConstraints(0, 1) = sndZero;
    instance.verticalConstraints(0, 2) = sndZero;
    instance.verticalConstraints(0, 3) = fstZero;
    
    instance.verticalConstraints(1, 0) = notEqual;
    instance.verticalConstraints(1, 1) = fstZero;
    instance.verticalConstraints(1, 2) = fstZero;
    instance.verticalConstraints(1, 3) = notEqual;

    instance.horizontalConstraints(0, 0) = equal;
    instance.horizontalConstraints(0, 1) = equal;
    instance.horizontalConstraints(0, 2) = notEqual;

    instance.horizontalConstraints(1, 0) = sndZero;
    instance.horizontalConstraints(1, 1) = bothZero;
    instance.horizontalConstraints(1, 2) = fstZero;

    instance.horizontalConstraints(2, 0) = equal;
    instance.horizontalConstraints(2, 1) = equal;
    instance.horizontalConstraints(2, 2) = notEqual;

    return instance;
}

std::string getResource(std::string resourceName) {
    std::ifstream fin(RESOURCE_DIR "/" + resourceName + ".txt");
    return std::string(std::istreambuf_iterator<char>(fin),
                       std::istreambuf_iterator<char>());
}

}
