#include <iostream>
#include <fstream>
#include "MarkovChar.h"
using namespace std;

int main(int argc, char** argv) {
  //Argument check.
  if(argc != 2) {
    cerr << "Usage: " << argv[0] << " corpus-file" << endl;
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
  MarkovChar<3> m;
  m.readFrom(ifile);

  //Generate a random Markov string (of chars) from the Markov object.
  for(int i=0; i<20; ++i) {
    cout << m.getMarkovString(0) << endl;
  }
  return 0;
}
