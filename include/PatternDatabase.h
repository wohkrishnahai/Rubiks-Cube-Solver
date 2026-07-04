#ifndef PATTERN_DATABASE_H
#define PATTERN_DATABASE_H

#include <cstdint>
#include <vector>
#include <string>

class PatternDatabase {
private:
  // A contiguous byte array for compressed nibbles
  std::vector<uint8_t> database;
  size_t size;

public:
  // Constructor
  PatternDatabase(size_t size);

  // BIT-PACKING LOGIC:
  // Sets the 4-bit move count at the given perfect hash index
  void setNumMoves(uint32_t index, uint8_t numMoves);

  // Retrieves the 4-bit move count
  uint8_t getNumMoves(uint32_t index) const;

  // FILE I/O:
  // Writing/reading ~44MB to Disk prevents form having to run BFS every time
  bool saveToFile(const std::string& filepath) const;
  bool loadFromFile(const std::string& filepath);
};

#endif // PATTERN_DATABASE_H
