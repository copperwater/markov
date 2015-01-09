DFLAGS = -g

all: test char_test

test: test.o
	g++ $(DFLAGS) test.o -o test

test.o: test.cpp markov.h
	g++ -c $(DFLAGS) test.cpp

char_test: char_test.o
	g++ $(DFLAGS) char_test.o -o char_test

char_test.o: char_test.cpp
	g++ -c $(DFLAGS) char_test.cpp

clean:
	rm -f *.o
