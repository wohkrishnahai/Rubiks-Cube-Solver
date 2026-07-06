#ifndef SCRAMBLER_H
#define SCRAMBLER_H

#include "Bitboard.h"
#include <vector>
#include <string>
using namespace std;

class Scrambler {
public:
  // Generates a WCA-compliant random scramble of given length
  static vector<MOVE> generateScramble(int length);

  // Parses a string of space-separated moves into a vector of enums
  static vector<MOVE> parseScramble(const string& scrambleStr);
};

#endif // SCRAMBLER_H
