#include "../include/Bitboard.h"
#include <iostream>

// CONSTRUCTOR:
// A solved face has 8 facelets (excluding center).
// We store each facelet in 8 bits (1 byte) of the 64-bit integer.
// Eg: if the color is 2 (Red), we shift '2' into all 8 byte positions.
Bitboard::Bitboard() {
  for (int i = 0; i < 6; ++i) {
    uint64_t solved_face = 0;
    // The center color matches the face index (0 = UP/WHITE, 1 = DOWN/GREEN, etc.)
    uint64_t color = static_cast<uint64_t>(i); 

    for (int j = 0; j < 8; ++j) {
      solved_face |= (color << (8 * j));
    }

    bitboard[i] = solved_face;
  }
}

// STATE RETRIEVAL:
bool Bitboard::isSolved() const {
  // A cube is solved if every face matches a perfectly solved face
  for (int i = 0; i < 6; ++i) {
    uint64_t solved_face = 0;
    uint64_t color = static_cast<uint64_t>(i);

    for (int j = 0; j < 8; ++j) {
      solved_face |= (color << (8 * j));
    }
    if (bitboard[i] != solved_face) return false;
  }
  return true;
}

COLOR Bitboard::getColor(FACE face, unsigned row, unsigned col) const {
  int face_idx = static_cast<int>(face);

  // The center piece (row 1, col 1) isn't stored in the 64-bit integer, because it never moves. 
  // So, just return the color associated with the face.
  if (row == 1 && col == 1) {
    return static_cast<COLOR>(face_idx);
  }

  // We map the 3x3 grid coordinates to our 0-7 bitboard indices.
  // The Circular Mapping Standard in Bitboard Representations:
  // (0,0)=0 | (0,1)=1 | (0,2)=2
  // (1,0)=7 | CENTER  | (1,2)=3
  // (2,0)=6 | (2,1)=5 | (2,2)=4
  int idx = -1;
  if (row == 0 && col == 0) idx = 0;
  else if (row == 0 && col == 1) idx = 1;
  else if (row == 0 && col == 2) idx = 2;
  else if (row == 1 && col == 2) idx = 3;
  else if (row == 2 && col == 2) idx = 4;
  else if (row == 2 && col == 1) idx = 5;
  else if (row == 2 && col == 0) idx = 6;
  else if (row == 1 && col == 0) idx = 7;

  // Shift the integer right to drop the unwanted bits, 
  // then mask with 0xFF to isolate the byte.
  uint64_t color_val = (bitboard[face_idx] >> (8 * idx)) & 0xFF;
  return static_cast<COLOR>(color_val);
}

uint64_t Bitboard::getFace(FACE face) const {
  return bitboard[static_cast<int>(face)];
}

void Bitboard::setFace(FACE face, uint64_t val) {
  bitboard[static_cast<int>(face)] = val;
}

// PLACEHOLDERS FOR COMPILATION:
void Bitboard::print() const {
  // Can implement a simple text output later for Debugging (if needed)
}

GenericCube& Bitboard::move(MOVE ind) {
  // Implement switch statement later
  return *this;
}

GenericCube& Bitboard::invert(MOVE ind) {
  // Implement inverse lookup later
  return *this;
}

// STUBS for further commits
// Returning '*this' allows the code to compile cleanly today.
GenericCube& Bitboard::u() { return *this; }
GenericCube& Bitboard::uPrime() { return *this; }
GenericCube& Bitboard::u2() { return *this; }

GenericCube& Bitboard::d() { return *this; }
GenericCube& Bitboard::dPrime() { return *this; }
GenericCube& Bitboard::d2() { return *this; }

GenericCube& Bitboard::f() { return *this; }
GenericCube& Bitboard::fPrime() { return *this; }
GenericCube& Bitboard::f2() { return *this; }

GenericCube& Bitboard::b() { return *this; }
GenericCube& Bitboard::bPrime() { return *this; }
GenericCube& Bitboard::b2() { return *this; }

GenericCube& Bitboard::l() { return *this; }
GenericCube& Bitboard::lPrime() { return *this; }
GenericCube& Bitboard::l2() { return *this; }

GenericCube& Bitboard::r() { return *this; }
GenericCube& Bitboard::rPrime() { return *this; }
GenericCube& Bitboard::r2() { return *this; }
