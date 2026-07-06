#include "../include/EdgeIndexer.h"
#include "../include/MathUtils.h"

// Define the 12 standard edge pieces based on their colors
// 0: UR, 1: UF, 2: UL, 3: UB, 4: DR, 5: DF, 6: DL, 7: DB, 8: FR, 9: FL, 10: BL, 11: BR
int EdgeIndexer::getEdgeId(COLOR c1, COLOR c2) {
  int mask = (1 << static_cast<int>(c1)) | (1 << static_cast<int>(c2));

  if (mask == ((1 << 0) | (1 << 5))) return 0;  // U-R (White-Red)
  if (mask == ((1 << 0) | (1 << 2))) return 1;  // U-F (White-Front)
  if (mask == ((1 << 0) | (1 << 4))) return 2;  // U-L (White-Left)
  if (mask == ((1 << 0) | (1 << 3))) return 3;  // U-B (White-Back)
  if (mask == ((1 << 1) | (1 << 5))) return 4;  // D-R (Green-Red)
  if (mask == ((1 << 1) | (1 << 2))) return 5;  // D-F (Green-Front)
  if (mask == ((1 << 1) | (1 << 4))) return 6;  // D-L (Green-Left)
  if (mask == ((1 << 1) | (1 << 3))) return 7;  // D-B (Green-Back)
  if (mask == ((1 << 2) | (1 << 5))) return 8;  // F-R (Front-Red)
  if (mask == ((1 << 2) | (1 << 4))) return 9;  // F-L (Front-Left)
  if (mask == ((1 << 3) | (1 << 4))) return 10; // B-L (Back-Left)
  if (mask == ((1 << 3) | (1 << 5))) return 11; // B-R (Back-Red)

  return -1;
}

uint32_t EdgeIndexer::getFirst6EdgesIndex(const GenericCube& cube) {
  // Define the 12 physical edge slots by their coordinates
  struct EdgePos {
    FACE f1; unsigned r1, c1;
    FACE f2; unsigned r2, c2;
  };

  EdgePos edges[12] = {
    {FACE::UP, 1, 2, FACE::RIGHT, 0, 1}, // UR
    {FACE::UP, 2, 1, FACE::FRONT, 0, 1}, // UF
    {FACE::UP, 1, 0, FACE::LEFT, 0, 1},  // UL
    {FACE::UP, 0, 1, FACE::BACK, 0, 1},  // UB
    {FACE::DOWN, 1, 2, FACE::RIGHT, 2, 1}, // DR
    {FACE::DOWN, 0, 1, FACE::FRONT, 2, 1}, // DF
    {FACE::DOWN, 1, 0, FACE::LEFT, 2, 1},  // DL
    {FACE::DOWN, 2, 1, FACE::BACK, 2, 1},  // DB
    {FACE::FRONT, 1, 2, FACE::RIGHT, 1, 0}, // FR
    {FACE::FRONT, 1, 0, FACE::LEFT, 1, 2},  // FL
    {FACE::BACK, 1, 2, FACE::LEFT, 1, 0},   // BL
    {FACE::BACK, 1, 0, FACE::RIGHT, 1, 2}   // BR
  };

  int perm[6];
  int orient[6];

  // Scan all 12 slots to find where the FIRST 6 edges (IDs 0-5) are currently sitting
  for (int i = 0; i < 12; ++i) {
    COLOR c1 = cube.getColor(edges[i].f1, edges[i].r1, edges[i].c1);
    COLOR c2 = cube.getColor(edges[i].f2, edges[i].r2, edges[i].c2);

    int edgeId = getEdgeId(c1, c2);

    // If this slot holds one of our target edges (IDs 0 through 5)
    if (edgeId >= 0 && edgeId < 6) {
      perm[edgeId] = i;  // Map the specific edge to its physical slot

      // Calculate Orientation (Base 2)
      // If the UP/DOWN color is on the UP/DOWN face, it is oriented (0)
      if (c1 == COLOR::WHITE || c1 == COLOR::GREEN) orient[edgeId] = 0;
      else orient[edgeId] = 1;
    }
  }

  // Orientation Hash (Base 2)
  uint32_t orientationRank = 0;
  for (int i = 0; i < 6; ++i) {
    orientationRank = (orientationRank << 1) | orient[i];
  }

  // Permutation Hash (Lehmer Code for P(12,6))
  uint32_t permutationRank = 0;
  for (int i = 0; i < 6; ++i) {
    int inversions = perm[i]; // Start with the physical slot (0 to 11)
    for (int j = 0; j < i; ++j) {
      if (perm[j] < perm[i]) inversions--;
    }

    // Instead of standard factorial, we calculate remaining slots
    permutationRank += inversions * (factorial(11-i) / factorial(12-6));
  }

  // Combine them (Multiply by 2^6 = 64)
  return (permutationRank * 64) + orientationRank;
}

