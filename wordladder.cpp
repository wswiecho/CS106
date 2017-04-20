/* This program repeatedly prompts the user for two words to ultimately find a minimum-length
 * ladder between the two entered words. The program is writtien in such a way that words are
 * checked against english dictionary (wordInDict()), if they are of the same length (sameLength())
 * and finally if they are in fact different words (theSameWords()).To achieve that, the program
 * uses a set of structures but mostly concentrates on the use of queues, sets and stacks
 * to store a series of words.
 *
 * The shortest ladder is produced using a given algorithm which was translated into the function
 * ladder(). That function is exectured only when the two words meet all the conditions. The
 * ladder() prints the shortes word path between the two words if such exists, if not a message
 * of nonexistance is printed instead.
 */

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
    ifstream inputFile;
    fileName = getLine("Dictionary file name? ");
            while (fileName != "dictionary.txt") {
                cout << "Unable to open that file. Try again." << endl;
                fileName = getLine("Dictionary file name? ");
            }

    // Checks if the dictionary file opens correctly.
    inputFile.open(fileName.c_str());

    if(inputFile.fail()) {
        cout << "Failed to open dictionary.txt";
    }
    inputFile.close();
}

// Changes the letters in a word to lower case.
void allLowerCase(string &word) {
    for(int i=0; i< word.length(); i++) {
        word[i] = tolower(word[i]);
    }
}

// Checks if a given input word is in the dictionary.
bool wordInDict(string &wordOne, string &wordTwo, Lexicon::Lexicon &english) {
    bool value = true;
    allLowerCase(wordOne);
    allLowerCase(wordTwo);

    if(!english.contains(wordOne) && !english.contains(wordTwo)) {
        cout << "The two words must be found in the dictionary. \n";
        value = false;
    }

    return value;
}

// Checks if the entered words are different.
bool theSameWords(string &wordOne, string &wordTwo, Lexicon::Lexicon &english) {
    bool value = true;
    if(wordOne.compare(wordTwo) == 0) {
        cout << "The words must be different. \n";
        value = false;
    }
    return value;
}


// Generates the ladder of words using the algorithm.
void ladder(string wordOne, string wordTwo, Lexicon::Lexicon &english) {

    // Defines all structures that will be used in this function.
    int idx = 0;
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
            idx = 1;
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
    if (idx == 0) {
        // Prints statement if no ladder was found.
        cout << "No word ladder found from " << wordTwo << " back to " << wordOne << "." << endl;
    }
  }

bool sameLength(string &wordOne, string &wordTwo) {
    bool value = true;
    if(wordOne.length() != wordTwo.length()) {
        cout << "The two words must be the same length. \n";
        value = false;
    }
    return value;
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

    // Runs the loop for words input and possibly prints the word ladder if input was valid.
    while(wordOne.length() >0 && wordTwo.length() >0) {
        cout << "\n";
        wordOne = getLine("Word #1 (or Enter to quit): ");

        if(wordOne.length() >0) {
            wordTwo = getLine("Word #2 (or Enter to quit): ");

            if (wordTwo.length() > 0) {

                // Checks if the words entered meet all the conditions and prints the word ladder.
                if (wordInDict(wordOne, wordTwo, english) && theSameWords(wordOne, wordTwo, english)
                        && sameLength(wordOne, wordTwo)) {
                        ladder(wordOne, wordTwo, english);
                }
            }
        }
    }

    cout << "Have a nice day." << endl;
    return 0;
}
