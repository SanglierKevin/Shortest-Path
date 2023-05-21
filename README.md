# Projet 3 - Groupe Y2

# Table of contents
1. [Bellman-Ford](#bellman-ford)
2. [Run program](#run-program)
3. [Structure](#structure)
4. [Testing](#testing)
5. [Cleaning](#cleaning)

# Bellman-Ford
This program uses the Bellman-Ford shortest path algorithm in a binary graph that is directed, weighted and that can have negative weights.
This algorithm takes into account isolated nodes and detects negative cycles.

# Run program
To run the program, open a terminal and type the command `make`, this will create an executable named 'sp'. You can then type the following command :

   `./sp [-f 'output_file'] [-n 'n_threads'] [-v] 'input_file'`

   - 'input_file'         : (required) Path to the instance file representing the graph to process.
   - -f 'output_file'     : (optional) Path to the file that will contain the program result (.txt/.bin/.ntf).   Default : stdout.
   - -n 'n_threads'       : (optional) Number of computation threads executing the shortest path algorithm.   Default : 4.
   - -v                   : (optional) Allow debug messages. If this is not enabled, no debug messages will be displayed, except error messages in case of failure.   Default : false.

If you need help in the terminal, you can type `./sp -h` or `./sp --help`.

# Structure
## Folders
| Folder | Content |
| --- | --- |
| include | All '.h' files with all structure and function declarations used in the program |
| src | All '.c' files in which our functions are implemented |
| tests | All the files used to run our tests |
| tests/correct_out | All output files that come from the python code |
| tests/in | All input files that are used for testing |
| tests/include | All '.h' files with all includes, structure and function declarations used in the tests |
| tests/out | All output files generated by the tests |
| tests/src | All '.c' files in which our test functions are implemented |

## Files
| File | Content |
| --- | --- |
| graph.h | Declarations of graph_t, thread_t, read_graph() and read_infos() |
| helper.h | Declarations of dp, cn, path_t, bellman_ford(), get_max() and get_path() |
| portable_endian.h | File for MacOS compatibility |
| portable_semaphore.h | File for MacOS compatibility |
| small_instance.h | A small graph instance |
| graph.c | Implementations of read_infos() and read_graph() |
| helper.c | Implementations of bellman_ford(), get_max() and get_path() |
| sp.c | The main '.c' file with the declarations of args_t and sourceArg_t and the implementations of usage(), parse_args(), sourceMain() and main() |
| tests/test.c | The main ".c' file for our tests with the declaration of main() which launch all the tests |
| Makefile | A file to manage our executables and commands to run our program and our tests |
| README.md | A magic file that contains the answers to almost all your questions |

# Testing
You can test several things with the following commands :

- `make` + `make tests` : Verification of the correct operation of all our functions with limit cases.
- `make` + `make valgrind` : Checking for memory leaks with [Valgrind](https://valgrind.org/).
- `make cppcheck` : Checking bugs and syntax errors with [Cppcheck](https://cppcheck.sourceforge.io/).
- `make gcov` : Will create a web site containing information such as the number of times each piece of code has been executed, which pieces we never get into and more with [Gcov](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html). We used this command to see which parts of our code needed to be optimized or threaded.

# Cleaning
The command `make clean` will clean all the files created by the commands `make`, `./sp`, `make tests`, `make valgrind`, `make cppcheck` and `make gcov`.