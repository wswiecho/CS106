// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "console.h"
#include "filelib.h"
#include "simpio.h"
#include "strlib.h"
#include "console.h"
#include "lexicon.h"

using namespace std;

void fileRead(string &fileName) {
    fileName = getLine("Dictionary file name? ");
            while (fileName != "dictionary.txt") {
                cout << "The filename you entered for the dictionary was incorrect. Try again." << endl;
                fileName = getLine("Dictionary file name? ");
            }
}

// Prompts the user for the two dictionary words and checks their validity.
/*
 * -change to lowercase
 * -the same length
 * -different words
 */


void wordInDict(string &word, Lexicon::Lexicon &english, string num) {
    string combination = "Word # "+ num +"(or Enter to quit): ";
    if(word.length()>0) {
        for(int i=0; i< word.length(); i++) {
            word[i] = tolower(word[i]);
        }

    }
    while(!english.contains(word) && word.length() !=0) {
        cout << "The word you entered is not valid. Try again. \n";
        word = getLine(combination);
    }
}

// Checks if words 1 and 2 are different.
string theSameWords(string &wordOne, string &wordTwo, Lexicon::Lexicon &english){
    while(wordOne.compare(wordTwo) == 0) {
        cout << "Word #2 must be different than Word #1." << endl;
        wordTwo = getLine("Word #2 (or Enter to quit): ");
        wordInDict(wordTwo, english, "2");
    }

    return wordTwo;
}

void wordsOutput(string &wordOne, string &wordTwo, Lexicon::Lexicon &english) {
    // Prompts the user for the first word.
    //wordOne = getLine("Word #1 (or Enter to quit): ");
    wordInDict(wordOne, english, "1");

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
            if (wordOne.length() >0 && wordOne.length() >0) {
                cout << "yeah " << endl;
            }
        }
        else {
            wordTwo = "";
        }
    }

    /*
     *  Finding a word ladder between words w1 and w2:
     *  Create an empty queue of stacks.
        Create/add a stack containing {w1} to the queue. While the queue is not empty:
            Dequeue the partial-ladder stack from the front of the queue.
            For each valid English word that is a "neighbor" (differs by 1 letter)
            of the word on top of the stack:
                If that neighbor word has not already been used in a ladder before:
                    If the neighbor word is w2:
                        Hooray! we have found a solution (and it is the stack you are working on in the queue).
                    Otherwise:
                        Create a copy of the current partial-ladder stack.
                        Put the neighbor word on top of the copy stack.
                 Add the copy stack to the end of the queue.

                 */



    cout << "Have a nice day." << endl;
    return 0;
}
