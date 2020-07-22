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
#include <ListColoring/Instances.h>

#include <iostream>
#include <cstring>

int pigeonhole(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "pigeonhole: Expected image sizes" << std::endl;
    }
    int n = std::stoi(argv[2]);
    int m = std::stoi(argv[3]);
    ListColoring::Legacy::write(std::cout, ListColoring::pigeonholeTest(n, m));
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Expected an instance name" << std::endl;
        return 2;
    }

    if (std::strcmp(argv[1], "pigeonhole") == 0) {
        return pigeonhole(argc, argv);
    }

    std::cerr << "Unknown instance name: " << argv[1] << std::endl;
    return 2;
}
