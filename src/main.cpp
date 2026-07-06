#include <iostream>
#include <vector>
#include "../include/Bitboard.h"
#include "../include/Solver.h"
using namespace std;

int main() {
  cout << "=== FULL RUBIK'S CUBE IDA* SOLVER ===" << endl << endl;
  Bitboard cube;

  // Scramble the cube
  cout << "Scrambling the cube: U R F D L B" << endl;
  cube.u().r().f().d().l().b();

  // Initialize the Solver (Loads all 3 databases)
  Solver solver;

  cout << "\nInitiating Search..." << endl;
  vector<MOVE> solution = solver.solve(cube);

  cout << "\n=== SEARCH COMPLETE ===" << endl;
  Solver::printSolution(solution);
  cout << "=======================" << endl;

  return 0;
}
