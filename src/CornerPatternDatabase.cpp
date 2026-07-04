#include "../include/CornerPatternDatabase.h"
#include "../include/CornerIndexer.h"
#include <queue>
#include <iostream>
using namespace std;

// Initialize the base PatternDatabase with 88,179,840 states
CornerPatternDatabase::CornerPatternDatabase() : PatternDatabase(88179840) {}

void CornerPatternDatabase::generateDatabase() {
  Bitboard solvedCube;
  uint32_t solvedIndex = CornerIndexer::getIndex(solvedCube);

  // Set the solved state to exactly 0 moves
  setNumMoves(solvedIndex, 0);

  // Standard BFS Queue
  queue<Bitboard> q;
  q.push(solvedCube);

  int nodesVisited = 1;
  uint8_t currentDepth = 0;

  cout << "Starting BFS Database Generation. This will take a few minutes...\n";
  cout << "Total States to map: 88,179,840\n\n";

  while (!q.empty()) {
    Bitboard currentCube = q.front();
    q.pop();

    uint32_t currentIndex = CornerIndexer::getIndex(currentCube);
    uint8_t currentMoves = getNumMoves(currentIndex);

    // Print progress every time the wavefront hits a new depth layer
    if (currentMoves > currentDepth) {
      currentDepth = currentMoves;
      cout << "Reached Depth: " << (int)currentDepth 
        << " | Nodes Mapped: " << nodesVisited << "\n";
    }

    // 11 is the mathematical maximum number of moves to solve any corner state.
    // We can stop searching deeper once we hit this layer.
    if (currentMoves == 11) continue;

    // An array of all 18 possible moves
    MOVE moves[] = {
      MOVE::U, MOVE::UPRIME, MOVE::U2,
      MOVE::D, MOVE::DPRIME, MOVE::D2,
      MOVE::F, MOVE::FPRIME, MOVE::F2,
      MOVE::B, MOVE::BPRIME, MOVE::B2,
      MOVE::L, MOVE::LPRIME, MOVE::L2,
      MOVE::R, MOVE::RPRIME, MOVE::R2
    };

    // Explore all 18 branches from the current node
    for (int i = 0; i < 18; ++i) {
      Bitboard nextCube = currentCube; // Create a copy
      nextCube.move(moves[i]);         // Apply the move

      uint32_t nextIndex = CornerIndexer::getIndex(nextCube);

      // 0x0F (15) is the "unvisited" placeholder. 
      // If it is 15, we have found a new shortest path to this state!
      if (getNumMoves(nextIndex) == 0x0F) {
        setNumMoves(nextIndex, currentMoves +1);
        q.push(nextCube);
        nodesVisited++;
      }
    }
  }

  cout << "\nDatabase Generation Complete!\n";
  cout << "Total Nodes Successfully Mapped: " << nodesVisited << "\n";
}
