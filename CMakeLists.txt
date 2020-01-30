cmake_minimum_required(VERSION 3.10)

project(ListColoring)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ODDs_DISABLE_TESTS ON)
add_subdirectory(ext/ODDs)

add_library(ListColoring STATIC
    lib/ListColoring/ListColoring.cpp
    lib/ListColoring/LegacyWriter.cpp
    lib/ListColoring/LegacyReader.cpp
    lib/ListColoring/Solver.cpp)
target_include_directories(ListColoring PUBLIC lib/)
target_include_directories(ListColoring PUBLIC ext/ODDs/lib/)
target_compile_options(ListColoring PRIVATE -Wall -Wextra -Wshadow)
target_link_libraries(ListColoring PUBLIC ODDs)

add_executable(IsThereSolution apps/IsThereSolution.cpp)
target_include_directories(IsThereSolution PUBLIC lib/)
target_compile_options(IsThereSolution PRIVATE -Wall -Wextra -Wshadow)
target_link_libraries(IsThereSolution PUBLIC ListColoring)

add_executable(Solver apps/Solver.cpp)
target_include_directories(Solver PUBLIC lib/)
target_compile_options(Solver PRIVATE -Wall -Wextra -Wshadow)
target_link_libraries(Solver PUBLIC ListColoring)

if (NOT LIST_COLORING_DISABLE_TESTS)
    add_subdirectory(ext/googletest)
    enable_testing()

    include(GoogleTest)

    add_executable(ListColoringTest
        test/Instances/Instances.cpp
        test/ListColoringClass.cpp
        test/LegacyIO.cpp
        test/Solver.cpp)
    target_include_directories(ListColoringTest PUBLIC lib/)
    target_include_directories(ListColoringTest PUBLIC test/)
    target_include_directories(ListColoringTest PUBLIC ext/ODDs/lib/)
    target_include_directories(ListColoringTest PUBLIC ext/googletest/googletest/include/)
    target_link_libraries(ListColoringTest gtest gtest_main)
    target_link_libraries(ListColoringTest ListColoring)
    target_compile_options(ListColoringTest PRIVATE -Wall -Wextra -Wshadow)
    target_compile_definitions(ListColoringTest PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/res")
    gtest_discover_tests(ListColoringTest)
endif()
