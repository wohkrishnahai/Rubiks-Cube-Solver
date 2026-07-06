#include <iostream>
#include <vector>
#include "../include/Bitboard.h"
#include "../include/Solver.h"
using namespace std;

int main() {
    cout << "=== IDA* SOLVER DIAGNOSTICS ===" << endl << endl;

    // Initialize a fresh, solved cube
    Bitboard cube;

    // Apply a scramble
    cout << "Scrambling the cube: U R F D L B" << endl;
    cube.u().r().f().d().l().b();

    // Initialize the Solver 
    // (Automatically loads corner_database.bin into RAM)
    Solver solver;

    // Run the IDA* search
    cout << "\nInitiating Search..." << endl;
    vector<MOVE> solution = solver.solve(cube);

    // Print the optimal move sequence
    cout << "\n=== SEARCH COMPLETE ===" << endl;
    Solver::printSolution(solution);
    cout << "=======================" << endl;

    return 0;
}

