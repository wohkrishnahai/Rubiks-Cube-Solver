#include <iostream>
#include "../include/CornerPatternDatabase.h"
using namespace std;

int main() {
  cout << "=== PATTERN DATABASE GENERATOR ===" << endl;

  cout << "1. Allocating 44MB of RAM for the database array..." << endl;
  CornerPatternDatabase cornerDB;

  cout << "2. Starting the Breadth-First Search (BFS)..." << endl;
  // Run the BFS to explore all 88,179,840 states
  cornerDB.generateDatabase();

  string fileName = "corner_database.bin";
  cout << "3. Saving database to " << fileName << "..." << endl;

  if (cornerDB.saveToFile(fileName)) {
    cout << "SUCCESS: Database saved to disk safely." << endl;
  } 
  else {
    cout << "ERROR: Failed to write file to disk. Check folder permissions." << endl;
  }

  cout << "==================================" << endl;

  return 0;
}
