/* life.cpp
 * The program animates a procreation cycle of a bacteria colony. It takes as an input user input file containing the grid
 * information and initial state of the colony (infromation exctracted using dataExtraction function). It then promts the
 * user for specification if the animation should wrap around the grid (code accounds for the possibilility of invalid
 * input in wrapType and wrapCounter). Next, it displays the initial colony state and prompts the user for the number of
 * cycles he/she wants performed on the colony (handeled by animateType and animateCounter functions). Once again, the
 * possibility of invalid input is handeled by reprompting the user to select a valid option. The program terminates with
 * selection of q, or Q option.
 * The life of colony is defined by 3 states: less than 2 cells in the 8-grid around the cell means that the cell dies, 2
 * neighbors preserves the current state of the cell, 3 creates a new cell and more than 3 neightbors kills the cell
 * (handeled by the gridElementDecision function, while the grid itself is set using makeGrid).
 *
 * I attempted two extra features: Random word generation and Graphical display. The code for the display is contained in
 * the nicePlot function and animationWrapperGrid to update for each new colony state. The code for the random word
 * generation is contained in main 512-574 and uses functions defined for the regular file to generate the grid and each
 * consecutive state of the bacteria.
 *
 * To see how to change string to an integer I used the following link to the c++ forum:
 * http://www.cplusplus.com/reference/string/string/substr/.
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


// Generates gui diplay of the bacteria procreation cycle.
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

// Fills the the grid with X's and -'s.
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

// Extracts information from the user input file.
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

// Checks if the user input for wrapping is one of 4 assumed options.
void wrapCounter(int & idx, string wrapping) {
    string wrapArray[4] = {"n", "N", "y", "Y"};

    for(int i = 0; i<4; i++) {
        if (wrapArray[i] == wrapping) {
           idx = 1;
       }
    }
}

// Wrapping input formatting. Established type of wrapping by prompting a user to command
// and checks if the input is valid. Reprompts for input if input was invalid.
char wrapType(string wrapping) {
    char output;
    int idx = 0;

    wrapping = getLine("Should the simulation wrap around the grid (y/n) ");
    wrapCounter(idx, wrapping);

    while (wrapping.length() > 1 || idx == 0) {
       idx = 0;
       cout << "The wrapping prefix you entered is invalid. Try again." << endl;
       wrapping = getLine("Should the simulation wrap around the grid (y/n) ");
       wrapCounter(idx, wrapping);
    }
    output = tolower(wrapping[0]);
    return output;
}

// Checks if the user input for animation is one of 6 assumed options.
void animateCounter(int & idx, string animating) {
    string animateArray[6] = {"a", "A", "t", "T", "q", "Q"};

    for(int i = 0; i<6; i++) {
       if (animateArray[i] == animating) {
           ++idx = 1;
       }
    }
}

// Wrapping input formatting. Established type of wrapping by prompting a user to command
// and checks if the input is valid. Reprompts for input if input was invalid.
char animateType(string animating) {
    int idx = 0;

    animating = getLine("a)nimate, t)ick, q)uit? ");
    animateCounter(idx, animating);

    while (animating.length() > 1 || idx == 0) {
       idx = 0;

       cout << "The animating prefix you entered is invalid. Please try again." << endl;
       animating = getLine("a)nimate, t)ick, q)uit? ");
       animateCounter(idx, animating);

    }
    return tolower(animating[0]);
}


// Counts the number of neighbors in all rows in the left/right columns adjacent to the cell
// of interest. Achieves that by taking as an input the range of rows to examine and the actual column
void rightLeftHandNeighbor(Grid<char> &grid, int &neighbors, int rowsStart, int rowsEnding, int col) {

    for (int i=rowsStart; i<=rowsEnding; i++) {
        if (grid[i][col] != '-') {
            ++ neighbors;
        }
    }
}

// Counts the number of neighbors in the same column as the cell of interest.
void centerNeighbor(Grid<char> &grid, int &neighbors, int row, int col) {
        if (grid[row][col] != '-') {
            ++ neighbors;
        }
}

// Established the state of the cell based on the number of neighbors.
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


// Established the number of neighbors based on the column in which the cell of intetst is located
// and if wrapping occures.
void columnSum(Grid<char> &grid, int & neighbors, int i, int k, int j, int cols, char wrapper) {
    if (j==0) {
        // No wrapping only checks the number of neighbors in the right hand column.
        if (wrapper == 'n') {
            rightLeftHandNeighbor(grid, neighbors, i, k,  j+1);
        }
        // Wrapping occures - checks the number of neighbors in the right and left hand columns.
        // For for first column in the grid it wraps to check the last column in the grid as the left hand column.
        else {
            rightLeftHandNeighbor(grid, neighbors, i, k,  j+1);
            rightLeftHandNeighbor(grid, neighbors, i, k,  cols-1);
        }
    }

    else if (j>0 && j<cols-1) {
        // Counts neighbors in the right (first) and left (second) hand columns.
        rightLeftHandNeighbor(grid, neighbors, i, k,  j+1);
        rightLeftHandNeighbor(grid, neighbors, i, k,  j-1);
    }

    else if (j==cols-1) {
        // No wrapping only checks the number of neighbors in the left hand column.
        if (wrapper == 'n') {
            rightLeftHandNeighbor(grid, neighbors, i, k,  j-1);
        }
        // Wrapping occures - checks the number of neighbors in the right and left hand columns.
        // For for last column in the grid it wraps to check the first column in the grid as the right hand column.
        else {
            rightLeftHandNeighbor(grid, neighbors, i, k,  j-1);
            rightLeftHandNeighbor(grid, neighbors, i, k,  0);
        }
    }
}

// Checks if user input is an integer.
int correctInputInteger(string format, int num) {
    for (int i=0; i< (int) format.length(); i++) {
        if (isdigit(format[i])) {
                ++num;
            }
    }
    return num;
}

// Established how many times an animation is performed and reprompts the user if the input was invalid.
// Returns integer stating the number of cycles of bacteria procreation will be performed.
int animationLoops(char animation) {
    int counter = 0;

    // Animation for the tick option.
    if (animation == 't') {
        counter = 1;
    }

    else if (animation == 'a') {
        string format = getLine("How many frames? ");

        // Checks if the input was an integer.
        int num = correctInputInteger(format, 0);

        // Reprompts the user if the input was not an integer.
        while (num != (int) format.length()) {
            cout << "Illigal integer format. Try again." << endl;
            format = getLine("How many frames? ");
            int num = correctInputInteger(format, 0);
        }
        // If input was an integer changes it from a string format to an int.
        counter = stoi(format);
    }
    return counter;
}


// Growth of bacteria animation based on the non wrapping animation.
void gridNonWrapping(Grid<char> &grid, char animation, int rows, int cols) {
    Grid<char> animationGrid(rows, cols);

    // Clears the consol and pauses before new iteration output. Only for animation
    if (animation == 'a') {
        cout << "yeah" << endl;
        clearConsole();
        pause(50);
    }

    // Defines elements of the new state of the grid.
    for (int i=0; i<rows; i++) {

        for (int j=0; j<cols; j++) {
                int neighbors = 0;
                // Executes for the first row.
                if (i==0 && i+1!= rows) {
                    // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i, i+1, j, cols, 'n');

                    // Checking below element of the grid.
                    centerNeighbor(grid, neighbors, i+1, j);

                    // Updates and displays the new grid element.
                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];

                }
                // Executes for rows 2 to n-1.
                else if (i<=rows-2) {
                  // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i-1, i+1, j, cols, 'n');

                    // Checking elements in the same column as the cell of interest.
                    centerNeighbor(grid, neighbors, i-1, j);
                    centerNeighbor(grid, neighbors, i+1, j);

                    // Updates and displays the new grid element.
                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                }

                // Executes for the last row.
                else if (i == rows-1) {
                    // Gets the left and right hand side columns handeled.
                    columnSum(grid, neighbors, i-1, i, j, cols, 'n');

                    // Checking above element.
                    centerNeighbor(grid, neighbors, i-1, j);

                    // Updates and displays the new grid element.
                    animationGrid[i][j] = gridElementDecision(neighbors, grid[i][j]);
                    cout << animationGrid[i][j];
                }
        }
        cout << "\n";
    }
    // Displays gui grid.
    nicePlot(rows, cols, animationGrid);
    // Replaces old grid with the updated version.
    grid = animationGrid;
}

// Growth of bacteria animation based on the wrapping animation. Similar to the
// above functions with addition neighbors counts because of wrapping.
void gridWrapping(Grid<char> &grid, char animation, int rows, int cols) {
    Grid<char> animationGrid(rows, cols);

        if (animation == 'a') {
            cout << "yeah" << endl;
            clearConsole();
            pause(50);
        }

        for (int i=0; i<rows; i++) {

            for (int j=0; j<cols; j++) {
                int neighbors = 0;
                // Animation for the last row which wraps around to the last row.
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

                // Animation for the last row which wraps around to the first row.
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
    // Displays gui grid with the locations of living bacterias.
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

// Generates a new life cycle of a bacteria by checking the type of animation and if wrapping is occuring.
// Also displays the new grid, as well as the uit window.
void animationWrapperGrid(string inputVaribleWrapping, string inputVeriableAnimation, int rows, int cols,
                          Grid<char> &grid, char animation, char wrapping) {
    // Wrapping command check.
    wrapping =  wrapType(inputVaribleWrapping);
    // Initializes gui display.
    LifeGUI name;
    name.resize(rows, cols);

    // Forms a grid with cell positions and displays them in the consol and gui window.
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

    // Animation command check.
    animation =  animateType(inputVeriableAnimation);

    while (!(animation == 'q' || animation == 'Q')) {
        // Simulation of the bacteria procreation.
        int numberOfLoops = animationLoops(animation);

        // Without wrapping.
        if (wrapping == 'n') {
            // Establishes how many iterations of the grid will be displayed.
            for (int k=0; k<numberOfLoops; k++) {
                gridNonWrapping(grid, animation, rows, cols);

                // Updates display for the gui window
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

                // // Updates display for the gui window
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
        // Prompts user for the animation input and checks its validity.
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

    // Handles file name input of random and performs all animation as for a regular file.
    else {
        double randomNumber;
        // Initializes a random number of columns and rows that can range from 1 to 100.
        int rows = randomInteger(1, 100);
        int cols = randomInteger(1, 100);
        Grid<char> grid(rows, cols);

        // Sets a grid with a random number of rows and columns using a random selection criteria for alive and
        // death cells (if random real number is [0, .5) inputs -, else inputs X).
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
        // Performs animation and updates of the grid.
        animationWrapperGrid(inputVaribleWrapping, inputVeriableAnimation, rows, cols, grid, animation, wrapping);
    }
    cout << "Have a nice Life!" << endl;
    return 0;
}
