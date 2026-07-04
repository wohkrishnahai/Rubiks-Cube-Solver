#include "../include/CornerIndexer.h"
#include "../include/MathUtils.h"
#include <vector>
using namespace std;

// In a standard cube:
// 0: UFL (White-Green-Orange)
// 1: UFR (White-Green-Red)
// 2: UBL (White-Blue-Orange)
// 3: UBR (White-Blue-Red)
// 4: DFL (Yellow-Green-Orange)
// 5: DFR (Yellow-Green-Red)
// 6: DBL (Yellow-Blue-Orange)
// 7: DBR (Yellow-Blue-Red)

int CornerIndexer::getCornerId(COLOR c1, COLOR c2, COLOR c3) {
  // A quick bitwise OR trick to identify the corner regardless of its twist orientation.
  // Each color maps to an integer 0-5. Shifting 1 by that amount creates a unique bitmask.
  int mask = (1 << static_cast<int>(c1)) | (1 << static_cast<int>(c2)) | (1 << static_cast<int>(c3));

  // UP=0, DOWN=1, FRONT=2, BACK=3, LEFT=4, RIGHT=5
  if (mask == ((1<<0) | (1<<2) | (1<<4))) return 0; // UFL
  if (mask == ((1<<0) | (1<<2) | (1<<5))) return 1; // UFR
  if (mask == ((1<<0) | (1<<3) | (1<<4))) return 2; // UBL
  if (mask == ((1<<0) | (1<<3) | (1<<5))) return 3; // UBR
  if (mask == ((1<<1) | (1<<2) | (1<<4))) return 4; // DFL
  if (mask == ((1<<1) | (1<<2) | (1<<5))) return 5; // DFR
  if (mask == ((1<<1) | (1<<3) | (1<<4))) return 6; // DBL
  if (mask == ((1<<1) | (1<<3) | (1<<5))) return 7; // DBR

  return -1;
}

uint32_t CornerIndexer::getIndex(const GenericCube& cube) {
  // Define the 8 corner slots by their 3 facelet coordinates (Face, Row, Col)
  // The first coordinate is always the U or D face to help determine orientation twist.
  struct CornerPos {
    FACE f1; unsigned r1, c1; // U/D Facelet
    FACE f2; unsigned r2, c2;
    FACE f3; unsigned r3, c3;
  };

  CornerPos corners[8] = {
    {FACE::UP, 2, 0, FACE::FRONT, 0, 0, FACE::LEFT, 0, 2},    // UFL
    {FACE::UP, 2, 2, FACE::FRONT, 0, 2, FACE::RIGHT, 0, 0},   // UFR
    {FACE::UP, 0, 0, FACE::BACK, 0, 2, FACE::LEFT, 0, 0},     // UBL
    {FACE::UP, 0, 2, FACE::BACK, 0, 0, FACE::RIGHT, 0, 2},    // UBR
    {FACE::DOWN, 0, 0, FACE::FRONT, 2, 0, FACE::LEFT, 2, 2},  // DFL
    {FACE::DOWN, 0, 2, FACE::FRONT, 2, 2, FACE::RIGHT, 2, 0}, // DFR
    {FACE::DOWN, 2, 0, FACE::BACK, 2, 2, FACE::LEFT, 2, 0},   // DBL
    {FACE::DOWN, 2, 2, FACE::BACK, 2, 0, FACE::RIGHT, 2, 2}   // DBR
  };

  int perm[8];
  int orient[8];

  // Extract the state of the 8 corners
  for (int i = 0; i < 8; ++i) {
    COLOR c1 = cube.getColor(corners[i].f1, corners[i].r1, corners[i].c1);
    COLOR c2 = cube.getColor(corners[i].f2, corners[i].r2, corners[i].c2);
    COLOR c3 = cube.getColor(corners[i].f3, corners[i].r3, corners[i].c3);

    perm[i] = getCornerId(c1, c2, c3);

    // Calculate Orientation (0 = solved, 1 = clockwise, 2 = counter-clockwise)
    // A corner is correctly oriented (0) if its UP/DOWN color (White or Yellow) is on the UP/DOWN face.
    if (c1 == COLOR::WHITE || c1 == COLOR::GREEN) orient[i] = 0;
    else if (c2 == COLOR::WHITE || c2 == COLOR::GREEN) orient[i] = 1;
    else orient[i] = 2;
  }

  // Calculate the Orientation Hash (Base 3)
  // We only need the first 7 corners. The 8th is mathematically forced.
  uint32_t orientationRank = 0;
  for (int i = 0; i < 7; ++i) {
    orientationRank = orientationRank * 3 + orient[i];
  }

  // Calculate the Permutation Hash (using The Lehmer Code)
  uint32_t permutationRank = 0;
  for (int i = 0; i < 7; ++i) {
    // Count how many numbers to the right are strictly less than the current number
    int inversions = 0;

    for (int j = i+1; j < 8; ++j) {
      if (perm[i] > perm[j]) inversions++;
    }

    // Multiply by (7-i)! 
    permutationRank += inversions * factorial(7-i);
  }

  // Combine them into a single Perfect Hash
  return (permutationRank * 2187) + orientationRank;
}
