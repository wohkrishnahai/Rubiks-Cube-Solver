#ifndef EDGE_PATTERN_DATABASE_H
#define EDGE_PATTERN_DATABASE_H

#include "PatternDatabase.h"
#include "Bitboard.h"

// Enum to tell BFS which set of edges to calculate
enum class EDGE_GROUP {
  FIRST_6,
  LAST_6
};

class EdgePatternDatabase : public PatternDatabase {
private:
  EDGE_GROUP group;

public:
  // Initialize the base class with exactly 42,577,920 states
  EdgePatternDatabase(EDGE_GROUP group);

  // Run the BFS and populate the database
  void generateDatabase();
};

#endif // EDGE_PATTERN_DATABASE_H
