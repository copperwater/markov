#include <iostream>
#include <fstream>
#include "Markov.h"
using namespace std;

//if you want to control the accuracy of the Markov object at compile time:
//g++ ... -DACCURACY=[x]
#define ACCURACY 2

void usage(string argv0) {
  cerr << "Usage: " << argv0 << " corpus-file [-l]" << endl;
  cerr << "    -l: read line by line" << endl;
}

int main(int argc, char** argv) {
  //Argument parsing
  if(argc < 2 || argc > 3) {
    usage(argv[0]);
    return 1;
  }
  bool lbl = false;
  if(argc == 3 && string(argv[2]) == "-l")
    lbl = true;
  
  //Initialize randomness
  srand(time(0));
  
  //Open corpus file
  ifstream ifile(argv[1]);
  if(!ifile) {
    cerr << "Failed to open file." << endl;
    return 1;
  }

  //Instantiate a Markov object and feed it the corpus
  Markov<ACCURACY> m;
  m.readFrom(ifile, lbl);
  //m.print();
  
  //Generate a random Markov chain of strings from the Markov object
  for(int i=0; i<100; ++i)
    cout << m.getMarkovString(0) << endl;
  
  return 0;
}
