#ifndef CORNER_INDEXER_H
#define CORNER_INDEXER_H

#include "GenericCube.h"
#include <cstdint>

class CornerIndexer {
private:
  // Helper to identify exactly which of the 8 corners is sitting in a specific slot
  static int getCornerId(COLOR c1, COLOR c2, COLOR c3);

public:
  // Takes ANY cube representation and calculates its exact Perfect Hash (0 to 88,179,839)
  static uint32_t getIndex(const GenericCube& cube);
};

#endif // CORNER_INDEXER_H
