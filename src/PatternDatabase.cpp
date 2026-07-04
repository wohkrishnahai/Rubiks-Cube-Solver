#include "../include/PatternDatabase.h"
#include <fstream>
#include <iostream>
using namespace std;

PatternDatabase::PatternDatabase(size_t size) : size(size) {
  // Divide size by 2 as each byte holds two entries.
  // Initialize the array with 0xFF. 
  // In the 4-bit logic, 0xF (15) represents an "unvisited" state during BFS.
  database.resize(size/2 +1, 0xFF);
}

void PatternDatabase::setNumMoves(uint32_t index, uint8_t numMoves) {
  uint32_t byteIndex = index/2;
  // Even indices take the upper 4 bits (left half)
  // Odd indices take the lower 4 bits (right half)
  bool isUpperHalf = (index % 2 == 0); 

  if (isUpperHalf) {
    // Clear the upper 4 bits, preserve the lower 4 bits, then insert numMoves
    database[byteIndex] = (database[byteIndex] & 0x0F) | (numMoves << 4);
  } 
  else {
    // Clear the lower 4 bits, preserve the upper 4 bits, then insert numMoves
    database[byteIndex] = (database[byteIndex] & 0xF0) | (numMoves & 0x0F);
  }
}

uint8_t PatternDatabase::getNumMoves(uint32_t index) const {
  uint32_t byteIndex = index / 2;
  bool isUpperHalf = (index % 2 == 0);

  if (isUpperHalf) {
    // Shift upper bits down to read them
    return (database[byteIndex] >> 4) & 0x0F;
  } 
  else {
    // Just mask the lower bits
    return database[byteIndex] & 0x0F;
  }
}

bool PatternDatabase::saveToFile(const string& filepath) const {
  // Open in binary mode for raw byte writing (much faster than text)
  ofstream file(filepath, ios::out | ios::binary);
  if (!file) return false;

  file.write(reinterpret_cast<const char*>(database.data()), database.size());
  return true;
}

bool PatternDatabase::loadFromFile(const string& filepath) {
  ifstream file(filepath, ios::in | ios::binary);
  if (!file) return false;

  file.read(reinterpret_cast<char*>(database.data()), database.size());
  return true;
}
