#include <iostream>
#include "../include/EdgePatternDatabase.h"
using namespace std;

int main() {
  cout << "=== EDGE PATTERN DATABASE GENERATOR ===" << endl << endl;

  // FIRST 6 EDGES:
  cout << "Allocating 21MB RAM for First 6 Edges..." << endl;
  EdgePatternDatabase first6DB(EDGE_GROUP::FIRST_6);
  first6DB.generateDatabase();

  string file1 = "first_6_edges.bin";
  cout << "Saving database to " << file1 << "..." << endl;
  if (first6DB.saveToFile(file1)) cout << "SUCCESS: " << file1 << " saved." << endl;
  else cout << "ERROR: Failed to write " << file1 << endl;

  cout << "\n-----------------------------------\n" << endl;

  // LAST 6 EDGES:
  cout << "Allocating 21MB RAM for Last 6 Edges..." << endl;
  EdgePatternDatabase last6DB(EDGE_GROUP::LAST_6);
  last6DB.generateDatabase();

  string file2 = "last_6_edges.bin";
  cout << "Saving database to " << file2 << "..." << endl;
  if (last6DB.saveToFile(file2)) cout << "SUCCESS: " << file2 << " saved." << endl;
  else cout << "ERROR: Failed to write " << file2 << endl;

  cout << "\n===================================" << endl;

  return 0;
}
