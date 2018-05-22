# Finite Automata
By Elisabetta Caldesi

Files
-------
- dfa-international.py: program that, when given one file that represents a set of possibly many input strings and another that represents a rule set for a deterministic finite automaton (DFA), computes the states that the DFA transitions through for each input string.
- nfa2dfa-international.py: program that takes the rule set for a NFA and converts it into a rule set for an equivalent DFA.

Details
--------

The key data structures used were dictionaries, lists, tuples, and sets. For the dfa, I used a default dictionary that would have a key and a value as a list. The key represented the state and the value (list) represented the possible paths from that state. Ex: {1: [ [b, 2], [a, 1] ] }. For the nfa, instead, I used a simpler dictionary that has a key and a series of lists for value based on the paths possible from that specific state. Ex {1: [b, 1, 2], [a, 3] }. The states, the alphabet and accept states were stored in a list. I used mostly lists, then the dictionary for my states and paths and used sets when I had to get unique values not repeated, for example to check that there wouldn’t be something like {1: [b,2] [a, 3], [b, 1] }, which would be {1: [b, 2, 1], [a, 3] }. I'm assuming that the dictionary won’t print in numerical order and neither will the states in the value for each key, this won’t impact anything other than the format of the output. I am also assuming that the user will input states for the nfa that are either numbers or are q+ any number, just to simplify code. The NFA program outputs the DFA to a file, while the DFA program outputs the paths and if the string was accepted or rejected to stdout. <br />
