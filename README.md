# GridCSP 

This is a solver for Grid-like constraint satisfaction problems (CSPs). The solver works in polynomial time on the smoothness of the input instance. Smoothness is a complexity measure defined in terms of ordered decision diagrams (ODDs). For more details please refer to the following publication. 

Vasily Alferov and Mateus de Oliveira Oliveira. On the Satisfiability of Smooth Grid CSPs.
Proceedings of the 20th Symposium on Experimental Algorithms (SEA 2022).


## Build instructions

External dependencies of the library are exported as git submodules in `ext/` subdirectory.

* [ODDs](https://github.com/mateusoliveiraoliveira/ODDs) is used by solver, as it heavily exploits Ordered Decision Diagrams.

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

After that, it prints the statistics: width of ODDs used in each row of the intstance.

The solver also accepts command line arguments:

* If `--quiet` or `-q` flag is passed, the solver only prints the solution without the statistics.
* If `--reduce-space` or `-r` flag is passed, the solver attempts to reduce the space consumption (might be reasonable for almost-square images)

### Disk mode

In disk mode, the solver saves intermediate ODDs to the specified directory instead of storing them in memory. This way, it consumes linear memory instead of cubic, but works 5-10 times slower (tested on a MacBook with SSD).

In order to enable disk mode one must provide an empty directory to store the execution data:

```
$ ./Solver -d /tmp/solver < instance.txt
```

If the execution in disk mode was interrupted in the middle, it can continue from the point of interruption, if flag  `-c` or `--continue` is passed.

```
$ ./Solver -c -d /tmp/solver < instance.txt
Continuing interrupted execution from row 22
```

## Building documentation

The header files are provided with [Doxygen](http://www.doxygen.nl/) documentation.
To build it, install Doxygen and type

```
$ doxygen Doxyfile
```

The home page of documentation will be available at `doc/html/index.html`.

## Contributors

See the [Contributors.md](Contributors.md) file.
