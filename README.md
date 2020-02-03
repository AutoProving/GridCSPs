# ListColoring

A List Coloring solver.

## Build instructions

External dependencies of the library are exported as git submodules in `ext/` subdirectory. Currently there is only one dependency:

* [Google Test](https://github.com/google/googletest/) framework is used for unit tests.

To clone the dependencies to supposed locations, just type the following command after cloning this repository.

```
$ git submodule update --init --recursive
```

[CMake](https://cmake.org/) build system is required to build the project.
After cloning the submodules, build the project with the following commands.

```
$ cd build/
$ cmake ..
$ make
```

The solver will be available as `./Solver`.

You can also run tests with the following command.

```
$ make test
```

## Usage

The solver takes an instance from stdin. You can find the format example in [this file](listColoringExample.tex).

If there's no solution, it prints "No solution".

Otherwise, it prints rows of space-separated final color names — the solution.

## Building documentation

The header files are provided with [Doxygen](http://www.doxygen.nl/) documentation.
To build it, install Doxygen and type

```
$ doxygen Doxyfile
```

The home page of documentation will be available at `doc/html/index.html`.

## Contributors

See the [Contributors.md](Contributors.md) file.
