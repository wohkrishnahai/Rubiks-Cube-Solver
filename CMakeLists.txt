cmake_minimum_required(VERSION 3.28)
project(Rubiks-Cube-Solver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Ensure maximum optimization for the solver engine
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

# Default to Release mode if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# All source files
set(SOURCES
  src/main.cpp
  src/Bitboard.cpp
  src/MathUtils.cpp
  src/CornerIndexer.cpp
  src/EdgeIndexer.cpp
  src/PatternDatabase.cpp
  src/CornerPatternDatabase.cpp
  src/EdgePatternDatabase.cpp
  src/Solver.cpp
  src/Scrambler.cpp
)

# Create the executable named 'solver'
add_executable(solver ${SOURCES})
