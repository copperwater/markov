DFLAGS = -g

all: markov-char markov-word

markov-word: markov-word.o
	g++ $(DFLAGS) markov-word.o -o markov-word

markov-word.o: markov-word.cpp MarkovWord.h
	g++ -c $(DFLAGS) markov-word.cpp

markov-char: markov-char.o
	g++ $(DFLAGS) markov-char.o -o markov-char

markov-char.o: markov-char.cpp MarkovChar.h
	g++ -c $(DFLAGS) markov-char.cpp

clean:
	rm -f *.o markov-word markov-char
