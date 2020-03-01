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

#include <ListColoring/Instances.h>

namespace ListColoring {
namespace {
namespace Pigeonhole {

ODDs::ODD::AlphabetMap leftBorderAlphabet() {
    ODDs::ODD::AlphabetMap alphabet;
    alphabet.addSymbol("b");
    return alphabet;
}

ODDs::ODD::AlphabetMap rightBorderAlphabet() {
    ODDs::ODD::AlphabetMap alphabet;
    alphabet.addSymbol("g");
    return alphabet;
}

ODDs::ODD::AlphabetMap intermediateAlphabet() {
    ODDs::ODD::AlphabetMap alphabet;
    alphabet.addSymbol("b");
    alphabet.addSymbol("g");
    alphabet.addSymbol("bb");
    alphabet.addSymbol("bg");
    alphabet.addSymbol("rr");
    alphabet.addSymbol("gb");
    alphabet.addSymbol("gg");
    return alphabet;
}

ColorMap borderColorMap() {
    return {0};
}

ColorMap intermediateColorMap() {
    return {0, 1, 2, 3, 4, 5, 6};
}

ConstraintContainer leftBorderHConstraints() {
    static const ODDs::ODD::AlphabetMap leftAlphabet = leftBorderAlphabet();
    static const ODDs::ODD::AlphabetMap rightAlphabet = intermediateAlphabet();
    return {
        {leftAlphabet.symbolToNumber("b"), rightAlphabet.symbolToNumber("bb")},
        {leftAlphabet.symbolToNumber("b"), rightAlphabet.symbolToNumber("bg")},
        {leftAlphabet.symbolToNumber("b"), rightAlphabet.symbolToNumber("rr")},
    };
}

ConstraintContainer rightBorderHConstraints() {
    static const ODDs::ODD::AlphabetMap leftAlphabet = intermediateAlphabet();
    static const ODDs::ODD::AlphabetMap rightAlphabet = rightBorderAlphabet();
    return {
        {leftAlphabet.symbolToNumber("gg"), rightAlphabet.symbolToNumber("g")},
        {leftAlphabet.symbolToNumber("gb"), rightAlphabet.symbolToNumber("g")},
        {leftAlphabet.symbolToNumber("rr"), rightAlphabet.symbolToNumber("g")},
    };
}

ConstraintContainer horizontalConstraints() {
    static const ODDs::ODD::AlphabetMap alphabet = intermediateAlphabet();
    return {
        // b_ -> b_, b_ -> rr
        {alphabet.symbolToNumber("bb"), alphabet.symbolToNumber("bb")},
        {alphabet.symbolToNumber("bb"), alphabet.symbolToNumber("bg")},
        {alphabet.symbolToNumber("bb"), alphabet.symbolToNumber("rr")},
        {alphabet.symbolToNumber("bg"), alphabet.symbolToNumber("bb")},
        {alphabet.symbolToNumber("bg"), alphabet.symbolToNumber("bg")},
        {alphabet.symbolToNumber("bg"), alphabet.symbolToNumber("rr")},
        // rr -> g_
        {alphabet.symbolToNumber("rr"), alphabet.symbolToNumber("gb")},
        {alphabet.symbolToNumber("rr"), alphabet.symbolToNumber("gg")},
        // g_ -> g_
        {alphabet.symbolToNumber("gb"), alphabet.symbolToNumber("gb")},
        {alphabet.symbolToNumber("gb"), alphabet.symbolToNumber("gg")},
        {alphabet.symbolToNumber("gg"), alphabet.symbolToNumber("gb")},
        {alphabet.symbolToNumber("gg"), alphabet.symbolToNumber("gg")},
    };
}

ConstraintContainer borderVerticalConstraints() {
    return {{0, 0}, {0, 0}};
}

ConstraintContainer verticalConstraints() {
    static const ODDs::ODD::AlphabetMap alphabet = intermediateAlphabet();
    return {
        // _b -> _b, _b -> rr
        {alphabet.symbolToNumber("bb"), alphabet.symbolToNumber("bb")},
        {alphabet.symbolToNumber("bb"), alphabet.symbolToNumber("gb")},
        {alphabet.symbolToNumber("bb"), alphabet.symbolToNumber("rr")},
        {alphabet.symbolToNumber("gb"), alphabet.symbolToNumber("bb")},
        {alphabet.symbolToNumber("gb"), alphabet.symbolToNumber("gb")},
        {alphabet.symbolToNumber("gb"), alphabet.symbolToNumber("rr")},
        // rr -> _g
        {alphabet.symbolToNumber("rr"), alphabet.symbolToNumber("bg")},
        {alphabet.symbolToNumber("rr"), alphabet.symbolToNumber("gg")},
        // _g -> _g
        {alphabet.symbolToNumber("bg"), alphabet.symbolToNumber("bg")},
        {alphabet.symbolToNumber("bg"), alphabet.symbolToNumber("gg")},
        {alphabet.symbolToNumber("gg"), alphabet.symbolToNumber("bg")},
        {alphabet.symbolToNumber("gg"), alphabet.symbolToNumber("gg")},
    };
}

}
}

ProblemInstance pigeonholeTest(int n, int m) {
    using namespace Pigeonhole;

    ProblemInstance instance(n, m + 2);

    for (int i = 0; i < n; i++) {
        instance.intermediateColors(i, 0) = leftBorderAlphabet();
        instance.finalColors(i, 0) = leftBorderAlphabet();
        instance.colorMap(i, 0) = borderColorMap();
        for (int j = 1; j <= m; j++) {
            instance.intermediateColors(i, j) = intermediateAlphabet();
            instance.finalColors(i, j) = intermediateAlphabet();
            instance.colorMap(i, j) = intermediateColorMap();
        }
        instance.intermediateColors(i, m + 1) = rightBorderAlphabet();
        instance.finalColors(i, m + 1) = rightBorderAlphabet();
        instance.colorMap(i, m + 1) = borderColorMap();
    }

    for (int i = 0; i < n - 1; i++) {
        instance.verticalConstraints(i, 0) = borderVerticalConstraints();
        for (int j = 1; j <= m; j++) {
            instance.verticalConstraints(i, j) = verticalConstraints();
        }
        instance.verticalConstraints(i, m + 1) = borderVerticalConstraints();
    }

    for (int i = 0; i < n; i++) {
        instance.horizontalConstraints(i, 0) = leftBorderHConstraints();
        for (int j = 1; j < m; j++) {
            instance.horizontalConstraints(i, j) = horizontalConstraints();
        }
        instance.horizontalConstraints(i, m) = rightBorderHConstraints();
    }

    return instance;
}

}
