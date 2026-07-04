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
  switch (ind) {
    case MOVE::U: return u();
    case MOVE::UPRIME: return uPrime();
    case MOVE::U2: return u2();
    case MOVE::D: return d();
    case MOVE::DPRIME: return dPrime();
    case MOVE::D2: return d2();
    case MOVE::F: return f();
    case MOVE::FPRIME: return fPrime();
    case MOVE::F2: return f2();
    case MOVE::B: return b();
    case MOVE::BPRIME: return bPrime();
    case MOVE::B2: return b2();
    case MOVE::L: return l();
    case MOVE::LPRIME: return lPrime();
    case MOVE::L2: return l2();
    case MOVE::R: return r();
    case MOVE::RPRIME: return rPrime();
    case MOVE::R2: return r2();
  }

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
  u(); u();
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
  d(); d();
  return *this;
}


// FRONT FACE ROTATIONS:
GenericCube& Bitboard::f() {
  rotateFaceClockwise(FACE::FRONT);

  // EXTRACT -> CLEAR HOLES (Destination Bits) -> SHIFT & INSERT
  int u = static_cast<int>(FACE::UP);
  int r = static_cast<int>(FACE::RIGHT);
  int d = static_cast<int>(FACE::DOWN);
  int l = static_cast<int>(FACE::LEFT);

  // F affects: UP(4,5,6), RIGHT(0,7,6), DOWN(0,1,2), LEFT(2,3,4)
  // Save UP (which will be overwritten first)
  uint64_t temp_u4 = (bitboard[u] >> (8 * 4)) & 0xFF;
  uint64_t temp_u5 = (bitboard[u] >> (8 * 5)) & 0xFF;
  uint64_t temp_u6 = (bitboard[u] >> (8 * 6)) & 0xFF;

  // UP gets LEFT
  bitboard[u] &= ~((0xFFull << (8 * 4)) | (0xFFull << (8 * 5)) | (0xFFull << (8 * 6)));
  bitboard[u] |= ((bitboard[l] >> (8 * 2)) & 0xFF) << (8 * 4);
  bitboard[u] |= ((bitboard[l] >> (8 * 3)) & 0xFF) << (8 * 5);
  bitboard[u] |= ((bitboard[l] >> (8 * 4)) & 0xFF) << (8 * 6);

  // LEFT gets DOWN
  bitboard[l] &= ~((0xFFull << (8 * 2)) | (0xFFull << (8 * 3)) | (0xFFull << (8 * 4)));
  bitboard[l] |= ((bitboard[d] >> (8 * 0)) & 0xFF) << (8 * 2);
  bitboard[l] |= ((bitboard[d] >> (8 * 1)) & 0xFF) << (8 * 3);
  bitboard[l] |= ((bitboard[d] >> (8 * 2)) & 0xFF) << (8 * 4);

  // DOWN gets RIGHT (index inversion)
  bitboard[d] &= ~((0xFFull << (8 * 0)) | (0xFFull << (8 * 1)) | (0xFFull << (8 * 2)));
  bitboard[d] |= ((bitboard[r] >> (8 * 6)) & 0xFF) << (8 * 0);
  bitboard[d] |= ((bitboard[r] >> (8 * 7)) & 0xFF) << (8 * 1);
  bitboard[d] |= ((bitboard[r] >> (8 * 0)) & 0xFF) << (8 * 2);

  // RIGHT gets original UP
  bitboard[r] &= ~((0xFFull << (8 * 0)) | (0xFFull << (8 * 7)) | (0xFFull << (8 * 6)));
  bitboard[r] |= (temp_u6 << (8 * 0));
  bitboard[r] |= (temp_u5 << (8 * 7));
  bitboard[r] |= (temp_u4 << (8 * 6));

  return *this;
}

GenericCube& Bitboard::fPrime() {
  // fPrime is simply applying f() three times
  f(); f(); f(); 
  return *this;
}

GenericCube& Bitboard::f2() {
  f(); f(); 
  return *this;
}

