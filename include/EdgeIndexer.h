#ifndef EDGE_INDEXER_H
#define EDGE_INDEXER_H

#include "GenericCube.h"
#include <cstdint>

class EdgeIndexer {
private:
  // Helper to identify exactly which of the 12 edges is sitting in a specific slot
  static int getEdgeId(COLOR c1, COLOR c2);

public:
  // Calculates perfect hash for the First 6 Edges (0 to 42,577,919)
  static uint32_t getFirst6EdgesIndex(const GenericCube& cube);

  // Calculates perfect hash for the Last 6 Edges (0 to 42,577,919)
  static uint32_t getLast6EdgesIndex(const GenericCube& cube);
};

#endif // EDGE_INDEXER_H
