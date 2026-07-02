#ifndef GENERICCUBE_H
#define GENERICCUBE_H

#include <iostream>
#include <algorithm>
#include <cstdint>
#include <string>
#include <vector>
using namespace std;

class RubiksCube {
public:
  enum class FACE {
    UP,
    LEFT,
    FRONT,
    RIGHT,
    BACK,
    DOWN
  };

  enum class COLOR {
    WHITE,
    GREEN,
    RED,
    BLUE,
    ORANGE,
    YELLOW
  };

  enum class MOVE {
    L, LPRIME, L2,
    R, RPRIME, R2,
    U, UPRIME, U2,
    D, DPRIME, D2,
    F, FPRIME, F2,
    B, BPRIME, B2
  };
};

#endif // GENERICCUBE_H
