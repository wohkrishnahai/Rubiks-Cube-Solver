#include <iostream>
#include "../include/Bitboard.h"
#include "../include/CornerIndexer.h"
using namespace std;

int main() {
  cout << "=== Perfect Hashing TESTING ===\n\n";
  Bitboard cube;

  // A solved cube has zero inversions and zero twists.
  uint32_t solvedIndex = CornerIndexer::getIndex(cube);
  cout << "Solved Cube Index: " << solvedIndex << " (Expected: 0)\n";

  // Scramble it
  cube.u().r().f();
  uint32_t scrambledIndex = CornerIndexer::getIndex(cube);
  cout << "Scrambled Cube Index: " << scrambledIndex << "\n";

  // Verify it's within bounds
  if (scrambledIndex > 0 && scrambledIndex < 88179840) {
    cout << "\nPASS: Hash is within valid database bounds!\n";
  } 
  else {
    cout << "\nFAIL: Hash out of bounds.\n";
  }

  cout << "\n=============================\n";

  return 0;
}
