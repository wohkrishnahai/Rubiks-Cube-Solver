#include "../include/Scrambler.h"
#include <random>
#include <ctime>
#include <sstream>
#include <unordered_map>
#include <stdexcept>
using namespace std;

// Generates a WCA-compliant random scramble
vector<MOVE> Scrambler::generateScramble(int length) {
  vector<MOVE> scramble;
  mt19937 rng(time(0));
  uniform_int_distribution<int> dist(0, 17);

  int lastFace = -1;
  int secondLastFace = -1;

  while (scramble.size() < length) {
    int randomMove = dist(rng);

    // Faces: 0=U, 1=D, 2=F, 3=B, 4=L, 5=R
    int currentFace = randomMove / 3;

    // Same Face Pruning (eg: no U followed by U2)
    if (currentFace == lastFace) continue;

    // Parallel Face Sequence Pruning (eg: no U followed by D followed by U')
    // We know faces are parallel if they divide by 2 to the same integer (0&1, 2&3, 4&5)
    if (currentFace == secondLastFace && (currentFace / 2 == lastFace / 2)) continue;

    scramble.push_back(static_cast<MOVE>(randomMove));
    secondLastFace = lastFace;
    lastFace = currentFace;
  }

  return scramble;
}

vector<MOVE> Scrambler::parseScramble(const string& scrambleStr) {
  vector<MOVE> scramble;

  unordered_map<string, MOVE> moveMap = {
    {"U", MOVE::U}, {"U'", MOVE::UPRIME}, {"U2", MOVE::U2},
    {"D", MOVE::D}, {"D'", MOVE::DPRIME}, {"D2", MOVE::D2},
    {"F", MOVE::F}, {"F'", MOVE::FPRIME}, {"F2", MOVE::F2},
    {"B", MOVE::B}, {"B'", MOVE::BPRIME}, {"B2", MOVE::B2},
    {"L", MOVE::L}, {"L'", MOVE::LPRIME}, {"L2", MOVE::L2},
    {"R", MOVE::R}, {"R'", MOVE::RPRIME}, {"R2", MOVE::R2}
  };

  stringstream ss(scrambleStr);
  string token;

  while (ss >> token) {
    if (moveMap.count(token)) {
      scramble.push_back(moveMap[token]);
    } else {
      throw invalid_argument("Invalid move detected: " + token);
    }
  }

  return scramble;
}

