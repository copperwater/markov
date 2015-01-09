#ifndef MARKOVCHAR_H
#define MARKOVCHAR_H

/* The exact same Markov code as in Markov.h, but this is specifically for
   generating single "words" rather than phrases, so it uses chars instead of
   strings. */

#include <map>
#include <cassert>

template <unsigned int N> class sourceChr {
  typedef unsigned int uint;
public:
  char chrs[N];

  //Default string is a start sourceChr
  sourceChr() {
    for(uint i=0; i<N; ++i) chrs[i] = '\0';
  }
  //Add a new string to the end of the sourceChr, rotating everything else back
  void rotate(const char next) {
    for(uint i=0; i<N-1; ++i) chrs[i] = chrs[i+1];
    chrs[N-1] = next;
  }
  //used for comparing in the markov map
  bool operator<(const sourceChr<N>& oth) const {
    uint i;
    for(i=0; i<N-1; ++i) {
      if(chrs[i] != oth.chrs[i]) break;
    }
    return chrs[i] < oth.chrs[i];
  }
  //Print all the strings in the sourceChr
  void print() const {
    std::cout << "( ";
    for(uint i=0; i<N; ++i)
      if(chrs[i])
	std::cout << chrs[i] << " ";
    else
      std::cout << "NULL "; //print NULL for \0
    std::cout << ")";
  }
  //Determine whether this is a "start" sourceChr (if all chars are null)
  bool isStartSourceChr() const {
    for(uint i=0; i<N; ++i)
      if(chrs[i] != '\0') return false;
    return true;
  }  
};

template <unsigned int N> class MarkovChar {
 private:
  typedef unsigned int uint;
  
  //Structure containing all the mappings
  std::map<sourceChr<N>, std::map<char, uint> > markov_maps; 
  
  //Gets the next character in the chain randomly
  char getNext(sourceChr<N> s) {
    std::map<char, uint> m = markov_maps[s];
    uint tot = 0;
    for(std::map<char,uint>::const_iterator itr=m.begin(); itr!=m.end(); ++itr) {
      tot += itr->second;
    }
    int rnd = rand() % tot;
    for(std::map<char,uint>::const_iterator itr=m.begin(); itr!=m.end(); ++itr) {
      rnd -= itr->second;
      if(rnd < 0) {
	return itr->first;
      }
    }
  }

  //Gets a random sourceChr from the list
  sourceChr<N> randSourceChr() {
    typename std::map<sourceChr<N>,std::map<char,uint> >::const_iterator it
      = markov_maps.begin();
    std::advance(it, rand() % markov_maps.size());
    return it->first;
  }

  //Scan the tree of sourceChrs N deep.
  //If all paths lead to a null then return true.
  bool isDeadEnd(sourceChr<N> src, int depth) {
    if(depth == 0) return false;
    if(markov_maps[src].size() == 1 && markov_maps[src].begin()->first == '\0')
      return true;
    sourceChr<N> tmpsrc = src;
    for(std::map<char,uint>::const_iterator it = markov_maps[src].begin();
	it != markov_maps[src].end(); ++it) {
      tmpsrc = src;
      tmpsrc.rotate(it->first);
      if(!isDeadEnd(tmpsrc, depth-1)) return false;
    }
    return true;
  }

  //Get a char that is guaranteed NOT to lead to a dead end in x steps.
  char getNextGuarantee(sourceChr<N> src, int x) {
    assert(!isDeadEnd(src,x));
    std::map<char,uint> tmp;
    std::map<char,uint> m = markov_maps[src];
    uint tot = 0;
    for(std::map<char,uint>::const_iterator itr=m.begin(); itr!=m.end(); ++itr) {
      sourceChr<N> tmpsrc = src;
      tmpsrc.rotate(itr->first);
      if(!isDeadEnd(tmpsrc, x)) {
	tmp[itr->first] = itr->second;
	tot += itr->second;
      }
    }
    int rnd = rand() % tot;
    for(std::map<char,uint>::const_iterator itr=tmp.begin(); itr!=tmp.end(); ++itr) {
      rnd -= itr->second;
      if(rnd < 0) {
	return itr->first;
      }
    }
  }

 public:
  
  //Generate Markov associations by reading in from a corpus
  void readFrom(std::istream& i) {
    std::string tmp;
    char c;
    while(i >> tmp) {
      sourceChr<N> src;
      for(int i=0; i<tmp.length(); ++i) {
	c = tmp[i];
	markov_maps[src][tmp[i]]++;
	src.rotate(tmp[i]);
      }
      markov_maps[src]['\0']++;
    }
  }

  //Get Markov string of chars
  std::string getMarkovString(unsigned char length) {
    std::string out;
    sourceChr<N> src;
    if(length < 1) {
      //This means that the user does not care about the length
      char tmp = getNext(src);
      while(tmp != '\0') {
	out.append(1,tmp);
	src.rotate(tmp);
	tmp = getNext(src);
      }
      return out;
    }
    for(int i=0; i<length; ++i) {
      char tmp = getNextGuarantee(src, (i>length-N ? length-i : N));
      out.append(1,tmp);
      src.rotate(tmp);
    }
    return out;
  }

  void print() const {
    for(typename std::map<sourceChr<N>,std::map<char,uint> >::const_iterator it
	  = markov_maps.begin(); it != markov_maps.end(); it++) {
      std::cout << std::endl;
      it->first.print();
      std::cout << std::endl;
      std::map<char,uint> tmp = it->second;
      for(std::map<char,uint>::const_iterator it2=tmp.begin();
	  it2!=tmp.end(); it2++) {
	std::cout << "    " << ((it2->first == '\0') ? '~' : it2->first)
		  << " " << it2->second << std::endl;
      }
    }
  }
};
    
#endif
