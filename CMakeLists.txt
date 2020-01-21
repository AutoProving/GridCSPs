cmake_minimum_required(VERSION 3.10)

project(ListColoring)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(ODDs_DISABLE_TESTS ON)
add_subdirectory(ext/ODDs)

add_library(ListColoring STATIC
    lib/ListColoring/ListColoring.cpp
    lib/ListColoring/LegacyWriter.cpp
    lib/ListColoring/LegacyReader.cpp)
target_include_directories(ListColoring PUBLIC lib/)
target_include_directories(ListColoring PUBLIC ext/ODDs/lib/)
target_include_directories(ListColoring PUBLIC ext/boost/spirit/include)
target_include_directories(ListColoring PUBLIC ext/boost/mpl/include)
target_include_directories(ListColoring PUBLIC ext/boost/config/include)
target_include_directories(ListColoring PUBLIC ext/boost/type_traits/include)
target_include_directories(ListColoring PUBLIC ext/boost/core/include)
target_include_directories(ListColoring PUBLIC ext/boost/preprocessor/include)
target_include_directories(ListColoring PUBLIC ext/boost/assert/include)
target_include_directories(ListColoring PUBLIC ext/boost/fusion/include)
target_include_directories(ListColoring PUBLIC ext/boost/static_assert/include)
target_include_directories(ListColoring PUBLIC ext/boost/utility/include)
target_include_directories(ListColoring PUBLIC ext/boost/variant/include)
target_include_directories(ListColoring PUBLIC ext/boost/type_index/include)
target_include_directories(ListColoring PUBLIC ext/boost/container_hash/include)
target_include_directories(ListColoring PUBLIC ext/boost/throw_exception/include)
target_include_directories(ListColoring PUBLIC ext/boost/integer/include)
target_include_directories(ListColoring PUBLIC ext/boost/container/include)
target_compile_options(ListColoring PRIVATE -Wall -Wextra -Wshadow)
target_link_libraries(ListColoring PUBLIC ODDs)

if (NOT LIST_COLORING_DISABLE_TESTS)
    add_subdirectory(ext/googletest)
    enable_testing()

    include(GoogleTest)

    add_executable(ListColoringTest
        test/Instances/Instances.cpp
        test/ListColoringClass.cpp
        test/LegacyIO.cpp)
    target_include_directories(ListColoringTest PUBLIC lib/)
    target_include_directories(ListColoringTest PUBLIC test/)
    target_include_directories(ListColoringTest PUBLIC ext/ODDs/lib/)
    target_include_directories(ListColoringTest PUBLIC ext/googletest/googletest/include/)
    target_link_libraries(ListColoringTest gtest gtest_main)
    target_link_libraries(ListColoringTest ListColoring)
    target_compile_options(ListColoringTest PRIVATE -Wall -Wextra -Wshadow)
    gtest_discover_tests(ListColoringTest)
endif()
