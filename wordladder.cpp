// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "queue.h"
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "console.h"
#include "lexicon.h"

using namespace std;

// Reads the input file from the user.
void fileRead(string &fileName) {
    fileName = getLine("Dictionary file name? ");
            while (fileName != "dictionary.txt") {
                cout << "The filename you entered for the dictionary was incorrect. Try again." << endl;
                fileName = getLine("Dictionary file name? ");
            }
}

// Checks if a given input word is in the dictionary.
void wordInDict(string &word, Lexicon::Lexicon &english, string num) {
    string combination = "Word #"+ num +" (or Enter to quit): ";
    if(word.length()>0) {
        for(int i=0; i< word.length(); i++) {
            word[i] = tolower(word[i]);
        }

    }
    while(!english.contains(word) && word.length() >0) {
        cout << "The word you entered is not valid. Try again. \n";
        word = getLine(combination);
    }
}

// Checks if words 1 and 2 are different.
string theSameWords(string &wordOne, string &wordTwo, Lexicon::Lexicon &english){
    while(wordOne.compare(wordTwo) == 0 && wordOne.length()>0) {
        cout << "Word #2 must be different than Word #1." << endl;
        wordTwo = getLine("Word #2 (or Enter to quit): ");
        wordInDict(wordTwo, english, "2");
    }

    return wordTwo;
}

void wordsOutput(string &wordOne, string &wordTwo, Lexicon::Lexicon &english) {

    wordInDict(wordOne, english, "1");

    if (wordOne.length() > 0) {

        // Prompts the user for the second word.
        wordTwo = getLine("Word #2 (or Enter to quit): ");
        wordInDict(wordTwo, english, "2");

        // Checks if the word entered is valid.
        int comparison = wordOne.compare(wordTwo);

        // Reprompts the user if Word #1 and Word #2 are the same.
        wordTwo = theSameWords(wordOne, wordTwo, english);

        while(wordOne.length() != wordTwo.length() && wordTwo.length()!=0) {

            if(wordOne.length() != wordTwo.length()) {

                cout << "The length of Word # 2 is incorrect. Enter a word of length "
                 << wordOne.length() << "." << endl;
            }

            else {
                cout << "Invalid word." << endl;
            }

            // Reprompts the user for new Word #2.
            wordTwo = getLine("Word #2 (or Enter to quit): ");
            wordInDict(wordTwo, english, "2");
            wordTwo = theSameWords(wordOne, wordTwo, english);
        }
    }
}

// Generates the ladder of words using the algorithm.
void ladder(string wordOne, string wordTwo, Lexicon::Lexicon &english) {

    // Defines all structures that will be used in this function.
    Set<string> setOfWords;
    Stack<string> initialStack, partialLadderHolder, updatedStack;
    string possibleNewWord, wordHolder;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";
    Queue< Stack<string> > ladderWithWords;

    // Adds Word #1 to stack and then adds the stack to the queue.
    initialStack.push(wordOne);
    ladderWithWords.enqueue(initialStack);

    // Preserves unique words that were unsed in the ladder.
    setOfWords.add(wordOne);

    // Defines the ladder between two words if exists.
    while (!ladderWithWords.isEmpty()) {

        // Gets the partial-ladder stack from the queue.
        partialLadderHolder = ladderWithWords.dequeue();

        // Gets the last word in the partial ladder.
        wordHolder = partialLadderHolder.peek();

        // Prints the output of the shortest ladder if such exists.
        if (wordHolder == wordTwo) {

            cout << "Ladder from " << wordTwo << " back to "<< wordOne << ":" << endl;

            // Output the elements of the stack as the solution.
            while(!partialLadderHolder.isEmpty()) {

                cout << partialLadderHolder.pop();
                if (!partialLadderHolder.isEmpty()) {
                    cout << " ";
                }
            }

            cout << "\n";
        }

        /* Gets the neighboring words by iterating over the original word and
         * all letters in the alphabet. Also appends new word to the ladder if
         * it wasn't used previously. Finally, updates a set, a copy of the
         * stack and the queue with the stack words with the new word.
         */
        else {

            for (int i = 0; i < wordHolder.length(); i++) {

                for (int j = 0; j < alphabet.length(); j++) {

                    // Changes the ith position of the wordHolder with the jth letter of the alphabet.
                    possibleNewWord = wordHolder;
                    possibleNewWord[i] = alphabet[j];

                    if (!setOfWords.contains(possibleNewWord) && english.contains(possibleNewWord)) {

                        // Updates a set, a stack and the queue with the stack words with the new word.
                        setOfWords.add(possibleNewWord);
                        updatedStack = partialLadderHolder;
                        updatedStack.push(possibleNewWord);
                        ladderWithWords.enqueue(updatedStack);
                    }
                }
            }
        }
    }

    // Prints statement if no ladder was found.
    cout << "No word ladder found from " << wordTwo << " back to " << wordOne << "." << endl;
  }



int main() {
    cout << "Welcome to CS 106 Word Ladder \n";
    cout << "Please give me two English words, and I will change the \n";
    cout << "first into the second by changing one letter at a time. \n" << endl;

    // Defines input variables from the user;
    string fileName = "fileName";
    string wordOne = "wordOne";
    string wordTwo = "wordTwo";

    // Prompts user for the dictionary file name.
    fileRead(fileName);

    // Defines the Lexicon
    Lexicon english(fileName);

    // Generates two valid english words of the same size.

    while(wordOne.length() >0 && wordTwo.length() >0) {
        wordOne = getLine("Word #1 (or Enter to quit): ");

        if(wordOne.length() >0) {
            wordsOutput(wordOne, wordTwo, english);

            if (wordOne.length() >0 && wordTwo.length() >0) {
                // Generates the ladder of words using the algorithm.
                ladder(wordOne, wordTwo, english);
            }
        }
    }

    cout << "\nHave a nice day." << endl;
    return 0;
}
