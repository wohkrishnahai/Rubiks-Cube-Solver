#ifndef BITBOARD_H
#define BITBOARD_H

#include "GenericCube.h"
#include <cstdint> // for uint64_t

class Bitboard : public GenericCube {
private:
    // The core memory of the cube. 
    // bitboard[0] is UP, bitboard[1] is DOWN, etc. (matching the FACE enum)
    uint64_t bitboard[6]{}; 
    
    // Helper Functions to rotate a single face
    inline void rotateFaceClockwise(FACE face);
    inline void rotateFaceCounterClockwise(FACE face);

public:
    // Constructor
    Bitboard();

    // OVERRIDDEN STATE FUNCTIONS:
    bool isSolved() const override;
    COLOR getColor(FACE face, unsigned row, unsigned col) const override;
    void print() const override;

    // OVERRIDDEN HELPER FUNCTIONS:
    GenericCube& move(MOVE ind) override;
    GenericCube& invert(MOVE ind) override;

    // OVERRIDDEN ROTATION FUNCTIONS:
    GenericCube& u() override;
    GenericCube& uPrime() override;
    GenericCube& u2() override;

    GenericCube& d() override;
    GenericCube& dPrime() override;
    GenericCube& d2() override;

    GenericCube& f() override;
    GenericCube& fPrime() override;
    GenericCube& f2() override;

    GenericCube& b() override;
    GenericCube& bPrime() override;
    GenericCube& b2() override;

    GenericCube& l() override;
    GenericCube& lPrime() override;
    GenericCube& l2() override;

    GenericCube& r() override;
    GenericCube& rPrime() override;
    GenericCube& r2() override;

    // BITBOARD SPECIFIC HELPERS:
    // Made public so the Pattern Database can hash the cube state later
    uint64_t getFace(FACE face) const;
    void setFace(FACE face, uint64_t val);
};

#endif // BITBOARD_H
