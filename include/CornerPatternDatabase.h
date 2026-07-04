#ifndef CORNER_PATTERN_DATABASE_H
#define CORNER_PATTERN_DATABASE_H

#include "PatternDatabase.h"
#include "Bitboard.h"

class CornerPatternDatabase : public PatternDatabase {
public:
  // The exact number of corner permutations: 8! * 3^7 = 88,179,840
  CornerPatternDatabase();

  // BFS Algorithm to populate the 44MB array
  void generateDatabase();
};

#endif // CORNER_PATTERN_DATABASE_H
