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

// PRIVATE HELPERS:
// A single face has 8 facelets (8 bytes = 64 bits).
// A 90-degree turn moves a facelet 2 spots over (eg: index 0 becomes index 2).
// 2 spots * 8 bits = 16 bit shift. 
void Bitboard::rotateFaceClockwise(FACE face) {
    int f = static_cast<int>(face);
    // Circular left shift by 16 bits
    bitboard[f] = (bitboard[f] << 16) | (bitboard[f] >> (64-16));
}

void Bitboard::rotateFaceCounterClockwise(FACE face) {
    int f = static_cast<int>(face);
    // Circular right shift by 16 bits
    bitboard[f] = (bitboard[f] >> 16) | (bitboard[f] << (64-16));
}

// UP FACE ROTATIONS: (Top Row Cycle)
GenericCube& Bitboard::u() {
  rotateFaceClockwise(FACE::UP);

  // Mask for the top row (indices 0, 1, 2) which occupies the first 24 bits
  uint64_t mask = 0xFFFFFF; 

  // U is clockwise: Front -> Left -> Back -> Right -> Front
  uint64_t temp = bitboard[static_cast<int>(FACE::FRONT)] & mask;
  bitboard[static_cast<int>(FACE::FRONT)] = (bitboard[static_cast<int>(FACE::FRONT)] & ~mask) | (bitboard[static_cast<int>(FACE::RIGHT)] & mask);
  bitboard[static_cast<int>(FACE::RIGHT)] = (bitboard[static_cast<int>(FACE::RIGHT)] & ~mask) | (bitboard[static_cast<int>(FACE::BACK)] & mask);
  bitboard[static_cast<int>(FACE::BACK)]  = (bitboard[static_cast<int>(FACE::BACK)] & ~mask)  | (bitboard[static_cast<int>(FACE::LEFT)] & mask);
  bitboard[static_cast<int>(FACE::LEFT)]  = (bitboard[static_cast<int>(FACE::LEFT)] & ~mask)  | temp;

  return *this;
}

GenericCube& Bitboard::uPrime() { 
  rotateFaceCounterClockwise(FACE::UP);
  uint64_t mask = 0xFFFFFF;

  // Reverse cycle: Front -> Right -> Back -> Left -> Front
  uint64_t temp = bitboard[static_cast<int>(FACE::FRONT)] & mask;
  bitboard[static_cast<int>(FACE::FRONT)] = (bitboard[static_cast<int>(FACE::FRONT)] & ~mask) | (bitboard[static_cast<int>(FACE::LEFT)] & mask);
  bitboard[static_cast<int>(FACE::LEFT)]  = (bitboard[static_cast<int>(FACE::LEFT)] & ~mask)  | (bitboard[static_cast<int>(FACE::BACK)] & mask);
  bitboard[static_cast<int>(FACE::BACK)]  = (bitboard[static_cast<int>(FACE::BACK)] & ~mask)  | (bitboard[static_cast<int>(FACE::RIGHT)] & mask);
  bitboard[static_cast<int>(FACE::RIGHT)] = (bitboard[static_cast<int>(FACE::RIGHT)] & ~mask) | temp;

  return *this; 
}

GenericCube& Bitboard::u2() { 
  // A 180-degree turn is just doing the move twice
  u(); 
  u();

  return *this; 
}

// DOWN FACE ROTATIONS: (Bottom Row Cycle)
GenericCube& Bitboard::d() { 
  rotateFaceClockwise(FACE::DOWN);

  // Mask for the bottom row (indices 4, 5, 6). 
  // Index 4 starts at bit 32. 3 bytes = 24 bits.
  // So we shift 0xFFFFFF over by 32 bits.
  uint64_t mask = 0xFFFFFFull << 32; 

  // D is clockwise looking from the bottom: Front -> Right -> Back -> Left -> Front
  uint64_t temp = bitboard[static_cast<int>(FACE::FRONT)] & mask;
  bitboard[static_cast<int>(FACE::FRONT)] = (bitboard[static_cast<int>(FACE::FRONT)] & ~mask) | (bitboard[static_cast<int>(FACE::LEFT)] & mask);
  bitboard[static_cast<int>(FACE::LEFT)]  = (bitboard[static_cast<int>(FACE::LEFT)] & ~mask)  | (bitboard[static_cast<int>(FACE::BACK)] & mask);
  bitboard[static_cast<int>(FACE::BACK)]  = (bitboard[static_cast<int>(FACE::BACK)] & ~mask)  | (bitboard[static_cast<int>(FACE::RIGHT)] & mask);
  bitboard[static_cast<int>(FACE::RIGHT)] = (bitboard[static_cast<int>(FACE::RIGHT)] & ~mask) | temp;

  return *this;
}

GenericCube& Bitboard::dPrime() {
  rotateFaceCounterClockwise(FACE::DOWN);
  uint64_t mask = 0xFFFFFFull << 32;

  uint64_t temp = bitboard[static_cast<int>(FACE::FRONT)] & mask;
  bitboard[static_cast<int>(FACE::FRONT)] = (bitboard[static_cast<int>(FACE::FRONT)] & ~mask) | (bitboard[static_cast<int>(FACE::RIGHT)] & mask);
  bitboard[static_cast<int>(FACE::RIGHT)] = (bitboard[static_cast<int>(FACE::RIGHT)] & ~mask) | (bitboard[static_cast<int>(FACE::BACK)] & mask);
  bitboard[static_cast<int>(FACE::BACK)]  = (bitboard[static_cast<int>(FACE::BACK)] & ~mask)  | (bitboard[static_cast<int>(FACE::LEFT)] & mask);
  bitboard[static_cast<int>(FACE::LEFT)]  = (bitboard[static_cast<int>(FACE::LEFT)] & ~mask)  | temp;

  return *this;
}

GenericCube& Bitboard::d2() {
  d();
  d();
  return *this;
}


// FRONT FACE ROTATIONS:
GenericCube& Bitboard::f() {

  return *this;
}

GenericCube& Bitboard::fPrime() {

  return *this;
}

GenericCube& Bitboard::f2() {

  return *this;
}

// BACK FACE ROTATIONS:
GenericCube& Bitboard::b() {

  return *this;
}

GenericCube& Bitboard::bPrime() {

  return *this;
}

GenericCube& Bitboard::b2() {

  return *this;
}

// LEFT FACE ROTATIONS:
GenericCube& Bitboard::l() {

  return *this;
}

GenericCube& Bitboard::lPrime() {

  return *this;
}

GenericCube& Bitboard::l2() {

  return *this;
}

// RIGHT FACE ROTATIONS:
GenericCube& Bitboard::r() {

  return *this;
}

GenericCube& Bitboard::rPrime() {

  return *this;
}

GenericCube& Bitboard::r2() {

  return *this;
}
