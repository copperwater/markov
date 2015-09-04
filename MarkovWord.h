#ifndef MARKOV_H
#define MARKOV_H

#include <iostream> //object printing
#include <sstream> //stringstreams are used in line-by-line reading
#include <map> //the major markov map

//Helper class for Markov.
//A source is a group of N strings that are used as the basis for calculating
//the next string. Each represents one state in the state machine
//interpretation of the Markov chain.
template <unsigned int N> class source {
  typedef unsigned int uint;
public:
  std::string strs[N];

  //Default string is a start source
  source() {
    for(uint i=0; i<N; ++i) strs[i] = "";
  }
  //Add a new string to the end of the source, rotating everything else back
  void rotate(const std::string& next) {
    for(uint i=0; i<N-1; ++i) strs[i] = strs[i+1];
    strs[N-1] = next;
  }
  //used for comparing in the markov map
  bool operator<(const source<N>& oth) const {
    uint i;
    for(i=0; i<N-1; ++i) {
      if(strs[i] != oth.strs[i]) break;
    }
    return strs[i] < oth.strs[i];
  }
  //Print all the strings in the source
  void print() const {
    std::cout << "( ";
    for(uint i=0; i<N; ++i)
      std::cout << strs[i] << " ";
    std::cout << ")";
  }
  //Determine whether this is a "start" source (if all strings are empty)
  bool isStartSource() const {
    for(uint i=0; i<N; ++i)
      if(strs[i] != "") return false;
    return true;
  }   
};

//The actual Markov class.
template <unsigned int N> class Markov {
 private:
  typedef unsigned int uint;
  
  //Structure containing all the mappings
  std::map<source<N>, std::map<std::string, uint> > markov_maps; 

  //Gets the next string in the chain randomly
  std::string getNext(source<N> s) {
    std::map<std::string, uint> m = markov_maps[s];
    uint tot = 0;
    for(std::map<std::string,uint>::const_iterator itr=m.begin();
	itr!=m.end(); ++itr) {
      tot += itr->second;
    }
    int rnd = rand() % tot;
    for(std::map<std::string,uint>::const_iterator itr=m.begin();
	itr!=m.end(); ++itr) {
      rnd -= itr->second;
      if(rnd < 0) {
	return itr->first;
      }
    }
  }

  //Gets a random source from the list
  source<N> randSource() {
    typename std::map<source<N>,std::map<std::string,uint> >::const_iterator it
      = markov_maps.begin();
    std::advance(it, rand() % markov_maps.size());
    return it->first;
  }

 public:
  //Generate Markov associations by reading in from a corpus
  void readFrom(std::istream& i, const bool linebyline) {
    source<N> src;
    std::string tmp;
    if(linebyline) {
      std::string tmp2;
      getline(i, tmp);
      std::stringstream s(tmp);
      do {
	while(s >> tmp2) {
	  markov_maps[src][tmp2]++;
	  src.rotate(tmp2);
	}
	markov_maps[src][""]++;
	getline(i, tmp);
	s.clear();
	s.str(tmp);
	src = source<N>();
      } while(!i.eof());
    } else {
      while(i >> tmp) {
	markov_maps[src][tmp]++;
	src.rotate(tmp);
      }
    }
  }

  //Get Markov chain of strings
  std::string getMarkovString(int num) {
    std::string out;
    source<N> src;
    std::string tmp;
    if(num < 1) {
      do {
	tmp = getNext(src);
	out.append(" "+tmp);
	src.rotate(tmp);
      } while(tmp != "");
    } else {
      src = randSource();
      for(int i=0; i<num; ++i) {
	tmp = getNext(src);
	if(tmp == "") break;
	out.append(" "+tmp);
	src.rotate(tmp);
      }
    }
    out.erase(0,1);
    return out;
  }

  //Dump all sources and associated words with their frequencies.
  void print() const {
    for(typename std::map<source<N>,std::map<std::string,uint> >::const_iterator it
	  = markov_maps.begin(); it != markov_maps.end(); it++) {
      std::cout << std::endl;
      it->first.print();
      std::cout << std::endl;
      std::map<std::string,uint> tmp = it->second;
      for(std::map<std::string,uint>::const_iterator it2=tmp.begin();
	  it2!=tmp.end(); it2++) {
	std::cout << "    " << ((it2->first == "") ? "EOL" : it2->first)
			   << " " << it2->second << std::endl;
      }
    }
  }
};
    
#endif
