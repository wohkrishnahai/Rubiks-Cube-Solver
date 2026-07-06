#include <iostream>
#include <vector>
#include <string>
#include "../include/Bitboard.h"
#include "../include/Solver.h"
#include "../include/Scrambler.h"
using namespace std;

int main() {
  cout << "=== FULL RUBIK'S CUBE SOLVER ===" << endl << endl;

  int choice;
  cout << "Select mode:" << endl;
  cout << "1. Random scramble" << endl;
  cout << "2. Enter custom scramble" << endl;
  cout << "Choice: ";

  if (!(cin >> choice)) {
    cout << "Invalid input. Exiting..." << endl;
    return 1;
  }

  vector<MOVE> scramble;

  if (choice == 1) {
    int length;
    cout << "Enter scramble length (eg: 20): ";
  
    // Error handling if the user types a letter or a negative number
    if (!(cin >> length) || length <= 0) {
      cout << "Invalid length detected. Defaulting to 20 moves." << endl;
      cin.clear(); // Clear the error flag
      cin.ignore(10000, '\n'); // Discard bad input
      length = 20;
    }

    scramble = Scrambler::generateScramble(length);
    cout << "\nGenerated Scramble (" << length << " moves): ";
    Solver::printSolution(scramble);
  } 
  else if (choice == 2) {
    cin.ignore(); // Clear the newline character from the input buffer
    string customScramble;
    cout << "Enter scramble (eg: U R F' D2): ";
    getline(cin, customScramble);

    try {
      scramble = Scrambler::parseScramble(customScramble);
    } catch (const exception& e) {
      cout << "Error: " << e.what() << endl;
      return 1;
    }
  } 
  else {
    cout << "Invalid choice. Exiting." << endl;
    return 1;
  }

  Bitboard cube;

  // Apply the scramble to the cube
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
