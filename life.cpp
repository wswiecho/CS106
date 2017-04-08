// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own.
// TODO: remove this comment header!
/*
 *
 *
 *
 *
 * To see certain commands I used the following link to the c++ forum: http://www.cplusplus.com/reference/string/string/substr/.
 */


#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "gwindow.h"
#include "simpio.h"
#include "strlib.h"
#include "lifegui.h"
using namespace std;



// add comment
void nicePlot(int rows, int cols, Grid<char> &grid) {
    LifeGUI name;
    name.resize(rows, cols);

    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {

            if (grid[i][j] == '-') {
                name.drawCell(i, j, false);
            }
            else {
                name.drawCell(i, j, true);
            }
        }
    }

}

// fills the row of the grid with X's and -'s.
void makeGrid(string lineBody, int rowidx, Grid<char> &grid) {
    for (int i=0; i < (int) lineBody.length(); i++) {

        if (lineBody[i] == 'X') {
            grid.set(rowidx, i, 'X');
        }

        else {
            grid.set(rowidx, i, '-');
        }
    }
}


// Extracts information from the file.
void dataExtraction(string fileName, Grid<char> &grid, int &rows, int & cols) {
    ifstream inputFile;
    int rowidx = 0;
    string lineBody, lineRows, lineCols;

        openFile(inputFile, fileName);

        if (inputFile.is_open()) {

        getline(inputFile, lineRows);
        getline(inputFile, lineCols);

        rows = stoi(lineRows);
        cols = stoi(lineCols);

        if (rows != 0 && cols != 0) {
            grid.resize(rows, cols);

            while (!inputFile.eof()) {
                getline(inputFile, lineBody);
                makeGrid(lineBody, rowidx, grid);
                ++ rowidx;
            }
        }

        inputFile.close();
    }

        else {
            cout << "Failed to open " << fileName;
        }

}


// Add a comment
void wrapCounter(int & idx, string wrapping) {
    string wrapArray[4] = {"n", "N", "y", "Y"};

    for(int i = 0; i<4; i++) {
       if (wrapArray[i] == wrapping) {
           ++idx = 1;
       }
    }
}


// Wrapping input formatting
char wrapType(string wrapping) {
    char output;
    int idx = 0;

    wrapping = getLine("Should the simulation wrap around the grid (y/n) ");
    wrapCounter(idx, wrapping);

    while (wrapping.length() > 1 || idx == 0) {
       idx = 0;

       cout << "The wrapping prefix you entered is invalid.\n"
            << "Please enter a single letter indicating your wrapping prefrences." << endl;

       wrapping = getLine("Should the simulation wrap around the grid (y/n) ");
       wrapCounter(idx, wrapping);
    }

    output = tolower(wrapping[0]);
    return output;
}


// Add comment
void animateCounter(int & idx, string animating) {
    string animateArray[6] = {"a", "A", "t", "T", "q", "Q"};

    for(int i = 0; i<6; i++) {
       if (animateArray[i] == animating) {
           ++idx = 1;
       }
    }
}


// Add comment
char animateType(string animating) {
    int idx = 0;

    animating = getLine("a)nimate, t)ick, q)uit? ");
    animateCounter(idx, animating);

    while (animating.length() > 1 || idx == 0) {
       idx = 0;

       cout << "The animating prefix you entered is invalid.\n"
            << "Please enter a single letter for animation type." << endl;

       animating = getLine("a)nimate, t)ick, q)uit? ");
       animateCounter(idx, animating);

    }
    return tolower(animating[0]);
}


// Inputting the rightHand side neightbors.
void rightLeftHandNeighbor(Grid<char> &grid, int &neighbors, int rowsStart, int rowsEnding, int col) {

    for (int i=rowsStart; i<=rowsEnding; i++) {

        if (grid[i][col] != '-') {
            ++ neighbors;
        }
    }
}


// Inputting the center neightbors.
void centerNeighbor(Grid<char> &grid, int &neighbors, int row, int col) {

        if (grid[row][col] != '-') {
            ++ neighbors;
        }
}


// Add comment
char gridElementDecision(int neighbors, char element) {

    switch(neighbors) {
        case 0:
            return '-';
            break;

        case 1:
            return '-';
            break;

        case 2:
            return element;
            break;

        case 3:
            return 'X';
            break;

        default:
            return '-';
            break;
    }
}


