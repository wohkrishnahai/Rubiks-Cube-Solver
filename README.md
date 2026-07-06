# Rubik's Cube IDA* Solver

## Overview

A hyper-optimized, C++ based algorithmic engine that solves any standard 3x3x3 Rubik's Cube optimally (in 20 moves or fewer). 

This project is a heavily modernized, bitboard-optimized implementation of **Korf's Algorithm**, first introduced by Richard Korf in his seminal 1997 paper *"Finding Optimal Solutions to Rubik's Cube Using Pattern Databases"*. 

The Rubik's Cube state space contains over $4.3 \times 10^{19}$ possible configurations, making brute-force search impossible. Following Korf's methodology, this engine reduces that complexity by pre-computing heuristic Pattern Databases and proving the optimal path using Iterative Deepening A* (IDA*) search. We take this a step further by mapping the 3D physical puzzle into single-cycle 64-bit CPU instructions for maximum performance.

## Features

* **Optimal Solving:** Guaranteed to find a solution in 20 moves or fewer (God's Number).
* **High-Performance Bitboards:** Represents the state space using 64-bit integer bitwise operations for maximum CPU efficiency.
* **Aggressive Search Pruning:** Implements same-face and parallel-face pruning during IDA* traversal to eliminate redundant branches instantly.
* **Perfect Hashing Compression:** Compresses permutations into gapless 1D arrays using Lehmer codes and factorial bases.
* **WCA-Compliant Scrambler:** Built-in random scramble generator that adheres to World Cube Association pruning rules.
* **Microsecond Profiling:** Built-in `<chrono>` benchmarking to accurately measure search algorithms independent of I/O load times.
* **Automation Ready:** Single-execution CLI design, perfect for bash scripting and standard input/output redirection.

## Core Architecture & Mathematics

### 1. Bitboard Representation
Traditional cube solvers often represent the puzzle using massive 3D arrays or complex object structures, resulting in severe overhead. This engine maps the entire state of the cube down to 64-bit integers. Every twist of a face is reduced to highly efficient bitwise operations, allowing the search tree to generate millions of nodes per second while maximizing CPU cache efficiency.

### 2. Perfect Hashing & Lehmer Codes
To navigate the search space, the engine relies on Pattern Databases (PDBs) that act as heuristic lookups. However, storing every permutation would require over 490 GB of RAM. To bypass this, the puzzle is mathematically split into smaller subsets (Corners, First 6 Edges, Last 6 Edges). We generate **Perfect Hashing Functions** for these subsets using:
* **Lehmer Codes:** To convert permutations into mathematical ranks.
* **Factorial Bases:** To compress these coordinates down to dense, gapless 1D array indexes.

By taking the `max()` of these heuristic tables, the engine maintains perfect mathematical admissibility while fitting entirely within ~86.5 MB of standard RAM.

### 3. IDA* Search & Pruning Optimizations
The engine traverses the search tree using an Iterative Deepening A* (IDA*) algorithm strictly bounded by God's Number (20). To prevent exponential explosion, the engine aggressively prunes dead branches mathematically:
* **Same-Face Pruning:** Rejects redundant moves like `U` followed by `U2`.
* **Parallel-Face Pruning:** The Front (`F`) and Back (`B`) faces do not intersect. The engine forces an alphabetical order on parallel face turns, explicitly rejecting branches like `B F'` in favor of `F' B`. This eliminates millions of redundant tree traversals instantly.

## Project Structure

```text
├── CMakeLists.txt                # CMake build configuration
├── README.md                     # Project documentation
├── corner_database.bin           # Pre-computed Corner Pattern Database
├── first_6_edges.bin             # Pre-computed First 6 Edges Pattern Database
├── last_6_edges.bin              # Pre-computed Last 6 Edges Pattern Database
├── build/                        # Compiled binaries and CMake artifacts (generated)
├── include/                      # Header files
│   ├── Bitboard.h                # 64-bit state representation and bitwise moves
│   ├── CornerIndexer.h           # Mathematical indexing for corner subsets
│   ├── CornerPatternDatabase.h   # Corner subset database loading and lookup
│   ├── EdgeIndexer.h             # Mathematical indexing for edge subsets
│   ├── EdgePatternDatabase.h     # Edge subset database loading and lookup
│   ├── GenericCube.h             # Base definitions and interface for cube states
│   ├── MathUtils.h               # Factorial arrays, Lehmer codes, and combinatorics
│   ├── PatternDatabase.h         # Base class for DB file I/O and heuristic lookups
│   ├── Scrambler.h               # WCA scramble generator and string parser
│   └── Solver.h                  # IDA* search, pruning, and heuristic calculation
└── src/                          # Source implementation
    ├── Bitboard.cpp              # Bitwise masking and state transition logic
    ├── CornerIndexer.cpp         # Implementation of corner indexing algorithms
    ├── CornerPatternDatabase.cpp # Corner database generation and memory mapping
    ├── EdgeIndexer.cpp           # Implementation of edge indexing algorithms
    ├── EdgePatternDatabase.cpp   # Edge database generation and memory mapping
    ├── MathUtils.cpp             # Math helper function implementations
    ├── PatternDatabase.cpp       # File streaming and memory allocation
    ├── Scrambler.cpp             # Randomization and WCA compliance rules
    ├── Solver.cpp                # Depth-First Search and IDA* loop logic
    └── main.cpp                  # CLI interface, user input, and initialization
```

## Requirements

* **Compiler:** A C++ compiler with strict C++17 support (e.g., GCC 9+, Clang 10+, or MSVC).
* **Build System:** CMake version 3.28 or higher.
* **Memory:** Minimum ~100 MB of free RAM available to load the pre-computed Pattern Database `.bin` files into memory during execution.

## Build Instructions

This project is strictly configured for Release mode (`-O3` optimization) to ensure maximum search speeds.

```bash
# 1. Clone the repository
git clone https://github.com/wohkrishnahai/Rubiks-Cube-Solver.git
cd Rubiks-Cube-Solver

# 2. Generate build files
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 3. Compile the engine
cmake --build build
```

## Usage

The executable is designed for clean, single-execution behavior. Run the solver from the root directory:

```bash
./build/solver
```

Upon execution, the engine loads the Pattern Databases directly into RAM, then prompts you with an interactive CLI menu:

```text
=== FULL RUBIK'S CUBE IDA* SOLVER ===

Select mode:
1. Random scramble
2. Enter custom scramble
0. Exit
Choice: 
```

* **`1` - Random Scramble:** Prompts you for a scramble length (e.g., `20`). The scrambler generates a mathematically clean, non-redundant scramble state.
* **`2` - Custom Scramble:** Input standard WCA cube notation (e.g., `U R F' L2 D'`). The engine validates the string, applies it to the bitboard, and calculates the optimal reverse path to the solved state.
* **`0` - Exit:** Cleanly terminates the program.

### Automation & I/O Redirection
Because the CLI menu terminates cleanly after a single solve, the engine is fully compatible with standard bash scripting and file redirection for batch testing:

```bash
# Feed a custom scramble directly via echo
echo -e "2\nU R F' L2 D'\n" | ./build/solver

# Run a batch of test cases from a text file
./build/solver < test_cases.txt > results.txt
```

## Performance

The `Solver` class features built-in `std::chrono` high-resolution profiling. It isolates the pure search time from the database initialization overhead. Solve times scale exponentially with the depth of the scramble:

* **0–12 moves:** Solved near-instantaneously (`0 ms` - `5 ms`) due to perfect heuristic lookups.
* **13–15 moves:** Typically solved in `< 200 ms`.
* **16–20 moves:** Deep traversal states. Depending on the exact permutation and CPU hardware, these maximum-depth states can take anywhere from a few hundred milliseconds to a few seconds.

## Limitations

* **Memory Initialization Overhead:** Because the solver is designed for single-execution, the ~86.5 MB `.bin` databases must be loaded from the disk into RAM every time the executable runs. This adds a minor startup delay (~100-200ms) before the search begins.
* **Single-Threaded Search:** The IDA* depth-first search currently runs on a single CPU core. It does not map sub-trees to parallel threads.

## Planned Improvements

* **Nodes Per Second (NPS) Tracking:** Implement an internal node counter within the `dfs` recursive loop to output raw engine speed (nodes evaluated per second).
* **CLI Argument Parsing:** Bypass the interactive menu entirely by accepting scrambles via standard `argc`/`argv` inputs (e.g., `./solver --scramble "U R F'"`).
* **Multi-threading:** Parallelize the initial depth branches of the IDA* search to utilize modern multi-core processors for deep 18–20 move states.

