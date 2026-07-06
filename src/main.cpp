#include <iostream>
#include <vector>
#include "../include/Bitboard.h"
#include "../include/Solver.h"
using namespace std;

int main() {
  cout << "=== FULL RUBIK'S CUBE SOLVER ===" << endl << endl;
  Bitboard cube;

  // A 15-move scramble
  MOVE scramble[] = {
    MOVE::F, MOVE::U, MOVE::FPRIME, MOVE::L, MOVE::D, 
    MOVE::RPRIME, MOVE::B, MOVE::LPRIME, MOVE::UPRIME, MOVE::R, 
    MOVE::DPRIME, MOVE::F, MOVE::BPRIME, MOVE::R, MOVE::U
  };

  cout << "Scrambling the cube (15 moves): F U F' L D R' B L' U' R D' F B' R U" << endl;
  for (MOVE m : scramble) {
    cube.move(m);
  }


  // Initialize the Solver (Loads all 3 databases)
  Solver solver;

  cout << "\nInitiating Search..." << endl;
  vector<MOVE> solution = solver.solve(cube);

  cout << "\n=== SEARCH COMPLETE ===" << endl;
  Solver::printSolution(solution);
  cout << "=======================" << endl;

  return 0;
}
