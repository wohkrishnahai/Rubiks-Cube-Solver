#include <iostream>
#include "../include/Bitboard.h"

using namespace std;

void runTest(string testName, Bitboard& cube) {
  cout << testName << ": ";
  if (cube.isSolved()) {
    cout << "PASS\n";
  } else {
    cout << "FAIL\n";
  }
}

int main() {
  cout << "=== TESTING ===\n\n";

  Bitboard uCube;
  uCube.u().uPrime();
  runTest("[TEST U] u() and uPrime()", uCube);

  Bitboard dCube;
  dCube.d().dPrime();
  runTest("[TEST D] d() and dPrime()", dCube);

  Bitboard fCube;
  fCube.f().fPrime();
  runTest("[TEST F] f() and fPrime()", fCube);

  Bitboard bCube;
  bCube.b().bPrime();
  runTest("[TEST B] b() and bPrime()", bCube);

  Bitboard lCube;
  lCube.l().lPrime();
  runTest("[TEST L] l() and lPrime()", lCube);

  Bitboard rCube;
  rCube.r().rPrime();
  runTest("[TEST R] r() and rPrime()", rCube);

  cout << "\n=== CYCLE TESTS ===\n";

  Bitboard fCycle;
  fCycle.f().f().f().f();
  runTest("[TEST F4] 4-Turn Cycle (F F F F)", fCycle);

  Bitboard bCycle;
  bCycle.b().b().b().b();
  runTest("[TEST B4] 4-Turn Cycle (B B B B)", bCycle);

  cout << "\n=============================\n";

  return 0;
}
