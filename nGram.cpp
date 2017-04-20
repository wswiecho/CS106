// Ngram

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "map.h"
#include "random.h"
#include "simpio.h"
#include "vector.h"

using namespace std;

// Propmpts the user for a file name. If incorrect file or the file does
// not open reprompts for a new file name.
void readFile(ifstream &inputFile) {
    string filename = getLine("Input file name? ");
    inputFile.open(filename.c_str());

    // Checks if the input file opens correctly. If not reprompts the user
    // for the new filename.
    while (inputFile.fail()) {
        cout << "The file " << filename << " failed to open. \n";
        filename = getLine("Input file name? ");
        inputFile.open(filename.c_str());
    }
}

// Wrapping around of the gramMap by connecting the last N-1 prefixes in the end
// of the file with the first N-1 words at the start of the file.
void Wrapping(Map<Vector<string>, Vector<string> > & nGram, Vector<string> initialWords, Vector<string> wordsWindow) {

    for (int i = 0; i < initialWords.size(); i++)
    {
        nGram[wordsWindow].add(initialWords[i]);
        for (int j = 0; j < wordsWindow.size()-1; j++) {
            wordsWindow[j] = wordsWindow[j+1];
        }
        wordsWindow[wordsWindow.size()-1] = initialWords[i];
    }
}

// Generates a map with nValue-size keys with words and corresponding most likey counterparts.
void extractData(Map<Vector<string>, Vector<string> > & nGram, ifstream & input, int &nValue) {

    // Declares all variables used in this function.
    bool value = true;
    string word;
    Vector<string> initialWords;
    Vector<string> wordsWindow;

    // Inputs new group elements into the window vector.
    while(wordsWindow.size() < nValue-1) {
        input >> word;
        wordsWindow.add(word);
    }

    // Makes a copy a starting window for the wrapping.
    initialWords = wordsWindow;

    // Builds a map with words by discarding the first (in each iteration)
    //word in the windo.
    while (value) {

        // Gets a new word from a file
        input >> word;
        if (!input.fail()) {

            // Associates a key-wordsWindow with a new word
            nGram[wordsWindow].add(word);

            // The following updates the window with words.
            for (int i = 0; i < wordsWindow.size()-1; i++) {
                wordsWindow[i] = wordsWindow[i+1];
            }

            // Adds the new word to the window.
            wordsWindow[wordsWindow.size()-1] = word;
        }

        // Terminates the loop
        else {
            value = false;
        }
    }

    // Wrapping around of the gramMap.
    Wrapping(nGram, initialWords, wordsWindow);
}

// Gets the right number of words for the N-gram.
void getNumberOfWords(int &numberOfWords, bool &value, int &nValue) {
    numberOfWords= getInteger("\n# of random words to generate (0 to quit)? ");
    if (numberOfWords == 0) {
        value = false;
    }

    // Reprompts the user for the new number of words if number entered is not
    // at least N.
    else if (numberOfWords < nValue) {
        cout << "Number of words must be at least N. \n";
        numberOfWords= getInteger("# of random words to generate (0 to quit)? ");
    }
}

// Prints randomly generated words
void PrintWords(int &numberOfWords, int &randNumber, Vector<string> &wordsWindow,
                Map< Vector<string>, Vector<string> > &nGram) {
    // Declares varirables used in this function.
    string displayWord;

    cout << "... ";
    for (int i = 0; i < numberOfWords; i++) {

        cout << wordsWindow[0] << " ";
        randNumber = randomInteger(0, nGram[wordsWindow].size()-1);
        displayWord = nGram[wordsWindow][randNumber];

        // Updates the window vector.
        for (int j = 0; j < wordsWindow.size()-1; j++) {
            wordsWindow[j] = wordsWindow[j+1];
        }

        // Replaces the last entry in the window vector with the
        // randomy selected word.
        wordsWindow[wordsWindow.size()-1] = displayWord;
    }
    cout << "..." << endl;
}



int main() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams'). \n";
    cout << "This program makes random text based on a document. \n";
    cout << "Give me an input file and an 'N' value for groups \n";
    cout << "of words, and I'll create random text for you. \n" << endl;

    // Declares all structures used in this function.
    bool value = true;
    ifstream input;
    int nValue;
    int numberOfWords;
    int randNumber;
    Map< Vector<string>, Vector<string> > nGram;
    Vector<string> wordsWindow;

    // Reads the input file and checks if it opens
    readFile(input);

    nValue = getInteger("Value of N? ");
    // Checks if N>1.
    if (nValue < 2) {
        cout << "Value must be greater than 1. Try again. \n";
        nValue = getInteger("Value of N? ");
    }

    // Generates the words map given the input file and the value of N.
    extractData(nGram, input, nValue);

    // Prints the random text.
    while (value) {

        //cout << "\n";
        // Establishes number of words displayed.
        getNumberOfWords(numberOfWords, value, nValue);

        // Choses a key at random.
        randNumber = randomInteger(0, nGram.size()-1);
        wordsWindow = nGram.keys()[randNumber];

        // Prints randomly generated words.
        if(value) {
            PrintWords(numberOfWords, randNumber, wordsWindow, nGram);
        }
    }

    // Closes the file.
    input.close();

    cout << "Exiting." << endl;
    return 0;
}
