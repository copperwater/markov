###Markov chain utility for strings and characters.

This is a program that uses Markov chains to generate new strings from a corpus. 

There are two headers provided, Markov.h and MarkovChar.h.

Markov.h implements the Markov class, which can be used to generate lists of strings. 
You can use it in "corpus" mode, which reads the entire corpus and generates associations without respecting line breaks, or in "line-by-line" mode, which generates line-independent associations.

MarkovChar.h implements the MarkovChar class, which can be used to generate single strings of letters.
