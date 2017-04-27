/*
 * File: grammarsolver.cpp
 * --------------------------
 * Name: Weronika J Swiechowicz
 * Section leader: John Pericich
 * This file contains grammar generating code for CS106B.
 *
 */

#include "grammarsolver.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include <algorithm>
#include "strlib.h"
#include "vector.h";

using namespace std;

// Reads the file in and fills the dictionary with rules and words.
void readFile(Map< string, Vector < Vector < string > > > &language, istream &inputFile) {
    string phrase;

    while ( getline( inputFile, phrase ) ) {

        // Defines all variables used in this loop.
        int rulesSize, tokenSize;
        string allRulesInOne, ruleTokens;
        Vector < string > tokenRulesSet;
        Vector < Vector <string> > rule;

        // Reads nonterminals elements.
        tokenSize = phrase.find_first_of( ':' );
        ruleTokens = phrase.substr( 0, tokenSize );

        // Obtains a particular grammar rule.
        rulesSize = tokenSize + 3;
        allRulesInOne = phrase.substr( rulesSize );

        // Separates grammar rules by |.
        if ( language.containsKey( ruleTokens ) ) throw "Multiple symbols";
        if ( string:: npos != allRulesInOne.find( "|" ) )
            tokenRulesSet = stringSplit( allRulesInOne, "|" );
        else tokenRulesSet.add( allRulesInOne );

        // Reads in the associated with a token in each line.
        for (string definition: tokenRulesSet) {

            if (string ::npos == definition.find(" ")) {
                 Vector < string > multipleRules;
                 multipleRules.add( definition );
                 rule.add( multipleRules );
            }
            else rule.add(stringSplit( definition, " " ) );
        }

        language.put(ruleTokens, rule);
    }
}

// Populates the dictionary with additional words.
void languageBuilder(Map <string, Vector < Vector<string> > > &language,
                     string symbol, Vector< string> &wordsVector) {
    // Declares variables used by this function.
    Vector <string> additionWords;

    // Performs recursion and adds new words to a vector associated with a particular symbol.
    if ( language.containsKey( symbol ) ) {
        additionWords = language.get(symbol).get(randomInteger(0, (language.get(symbol).size() - 1)));
        for (string token : additionWords) languageBuilder(language, token, wordsVector);
    }

    // Base case
    else wordsVector.add( symbol );
}


/**
 * Generates grammar for a given symbol a certain number of times given
 * a BNF input file.
 *
 * This will be called by grammarmain.cpp.
 *
 * @param input - Input stream of BNF file.
 * @param symbol - Symbol to generate
 * @param times - Number of times grammar is generated
 * @return Vector of strings of size times with random generations of symbol
 */


// Reads the file in and generates a sequence of times random elements associated with symbol
// in the language dictionary.
Vector< string > grammarGenerate(istream& input, string symbol, int times) {

    // Handles possible input problems.
    if (symbol == "") throw "Symbol parameter passed to function is empty.";

    // Structures used by this function
    Map < string, Vector< Vector < string > > > language;
    Vector< string > wordsVector;

    // Reads the file in and generates language rules tree.
    readFile(language, input);

    // Generates a sequence of times random elements associated with symbol
    // in the language dictionary.
    for ( int i = 0; i < times; i++ ) {
        Vector< string > addition;
        languageBuilder( language, symbol, addition );

        // Strips {" and "} from the wordsSequence and the following strips the rest
        string wordsSequence = addition.toString().substr(2, addition.toString().length()-4);         
        for ( int j = 0; j < (int)wordsSequence.length(); j++ ) {
            if( wordsSequence[ j ] == '\"' && wordsSequence[ j+1 ] == ',') {
                wordsSequence = wordsSequence.substr( 0, j ) + " " + wordsSequence.substr( j + 4, wordsSequence.length() - 1 );
                j = j-4;
            }
        }
        wordsVector.add( wordsSequence );
    }
    return wordsVector;
}
