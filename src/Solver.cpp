
#include "../include/Solver.h"
#include "../include/CornerIndexer.h"
#include "../include/EdgeIndexer.h"
#include <iostream>
#include <chrono>
using namespace std;

// The constructor must initialize the EdgePatternDatabase objects with their enums
Solver::Solver() : first6EdgesDB(EDGE_GROUP::FIRST_6), last6EdgesDB(EDGE_GROUP::LAST_6) {
  cout << "Loading Pattern Databases into RAM (~86.5 MB)..." << endl;

  if (!cornerDB.loadFromFile("corner_database.bin")) exit(1);
  if (!first6EdgesDB.loadFromFile("first_6_edges.bin")) exit(1);
  if (!last6EdgesDB.loadFromFile("last_6_edges.bin")) exit(1);

  cout << "All databases loaded successfully." << endl;
}

bool Solver::dfs(Bitboard cube, uint8_t depth, uint8_t bound, MOVE lastMove) {
  // Calculate heuristics for all three databases
  // IDA* Pruning Condition: f(n) = g(n) + h(n)
  // Optimization: If our current moves + estimated remaining moves exceeds our limit, cut the branch.

  uint8_t h_corners = cornerDB.getNumMoves(CornerIndexer::getIndex(cube));
  if (depth + h_corners > bound) return false;

  uint8_t h_first6 = first6EdgesDB.getNumMoves(EdgeIndexer::getFirst6EdgesIndex(cube));
  if (depth + h_first6 > bound) return false;

  uint8_t h_last6 = last6EdgesDB.getNumMoves(EdgeIndexer::getLast6EdgesIndex(cube));
  if (depth + h_last6 > bound) return false;

  // Win Condition (All 3 must be 0)
  if (depth <= bound && h_corners == 0 && h_first6 == 0 && h_last6 == 0) return true;

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
    if (!solution.empty()) {
      MOVE lastMoveMade = solution.back();

      // Every face has 3 moves (eg: U, U', U2). 
      // Dividing by 3 groups them by face: 0=U, 1=D, 2=F, 3=B, 4=L, 5=R
      int nextFace = (int)nextMove / 3;
      int lastFace = (int)lastMoveMade / 3;

      // Same Face Pruning: Don't turn the same face twice in a row.
      // (eg: U followed by U' is redundant)
      if (nextFace == lastFace) continue;

      // Parallel Face Pruning (Commutative moves)
      // U and D are parallel. L and R are parallel. F and B are parallel.
      // Turning U then D is the exact same as turning D then U.
      // We enforce an arbitrary order (eg: only allow U then D, ignore D then U) to cut parallel branches in half.
      if (lastFace % 2 == 1 && nextFace == lastFace -1) continue;
    }

    // Optimization- Pass by Value (no backtracking needed)
    Bitboard nextCube = cube;

    // Apply move
    nextCube.move(nextMove);
    solution.push_back(nextMove);

    // Recursive DFS call
    if (dfs(nextCube, depth+1, bound, nextMove)) return true; // Solution found!

    // Backtrack (Undo the move)
    solution.pop_back();
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
  // Start the timer exactly when the algorithm begins
  auto start = chrono::high_resolution_clock::now();

  // Iteratively increase the depth bound.
  // If the corners are 'x' moves away, start the search limited to 'x' moves.
  for (uint8_t bound = startBound; bound <= 20; ++bound) {
    cout << "Searching depth bound: " << (int)bound << endl;

    // The initial 'lastMove' doesn't matter
    if (dfs(cube, 0, bound, MOVE::U)) {
      // Stop the timer the exact microsecond a solution is found
      auto stop = chrono::high_resolution_clock::now();
      auto duration = chrono::duration_cast<chrono::milliseconds>(stop - start);

      cout << "Solution Found!" << endl;
      cout << "Search Time: " << duration.count() << " ms (" 
        << duration.count() / 1000.0 << " seconds)" << endl;

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
