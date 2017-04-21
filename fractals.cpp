/*
 * File: fractals.cpp
 * --------------------------
 * Name:
 * Section leader:
 * This file contains fractal problems for CS106B.
 */


#include "fractals.h"
#include <cmath>
#include <math.h>

using namespace std;

const int LEAF_COLOR = 0x2e8b57;   /* Color of all leaves of recursive tree (level 1) */
const int BRANCH_COLOR = 0x8b7765; /* Color of all branches of recursive tree (level >=2) */

/**
 * Draws a Sierpinski triangle of the specified size and order, placing its
 * top-left corner at position (x, y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Sierpinski triangle.
 * @param x - The x coordinate of the top-left corner of the triangle.
 * @param y - The y coordinate of the top-left corner of the triangle.
 * @param size - The length of one side of the triangle.
 * @param order - The order of the fractal.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, double size, int order) {
    // TODO: write this function

    if (x < 0 || y < 0 || size < 0 || order < 0) {
        throw "Input values must be nonnegative.";
    }

    // Base case draws a single trainlge of order 1.
    if (order == 1) {
        // draw the triangle
        double x1 = x + size;
        double x2 = x + size / 2;
        double y2 = y + sqrt(3)* size / 2;
        gw.drawLine(x, y, x1, y);
        gw.drawLine(x, y, x2, y2);
        gw.drawLine(x1, y, x2, y2);
    }

    // Draws three triangles of order: order-1.
    if (order > 1) {
        double x1 = x + size/2;
        double x2 = x + size / 4;
        double y2 = y + sqrt(3)* size / 4;
        drawSierpinskiTriangle(gw, x, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x1, y, size/2, order-1);
        drawSierpinskiTriangle(gw, x2, y2, size/2, order-1);
    }

}

/**
 * Draws a recursive tree fractal image of the specified size and order,
 * placing the bounding box's top-left corner at position (x,y).
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the recursive tree.
 * @param x - The x coordinate of the top-left corner of the bounding box.
 * @param y - The y coordinate of the top-left corner of the bounding box.
 * @param size - The length of one side of the bounding box.
 * @param order - The order of the fractal.
 */

void drawTreeHelper(GWindow& gw, double x1, double y1, double size, int order, double angle) {
    if (order == 0) return;
    int x2 = x1 + (int) (sin(angle*M_PI/180) * size);
    int y2 = y1 - (int) (cos(angle*M_PI/180) * size);
    if (order == 1) gw.setColor("#2e8b57");
    else gw.setColor("#8b7765");
    gw.drawLine(x1, y1, x2, y2);
    drawTreeHelper(gw, x2, y2, size/2, order - 1, angle -45);
    drawTreeHelper(gw, x2, y2, size/2, order - 1, angle -30);
    drawTreeHelper(gw, x2, y2, size/2, order - 1, angle -15);
    drawTreeHelper(gw, x2, y2, size/2, order - 1, angle - 0);
    drawTreeHelper(gw, x2, y2, size/2, order - 1, angle +15);
    drawTreeHelper(gw, x2, y2, size/2, order - 1, angle +30);
    drawTreeHelper(gw, x2, y2, size/2, order - 1, angle +45);
}

void drawTree(GWindow& gw, double x, double y, double size, int order) {
    if (order == 0) return;
    drawTreeHelper(gw, x+size/2, y+size, size/2, order, 0);
}

/**
 * Draws a Mandelbrot Set in the graphical window give, with maxIterations
 * (size in GUI) and in a given color (zero for palette)
 *
 * This will be called by fractalgui.cpp.
 *
 * @param gw - The window in which to draw the Mandelbrot set.
 * @param minX - left-most column of grid
 * @param incX - increment value of columns of grid
 * @param minY - top-most row of grid
 * @param incY - increment value of rows of grid
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @param color - The color of the fractal; zero if palette is to be used
 */
void mandelbrotSet(GWindow& gw, double minX, double incX,
                   double minY, double incY, int maxIterations, int color) {

    // Creates palette of colors
    // To use palette:
    // pixels[r][c] = palette[numIterations % palette.size()];
    Vector<int> palette = setPalette();

    int width = gw.getCanvasWidth();
    int height = gw.getCanvasHeight();
    GBufferedImage image(width, height, 0xffffff);
    gw.add(&image);
    Grid<int> pixels = image.toGrid(); // Convert image to grid

    // Iterates over cols and rows of the grid to check if particular pixel
    // is inside of the Madelbrot set.
    for(int i=0; i< width; i++) {

        for(int j=0; j<height; j++) {


            // Calls the mandelbortSetInterations to get the number of 
            // iterations necessary to determine if c diverges.
            Complex c(minX + i*incX, minY + j*incY);
            int iter = mandelbrotSetIterations(c, maxIterations);

            // Defines colors of each pixel within the grid.
            if (iter == 0) {
                if (color==0) {
                    pixels[j][i] = palette[iter % palette.size()];
                }
                
                else {
                    pixels[j][i] = color;
                }
            }

            else if (color == 0) {
                pixels[j][i] = palette[iter % palette.size()];
            }
        }
    }

    image.fromGrid(pixels); // Converts and puts the grid back into the image
}

/**
 * Runs the Mandelbrot Set recursive formula on complex number c a maximum
 * of maxIterations times.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param c - Complex number to use for recursive formula.
 * @param maxIterations - The maximum number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */

// Initiates z=0 and calls the recurseve helper function passing z, c and the max
// number of iterations.
int mandelbrotSetIterations(Complex c, int maxIterations) {
    Complex z(0,0);
    return mandelbrotSetIterations(z, c, maxIterations);
}
/**
 * An iteration of the Mandelbrot Set recursive formula with given values z and c, to
 * run for a maximum of maxIterations.
 *
 * This will be called by you. Think about how this fits with the other two functions.
 *
 * @param z - Complex number for a given number of iterations
 * @param c - Complex number to use for recursive formula.
 * @param remainingIterations - The remaining number of iterations to run recursive step
 * @return number of iterations needed to determine if c is unbounded
 */

// Recursion function to find if the paricular value of c diverges.
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {

    // base assumption.
    int iter = remainingIterations;

    if (z.abs() < 4 && remainingIterations > 0) {
        z = z*z + c;
        iter = mandelbrotSetIterations(z, c, remainingIterations - 1);
    }

    return iter;
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;
    string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