// Add comment
void columnSum(Grid<char> &grid, int & neighbors, int i, int k, int j, int cols, char wrapper) {

    if (j==0) {
        if (wrapper == 'n') {
        // Checking Right hand neightbors in the same column.
            rightLeftHandNeighbor(grid, neighbors, i, k,  j+1);
        }

        else {
            rightLeftHandNeighbor(grid, neighbors, i, k,  j+1);
            rightLeftHandNeighbor(grid, neighbors, i, k,  cols-1);
        }
    }

    else if (j>0 && j<cols-1) {
        // Checking Right hand neightbors in the same column.
        rightLeftHandNeighbor(grid, neighbors, i, k,  j+1);

        // Checking Left hand neightbors in the same column.
        rightLeftHandNeighbor(grid, neighbors, i, k,  j-1);
    }

    else if (j==cols-1) {
        if (wrapper == 'n') {
        // Checking Left hand neightbors in the same column.
            rightLeftHandNeighbor(grid, neighbors, i, k,  j-1);
        }
        else {
            rightLeftHandNeighbor(grid, neighbors, i, k,  j-1);
            rightLeftHandNeighbor(grid, neighbors, i, k,  0);
        }
    }
}


// Comment
int animationLoops(char animation) {
    int counter = 0;

    if (animation == 't') {
        counter = 1;
    }

    else if (animation == 'a') {
        int num = 0;

        string format = getLine("How many frames? ");

        for (int i=0; i< (int) format.length(); i++) {

            if (isdigit(format[i])) {
                    ++num;
                }
        }

        while (num != (int) format.length()) {

            cout << "Illigal integer format. Try again." << endl;
            format = getLine("How many frames? ");

            for (int i=0; i< (int) format.length(); i++) {

                if (isdigit(format[i])) {
                    ++num;
                }
            }
        }

        counter = stoi(format);
    }

    return counter;
}


// Growth of bacteria animation
void gridNonWrapping(Grid<char> &grid, char animation, int rows, int cols) {

        if (animation == 'a') {
            cout << "yeah" << endl;
            clearConsole();
            pause(50);
        }

        Grid<char> animationGrid(rows, cols);

        for (int i=0; i<rows; i++) {

            for (int j=0; j<cols; j++) {
                int neighbors = 0;

                if (i==0 && i+1!= rows) {
                // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i, i+1, j, cols, 'n');

                // Checking below element
                    centerNeighbor(grid, neighbors, i+1, j);

                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                 }

                 else if (i<=rows-2) {

                // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i-1, i+1, j, cols, 'n');


                // Checking elements in the same column as the cell of interest.
                    centerNeighbor(grid, neighbors, i-1, j);
                    centerNeighbor(grid, neighbors, i+1, j);

                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                    }

                 else if (i == rows-1) {
                // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i-1, i, j, cols, 'n');

                // Checking above element
                    centerNeighbor(grid, neighbors, i-1, j);

                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                }

            }
            cout << "\n";
       }

       nicePlot(rows, cols, animationGrid);
       grid = animationGrid;

}



// Growth of bacteria animation
void gridWrapping(Grid<char> &grid, char animation, int rows, int cols) {

        if (animation == 'a') {
            cout << "yeah" << endl;
            clearConsole();
            pause(50);
        }

        Grid<char> animationGrid(rows, cols);

        for (int i=0; i<rows; i++) {

            for (int j=0; j<cols; j++) {
                int neighbors = 0;

                if (i==0 && i+1!= rows) {
                // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i, i+1, j, cols, 'y');
                    columnSum(grid, neighbors, rows-1, rows-1, j, cols, 'y');

                // Checking below element
                    centerNeighbor(grid, neighbors, i+1, j);
                    centerNeighbor(grid, neighbors, rows-1, j);

                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                }

                else if (i<=rows-2) {

                // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i-1, i+1, j, cols, 'y');


                // Checking elements in the same column as the cell of interest.
                    centerNeighbor(grid, neighbors, i-1, j);
                    centerNeighbor(grid, neighbors, i+1, j);

                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                }

                else if (i == rows-1) {

                // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i-1, i, j, cols, 'y');
                    columnSum(grid, neighbors, 0, 0, j, cols, 'y');

                // Checking above element
                    centerNeighbor(grid, neighbors, i-1, j);
                    centerNeighbor(grid, neighbors, 0, j);

                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                }

            }
            cout << "\n";

        }

        nicePlot(rows, cols, animationGrid);
        grid = animationGrid;

}



