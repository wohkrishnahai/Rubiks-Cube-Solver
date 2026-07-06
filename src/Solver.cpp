
#include "../include/Solver.h"
#include "../include/CornerIndexer.h"
#include "../include/EdgeIndexer.h"
#include <iostream>
#include <algorithm> // for max
using namespace std;

// The constructor must initialize the EdgePatternDatabase objects with their enums
Solver::Solver() : first6EdgesDB(EDGE_GROUP::FIRST_6), last6EdgesDB(EDGE_GROUP::LAST_6) {
  cout << "Loading Pattern Databases into RAM (~86.5 MB)..." << endl;

  if (!cornerDB.loadFromFile("corner_database.bin")) exit(1);
  if (!first6EdgesDB.loadFromFile("first_6_edges.bin")) exit(1);
  if (!last6EdgesDB.loadFromFile("last_6_edges.bin")) exit(1);

  cout << "All databases loaded successfully." << endl;
}

bool Solver::dfs(Bitboard& cube, uint8_t depth, uint8_t bound, MOVE lastMove) {
  // Calculate heuristics for all three databases
  uint8_t h_corners = cornerDB.getNumMoves(CornerIndexer::getIndex(cube));
  uint8_t h_first6 = first6EdgesDB.getNumMoves(EdgeIndexer::getFirst6EdgesIndex(cube));
  uint8_t h_last6 = last6EdgesDB.getNumMoves(EdgeIndexer::getLast6EdgesIndex(cube));

  // Take the maximum (Admissible Heuristic)
  uint8_t h_n = max({h_corners, h_first6, h_last6});

  // IDA* Pruning Condition: f(n) = g(n) + h(n)
  // If our current moves + estimated remaining moves exceeds our limit, cut the branch.
  if (depth + h_n > bound) return false;

  // Win Condition (All 3 must be 0)
  if (depth <= bound && h_n == 0) return true;

  // Standard array of all 18 moves
  MOVE moves[] = {
    MOVE::U, MOVE::UPRIME, MOVE::U2,
    MOVE::D, MOVE::DPRIME, MOVE::D2,
    MOVE::F, MOVE::FPRIME, MOVE::F2,
    MOVE::B, MOVE::BPRIME, MOVE::B2,
    MOVE::L, MOVE::LPRIME, MOVE::L2,
    MOVE::R, MOVE::RPRIME, MOVE::R2
  };

  // DFS Exploration
  for (int i = 0; i < 18; ++i) {
    MOVE nextMove = moves[i];

    // PRUNING OPTIMIZATIONS:
    // Do not undo the move just made (eg: U -> U')
    // (Write a cleaner Inverse Lookup later)

    // Apply move
    cube.move(nextMove);
    solution.push_back(nextMove);

    // Recursive DFS call
    if (dfs(cube, depth+1, bound, nextMove)) return true; // Solution found!

    // Backtrack (Undo the move)
    solution.pop_back();

    // To properly backtrack in our bitboard, we must apply the inverse move.
    // For now, doing the move 3 more times acts as an inverse (for 90-deg turns).
    if ((int)nextMove % 3 == 0) cube.move((MOVE)((int)nextMove +1)); // Clockwise
    else if ((int)nextMove % 3 == 1) cube.move((MOVE)((int)nextMove -1)); // Counter
    else cube.move(nextMove); // 180 degrees
  }

  return false;
}

vector<MOVE> Solver::solve(Bitboard cube) {
  solution.clear();
  
  uint8_t h_corners = cornerDB.getNumMoves(CornerIndexer::getIndex(cube));
  uint8_t h_first6 = first6EdgesDB.getNumMoves(EdgeIndexer::getFirst6EdgesIndex(cube));
  uint8_t h_last6 = last6EdgesDB.getNumMoves(EdgeIndexer::getLast6EdgesIndex(cube));

  uint8_t startBound = max({h_corners, h_first6, h_last6});

  cout << "Starting IDA* Search..." << endl;

  // Iteratively increase the depth bound.
  // If the corners are 'x' moves away, start the search limited to 'x' moves.
  for (uint8_t bound = startBound; bound <= 20; ++bound) {
    cout << "Searching depth bound: " << (int)bound << endl;

    // The initial 'lastMove' doesn't matter
    if (dfs(cube, 0, bound, MOVE::U)) {
      cout << "Solution Found!" << endl;
      return solution;
    }
  }

  return solution;
}

void Solver::printSolution(const vector<MOVE>& sol) {
  string moveNames[] = {
    "U", "U'", "U2", "D", "D'", "D2", 
    "F", "F'", "F2", "B", "B'", "B2", 
    "L", "L'", "L2", "R", "R'", "R2"
  };

  cout << "Solution (" << sol.size() << " moves): ";
  for (MOVE m : sol) {
    cout << moveNames[static_cast<int>(m)] << " ";
  }
  cout << endl;
}
