#include "../include/EdgePatternDatabase.h"
#include "../include/EdgeIndexer.h"
#include <queue>
#include <iostream>
using namespace std;

EdgePatternDatabase::EdgePatternDatabase(EDGE_GROUP group) 
  : PatternDatabase(42577920), group(group) {}

void EdgePatternDatabase::generateDatabase() {
  Bitboard solvedCube;
  uint32_t solvedIndex;

  if (group == EDGE_GROUP::FIRST_6) {
    solvedIndex = EdgeIndexer::getFirst6EdgesIndex(solvedCube);
  } else {
    solvedIndex = EdgeIndexer::getLast6EdgesIndex(solvedCube);
  }

  // Set the solved state to exactly 0 moves
  setNumMoves(solvedIndex, 0);

  queue<Bitboard> q;
  q.push(solvedCube);

  int nodesVisited = 1;
  uint8_t currentDepth = 0;

  cout << "Starting BFS for " 
    << (group == EDGE_GROUP::FIRST_6 ? "First 6 Edges" : "Last 6 Edges") 
    << "..." << endl;
  cout << "Total States to map: 42,577,920" << endl << endl;

  while (!q.empty()) {
    Bitboard currentCube = q.front();
    q.pop();

    uint32_t currentIndex;
    if (group == EDGE_GROUP::FIRST_6) {
      currentIndex = EdgeIndexer::getFirst6EdgesIndex(currentCube);
    } else {
      currentIndex = EdgeIndexer::getLast6EdgesIndex(currentCube);
    }

    uint8_t currentMoves = getNumMoves(currentIndex);

    if (currentMoves > currentDepth) {
      currentDepth = currentMoves;
      cout << "Reached Depth: " << (int)currentDepth 
        << " | Nodes Mapped: " << nodesVisited << endl;
    }

    MOVE moves[] = {
      MOVE::U, MOVE::UPRIME, MOVE::U2,
      MOVE::D, MOVE::DPRIME, MOVE::D2,
      MOVE::F, MOVE::FPRIME, MOVE::F2,
      MOVE::B, MOVE::BPRIME, MOVE::B2,
      MOVE::L, MOVE::LPRIME, MOVE::L2,
      MOVE::R, MOVE::RPRIME, MOVE::R2
    };

    for (int i = 0; i < 18; ++i) {
      Bitboard nextCube = currentCube;
      nextCube.move(moves[i]);

      uint32_t nextIndex;
      if (group == EDGE_GROUP::FIRST_6) {
        nextIndex = EdgeIndexer::getFirst6EdgesIndex(nextCube);
      } else {
        nextIndex = EdgeIndexer::getLast6EdgesIndex(nextCube);
      }

      // If the state is unvisited (0x0F / 15)
      if (getNumMoves(nextIndex) == 0x0F) {
        setNumMoves(nextIndex, currentMoves +1);
        q.push(nextCube);
        nodesVisited++;
      }
    }
  }

  cout << "\nDatabase Generation Complete!" << endl;
  cout << "Total Nodes Successfully Mapped: " << nodesVisited << endl;
}
