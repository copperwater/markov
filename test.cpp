#include <iostream>
#include <fstream>
#include "Markov.h"
using namespace std;

int main(int argc, char** argv) {
  //Argument check.
  if(argc != 3) {
    cerr << "Usage: " << argv[0] << " corpus-file [lbl | corpus]" << endl;
    return 1;
  }

  //Initialize randomness.
  srand(time(0));
  
  //Open corpus file.
  ifstream ifile(argv[1]);
  if(!ifile) {
    cerr << "Failed to open file." << endl;
    return 1;
  }

  //Instantiate a Markov object and feed it the corpus.
  Markov<3> m;
  m.readFrom(ifile, argv[2]);

  //Generate a random Markov chain of strings from the Markov object.
  cout << m.getMarkovString(0) << endl;
  
  return 0;
}