// Simulation of the bacteria procreation.
void bacteriaGrowth(Grid<char> &grid, int rows, int cols, char wrapping, char animation) {

    nicePlot(rows, cols, grid);

    int numberOfLoops = animationLoops(animation);
    // Without wrapping.
    if (wrapping == 'n') {
        // Establishes how many iterations of the grid will be displayed.
        for (int k=0; k<numberOfLoops; k++) {
            gridNonWrapping(grid, animation, rows, cols);
            nicePlot(rows, cols, grid);
        }
    }

    // With wrapping.
    else {
        // Establishes how many iterations of the grid will be displayed.
        for (int k=0; k<numberOfLoops; k++) {
            gridWrapping(grid, animation, rows, cols);
            nicePlot(rows, cols, grid);
        }
    }
}


void animationWrapperGrid(string inputVaribleWrapping, string inputVeriableAnimation, int rows, int cols, Grid<char> &grid, char animation, char wrapping) {
    // Wrapping command and possible errors.
    wrapping =  wrapType(inputVaribleWrapping);
    LifeGUI name;
    name.resize(rows, cols);

    // forms a grid with cell positions.
    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            cout << grid[i][j];
            if (grid[i][j] == '-') {
                name.drawCell(i, j, false);
            }
            else {
                name.drawCell(i, j, true);
            }
        }
        cout << '\n';
    }


    // Wrapping command and possible errors.
    animation =  animateType(inputVeriableAnimation);

    while (!(animation == 'q' || animation == 'Q')) {
    // Simulation of the bacteria procreation.

        int numberOfLoops = animationLoops(animation);
        // Without wrapping.
        if (wrapping == 'n') {
            // Establishes how many iterations of the grid will be displayed.
            for (int k=0; k<numberOfLoops; k++) {
                gridNonWrapping(grid, animation, rows, cols);
                for (int i=0; i<rows; i++) {
                    for (int j=0; j<cols; j++) {

                        if (grid[i][j] == '-') {
                            name.drawCell(i, j, false);
                        }
                        else {
                            name.drawCell(i, j, true);
                        }
                    }
                }
            }
        }

        // With wrapping.
        else {
            // Establishes how many iterations of the grid will be displayed.
            for (int k=0; k<numberOfLoops; k++) {
                gridWrapping(grid, animation, rows, cols);
                for (int i=0; i<rows; i++) {
                    for (int j=0; j<cols; j++) {

                        if (grid[i][j] == '-') {
                            name.drawCell(i, j, false);
                        }
                        else {
                            name.drawCell(i, j, true);
                        }
                    }
                }
            }
        }

        animation =  animateType(inputVeriableAnimation);
    }
}



int main() {

    cout << "Welcome to the CS 106B Game of Life, a simulation of the lifecycle of a bacteria colony. "
         << "Cells (X) live and die by the following rules: \n"
         << " - A cell with 1 or fewer neightboors dies.\n"
         << " - Locations with 2 neightbors remain stable.\n"
         << " - Locations with 3 neightbors will create life. \n"
         << " - A cell with 4 or more neightbors dies. \n" << endl;


    // Defines user based variables.
    string fileName, inputVaribleWrapping, movement, inputVeriableAnimation;
    char wrapping, animation;

    // Taking file name input from the user.
    fileName = getLine("Grid input file name? ");


    if (fileName.compare("random")) {
        ifstream stream;
        stream.open(fileName.c_str());

        while (stream.fail()) {

            cout << "The filename you entered does not exist. Please try again."<< endl;

            fileName = getLine("Grid input file name? ");
            cout << "newfile name " << fileName <<endl;
            stream.open(fileName.c_str());
        }


        // Defines file based variables
        Grid<char> grid;
        int cols, rows;

        // Extracts information from the file.
        dataExtraction(fileName, grid, rows, cols);

        //Add comment
        animationWrapperGrid(inputVaribleWrapping, inputVeriableAnimation, rows, cols, grid, animation, wrapping);
    }

    else {
        double randomNumber;
        int rows = randomInteger(1, 100);
        int cols = randomInteger(1, 100);
        Grid<char> grid(rows, cols);

        for(int i=0; i<rows; i++) {
            for(int j=0; j<cols; j++) {
                randomNumber = randomReal(0, 1);
                if (randomNumber >= .5) {
                    grid.set(i, j, '-');
                }

                else {
                    grid.set(i, j, 'X');
                }
            }
        }

        //Add comment
        animationWrapperGrid(inputVaribleWrapping, inputVeriableAnimation, rows, cols, grid, animation, wrapping);
    }


    cout << "Have a nice Life!" << endl;
    return 0;
}