uint32_t EdgeIndexer::getLast6EdgesIndex(const GenericCube& cube) {
  // Define the 12 physical edge slots by their coordinates
  // The implementation for the last 6 edges (IDs 6-11) is nearly identical.
  struct EdgePos {
    FACE f1; unsigned r1, c1;
    FACE f2; unsigned r2, c2;
  };

  EdgePos edges[12] = {
    {FACE::UP, 1, 2, FACE::RIGHT, 0, 1}, // UR
    {FACE::UP, 2, 1, FACE::FRONT, 0, 1}, // UF
    {FACE::UP, 1, 0, FACE::LEFT, 0, 1},  // UL
    {FACE::UP, 0, 1, FACE::BACK, 0, 1},  // UB
    {FACE::DOWN, 1, 2, FACE::RIGHT, 2, 1}, // DR
    {FACE::DOWN, 0, 1, FACE::FRONT, 2, 1}, // DF
    {FACE::DOWN, 1, 0, FACE::LEFT, 2, 1},  // DL
    {FACE::DOWN, 2, 1, FACE::BACK, 2, 1},  // DB
    {FACE::FRONT, 1, 2, FACE::RIGHT, 1, 0}, // FR
    {FACE::FRONT, 1, 0, FACE::LEFT, 1, 2},  // FL
    {FACE::BACK, 1, 2, FACE::LEFT, 1, 0},   // BL
    {FACE::BACK, 1, 0, FACE::RIGHT, 1, 2}   // BR
  };

  int perm[6];
  int orient[6];

  // Scan all 12 slots to find where the LAST 6 edges (IDs 6-11) are currently sitting
  for (int i = 0; i < 12; ++i) {
    COLOR c1 = cube.getColor(edges[i].f1, edges[i].r1, edges[i].c1);
    COLOR c2 = cube.getColor(edges[i].f2, edges[i].r2, edges[i].c2);

    int edgeId = getEdgeId(c1, c2);

    // Target the last 6 edges (IDs 6 through 11)
    if (edgeId >= 6 && edgeId < 12) {
      perm[edgeId -6] = i; 

      // Calculate Orientation (Base 2)
      // D-edges (6,7) use Up/Down colors (0,1). 
      // E-slice edges (8-11) use Front/Back colors (2,3).
      if (edgeId == 6 || edgeId == 7) {
        if ((int)c1 == 0 || (int)c1 == 1) orient[edgeId -6] = 0;
        else orient[edgeId -6] = 1;
      } else {
        if ((int)c1 == 2 || (int)c1 == 3) orient[edgeId -6] = 0;
        else orient[edgeId -6] = 1;
      }
    }
  }

  // Orientation Hash (Base 2)
  uint32_t orientationRank = 0;
  for (int i = 0; i < 6; ++i) {
    orientationRank = (orientationRank << 1) | orient[i];
  }

  // Permutation Hash (Lehmer Code for P(12,6))
  uint32_t permutationRank = 0;
  for (int i = 0; i < 6; ++i) {
    int inversions = perm[i];
    for (int j = 0; j < i; ++j) {
      if (perm[j] < perm[i]) inversions--;
    }

    permutationRank += inversions * (factorial(11-i) / factorial(12-6));
  }

  // Combine them (Multiply by 2^6 = 64)
  return (permutationRank * 64) + orientationRank;
}
