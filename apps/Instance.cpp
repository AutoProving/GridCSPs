#include <ListColoring/ListColoring.h>
#include <ListColoring/LegacyWriter.h>
#include <ListColoring/Instances.h>

#include <iostream>

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
