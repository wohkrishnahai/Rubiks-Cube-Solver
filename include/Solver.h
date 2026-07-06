#ifndef SOLVER_H
#define SOLVER_H

#include "Bitboard.h"
#include "CornerPatternDatabase.h"
#include <vector>
using namespace std;

class Solver {
private:
  CornerPatternDatabase cornerDB;
  vector<MOVE> solution;

  // The recursive DFS function for the IDA* Algorithm
  bool dfs(Bitboard& cube, uint8_t depth, uint8_t bound, MOVE lastMove);

public:
  Solver();

  // Returns the sequence of moves to solve the cube
  vector<MOVE> solve(Bitboard cube);

  // Helper to print the solution cleanly
  static void printSolution(const vector<MOVE>& sol);
};

#endif // SOLVER_H
