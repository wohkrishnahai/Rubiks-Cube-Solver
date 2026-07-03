#ifndef GENERICCUBE_H
#define GENERICCUBE_H

#include <string>
#include <vector>
using namespace std;

// The 6 Faces of the Cube
enum class FACE {
  UP, DOWN,
  FRONT, BACK,
  LEFT, RIGHT
};

// The 6 Colors in the Cube
enum class COLOR {
  WHITE, GREEN,
  RED, BLUE,
  ORANGE, YELLOW
};

// The 18 Possible Moves in the Cube
enum class MOVE {
  U, UPRIME, U2,
  D, DPRIME, D2,
  F, FPRIME, F2,
  B, BPRIME, B2,
  L, LPRIME, L2,
  R, RPRIME, R2
};

class GenericCube {
public:
  // Virtual Destructor for Abstract Base Classes
  virtual ~GenericCube() = default;

  // STATE FUNCTIONS:
  // Returns true if the cube is completely solved
  virtual bool isSolved() const = 0;

  // Returns the color of a specific square on a specific face
  virtual COLOR getColor(FACE face, unsigned row, unsigned col) const = 0;

  // Prints the cube to the Terminal
  virtual void print() const = 0;

  // HELPER FUNCTIONS:
  // A function that takes a generic MOVE enum and calls the correct rotation
  virtual GenericCube& move(MOVE ind) = 0;

  // Inverts a Move (eg: U returns U')
  virtual GenericCube& invert(MOVE ind) = 0;

  // THE 18 ROTATION FUNCTIONS:
  // Returning a Reference (GenericCube&) to Chain Moves (like: cube.u().r().f();)
  virtual GenericCube& u() = 0;
  virtual GenericCube& uPrime() = 0;
  virtual GenericCube& u2() = 0;

  virtual GenericCube& d() = 0;
  virtual GenericCube& dPrime() = 0;
  virtual GenericCube& d2() = 0;

  virtual GenericCube& f() = 0;
  virtual GenericCube& fPrime() = 0;
  virtual GenericCube& f2() = 0;

  virtual GenericCube& b() = 0;
  virtual GenericCube& bPrime() = 0;
  virtual GenericCube& b2() = 0;

  virtual GenericCube& l() = 0;
  virtual GenericCube& lPrime() = 0;
  virtual GenericCube& l2() = 0;

  virtual GenericCube& r() = 0;
  virtual GenericCube& rPrime() = 0;
  virtual GenericCube& r2() = 0;
};

#endif // GENERICCUBE_H