// BACK FACE ROTATIONS:
GenericCube& Bitboard::b() {
  rotateFaceClockwise(FACE::BACK);

  int u = static_cast<int>(FACE::UP);
  int r = static_cast<int>(FACE::RIGHT);
  int d = static_cast<int>(FACE::DOWN);
  int l = static_cast<int>(FACE::LEFT);

  // B affects: UP(0,1,2), LEFT(0,7,6), DOWN(4,5,6), RIGHT(2,3,4)
  uint64_t temp_u0 = (bitboard[u] >> (8 * 0)) & 0xFF;
  uint64_t temp_u1 = (bitboard[u] >> (8 * 1)) & 0xFF;
  uint64_t temp_u2 = (bitboard[u] >> (8 * 2)) & 0xFF;

  // UP gets RIGHT
  bitboard[u] &= ~((0xFFull << (8 * 0)) | (0xFFull << (8 * 1)) | (0xFFull << (8 * 2)));
  bitboard[u] |= ((bitboard[r] >> (8 * 2)) & 0xFF) << (8 * 0);
  bitboard[u] |= ((bitboard[r] >> (8 * 3)) & 0xFF) << (8 * 1);
  bitboard[u] |= ((bitboard[r] >> (8 * 4)) & 0xFF) << (8 * 2);

  // RIGHT gets DOWN
  bitboard[r] &= ~((0xFFull << (8 * 2)) | (0xFFull << (8 * 3)) | (0xFFull << (8 * 4)));
  bitboard[r] |= ((bitboard[d] >> (8 * 6)) & 0xFF) << (8 * 2);
  bitboard[r] |= ((bitboard[d] >> (8 * 5)) & 0xFF) << (8 * 3);
  bitboard[r] |= ((bitboard[d] >> (8 * 4)) & 0xFF) << (8 * 4);

  // DOWN gets LEFT
  bitboard[d] &= ~((0xFFull << (8 * 4)) | (0xFFull << (8 * 5)) | (0xFFull << (8 * 6)));
  bitboard[d] |= ((bitboard[l] >> (8 * 0)) & 0xFF) << (8 * 6);
  bitboard[d] |= ((bitboard[l] >> (8 * 7)) & 0xFF) << (8 * 5);
  bitboard[d] |= ((bitboard[l] >> (8 * 6)) & 0xFF) << (8 * 4);

  // LEFT gets original UP
  bitboard[l] &= ~((0xFFull << (8 * 0)) | (0xFFull << (8 * 7)) | (0xFFull << (8 * 6)));
  bitboard[l] |= (temp_u0 << (8 * 0));
  bitboard[l] |= (temp_u1 << (8 * 7));
  bitboard[l] |= (temp_u2 << (8 * 6));

  return *this;
}

GenericCube& Bitboard::bPrime() {
  b(); b(); b();
  return *this;
}

GenericCube& Bitboard::b2() {
  b(); b();
  return *this;
}

// LEFT FACE ROTATIONS:
GenericCube& Bitboard::l() {
  rotateFaceClockwise(FACE::LEFT);

  int u = static_cast<int>(FACE::UP);
  int f = static_cast<int>(FACE::FRONT);
  int d = static_cast<int>(FACE::DOWN);
  int b_face = static_cast<int>(FACE::BACK); // 'b_face' to avoid shadowing function b()

  // L affects: UP(0,7,6), FRONT(0,7,6), DOWN(0,7,6), BACK(4,3,2)
  uint64_t temp_u0 = (bitboard[u] >> (8 * 0)) & 0xFF;
  uint64_t temp_u7 = (bitboard[u] >> (8 * 7)) & 0xFF;
  uint64_t temp_u6 = (bitboard[u] >> (8 * 6)) & 0xFF;

  // UP gets BACK (inverted indices)
  bitboard[u] &= ~((0xFFull << (8 * 0)) | (0xFFull << (8 * 7)) | (0xFFull << (8 * 6)));
  bitboard[u] |= ((bitboard[b_face] >> (8 * 4)) & 0xFF) << (8 * 0);
  bitboard[u] |= ((bitboard[b_face] >> (8 * 3)) & 0xFF) << (8 * 7);
  bitboard[u] |= ((bitboard[b_face] >> (8 * 2)) & 0xFF) << (8 * 6);

  // BACK gets DOWN (inverted indices)
  bitboard[b_face] &= ~((0xFFull << (8 * 4)) | (0xFFull << (8 * 3)) | (0xFFull << (8 * 2)));
  bitboard[b_face] |= ((bitboard[d] >> (8 * 0)) & 0xFF) << (8 * 4);
  bitboard[b_face] |= ((bitboard[d] >> (8 * 7)) & 0xFF) << (8 * 3);
  bitboard[b_face] |= ((bitboard[d] >> (8 * 6)) & 0xFF) << (8 * 2);

  // DOWN gets FRONT
  bitboard[d] &= ~((0xFFull << (8 * 0)) | (0xFFull << (8 * 7)) | (0xFFull << (8 * 6)));
  bitboard[d] |= ((bitboard[f] >> (8 * 0)) & 0xFF) << (8 * 0);
  bitboard[d] |= ((bitboard[f] >> (8 * 7)) & 0xFF) << (8 * 7);
  bitboard[d] |= ((bitboard[f] >> (8 * 6)) & 0xFF) << (8 * 6);

  // FRONT gets original UP
  bitboard[f] &= ~((0xFFull << (8 * 0)) | (0xFFull << (8 * 7)) | (0xFFull << (8 * 6)));
  bitboard[f] |= (temp_u0 << (8 * 0));
  bitboard[f] |= (temp_u7 << (8 * 7));
  bitboard[f] |= (temp_u6 << (8 * 6));

  return *this;
}

