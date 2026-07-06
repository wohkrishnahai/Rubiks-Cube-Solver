#include "../include/Solver.h"
#include "../include/CornerIndexer.h"
#include <iostream>
using namespace std;

Solver::Solver() {
  // Load the 44MB file already precomputed into RAM
  cout << "Loading Pattern Database into RAM..." << endl;
  if (!cornerDB.loadFromFile("corner_database.bin")) {
    cerr << "FATAL ERROR: Could not find corner_database.bin!" << endl;
    exit(1);
  }
  cout << "Database loaded successfully." << endl;
}

bool Solver::dfs(Bitboard& cube, uint8_t depth, uint8_t bound, MOVE lastMove) {
  // Calculate h(n) - The estimated cost to solve the corners
  uint32_t cornerIndex = CornerIndexer::getIndex(cube);
  uint8_t h_n = cornerDB.getNumMoves(cornerIndex);

  // IDA* Pruning Condition: f(n) = g(n) + h(n)
  // If our current moves + estimated remaining moves exceeds our limit, cut the branch.
  if (depth + h_n > bound) return false;

  // Win Condition (Since we only have corners right now, just check if corners are solved)
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
    if ((int)nextMove % 3 == 0) { // Clockwise
      cube.move((MOVE)((int)nextMove + 1)); 
    } else if ((int)nextMove % 3 == 1) { // Counter
      cube.move((MOVE)((int)nextMove - 1));
    } else { // 180 degrees
      cube.move(nextMove);
    }
  }

  return false;
}

vector<MOVE> Solver::solve(Bitboard cube) {
  solution.clear();

  uint32_t initialIndex = CornerIndexer::getIndex(cube);
  uint8_t startBound = cornerDB.getNumMoves(initialIndex);

  cout << "Starting IDA* Search..." << endl;

  // Iteratively increase the depth bound.
  // If the corners are 7 moves away, start the search limited to 7 moves.
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