GenericCube& Bitboard::lPrime() {
  l(); l(); l();
  return *this;
}

GenericCube& Bitboard::l2() {
  l(); l();
  return *this;
}

// RIGHT FACE ROTATIONS:
GenericCube& Bitboard::r() {
  rotateFaceClockwise(FACE::RIGHT);

  int u = static_cast<int>(FACE::UP);
  int b_face = static_cast<int>(FACE::BACK);
  int d = static_cast<int>(FACE::DOWN);
  int f = static_cast<int>(FACE::FRONT);

  // R affects: UP(2,3,4), BACK(6,7,0), DOWN(2,3,4), FRONT(2,3,4)
  uint64_t temp_u2 = (bitboard[u] >> (8 * 2)) & 0xFF;
  uint64_t temp_u3 = (bitboard[u] >> (8 * 3)) & 0xFF;
  uint64_t temp_u4 = (bitboard[u] >> (8 * 4)) & 0xFF;

  // UP gets FRONT
  bitboard[u] &= ~((0xFFull << (8 * 2)) | (0xFFull << (8 * 3)) | (0xFFull << (8 * 4)));
  bitboard[u] |= ((bitboard[f] >> (8 * 2)) & 0xFF) << (8 * 2);
  bitboard[u] |= ((bitboard[f] >> (8 * 3)) & 0xFF) << (8 * 3);
  bitboard[u] |= ((bitboard[f] >> (8 * 4)) & 0xFF) << (8 * 4);

  // FRONT gets DOWN
  bitboard[f] &= ~((0xFFull << (8 * 2)) | (0xFFull << (8 * 3)) | (0xFFull << (8 * 4)));
  bitboard[f] |= ((bitboard[d] >> (8 * 2)) & 0xFF) << (8 * 2);
  bitboard[f] |= ((bitboard[d] >> (8 * 3)) & 0xFF) << (8 * 3);
  bitboard[f] |= ((bitboard[d] >> (8 * 4)) & 0xFF) << (8 * 4);

  // DOWN gets BACK (inverted indices)
  bitboard[d] &= ~((0xFFull << (8 * 2)) | (0xFFull << (8 * 3)) | (0xFFull << (8 * 4)));
  bitboard[d] |= ((bitboard[b_face] >> (8 * 6)) & 0xFF) << (8 * 2);
  bitboard[d] |= ((bitboard[b_face] >> (8 * 7)) & 0xFF) << (8 * 3);
  bitboard[d] |= ((bitboard[b_face] >> (8 * 0)) & 0xFF) << (8 * 4);

  // BACK gets original UP (inverted indices)
  bitboard[b_face] &= ~((0xFFull << (8 * 6)) | (0xFFull << (8 * 7)) | (0xFFull << (8 * 0)));
  bitboard[b_face] |= (temp_u2 << (8 * 6));
  bitboard[b_face] |= (temp_u3 << (8 * 7));
  bitboard[b_face] |= (temp_u4 << (8 * 0));

  return *this;
}

GenericCube& Bitboard::rPrime() {
  r(); r(); r();
  return *this;
}

GenericCube& Bitboard::r2() {
  r(); r();
  return *this;
}
