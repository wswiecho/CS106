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

    if (order > 0) {

    //compute triangle points
    double x0 = x;
    double y0 = y;
    double x1 = x0 + size;
    double y1 = y0;
    double x2 = x0 + size / 2;
    double y2 = y0 + sqrt(3)* size / 2;

    // draw the triangle
    gw.drawLine(x0, y0, x1, y1);
    gw.drawLine(x0, y0, x2, y2);
    gw.drawLine(x1, y1, x2, y2);


    //recursive calls
    drawSierpinskiTriangle(gw, x0, y0, size / 2, order-1);
    drawSierpinskiTriangle(gw, (x0 + x1) / 2, (y0 + y1) / 2, size / 2, order-1);
    drawSierpinskiTriangle(gw, (x0 + x2) / 2, (y0 + y2) / 2, size / 2, order-1);
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

    // TODO: Put your Mandelbrot Set code here
    // iterating over the cols
    for(int i=0; i< width; i++) {
        // Iterating over the rows
        for(int j=0; j<height; j++) {


            // Calls the mandelbortSetInterations to get the iter
            Complex c(minX + i*incX, minY + j*incY);
            //cout << "real part of c" << c.realPart() << endl;
            //cout << "imaginary part of c" <<c.imagPart() << endl;
            int iter = mandelbrotSetIterations(c, maxIterations);
            if (iter == 0) {
            pixels[j][i] = palette[iter % palette.size()];
            }
            else {
                pixels[j][i] =iter;
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
int mandelbrotSetIterations(Complex c, int maxIterations) {
    // Initiates z0=0;
    Complex z(0,0);
    //cout << abs(z) << endl;

    int iter = mandelbrotSetIterations(z, c, maxIterations);

    return abs(iter-maxIterations); // Only here to make this compile
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
int mandelbrotSetIterations(Complex z, Complex c, int remainingIterations) {
    // TODO: write this function
    int iter = remainingIterations;
    // Base case
    //double norm =
    //cout << cpx.realPart(z) << endl;

    if(200-iter == 200 && z.abs() <4) {
        iter =  200;
    }
    if (200-iter < 200 || z.abs() > 4) {
        iter =  200-iter;
    }
    if(200-iter < 200 && z.abs() < 4) {
        z = z*z + c;
        iter = mandelbrotSetIterations(z, c, remainingIterations -1);
   }
    return iter;
}

// Helper function to set the palette
Vector<int> setPalette() {
    Vector<int> colors;

    // Feel free to replace with any palette.
    // You can find palettes at:
    // http://www.colourlovers.com/palettes

    // Example palettes:
    // http://www.colourlovers.com/palette/4480793/in_the_middle
    //string colorSt = "#A0B965,#908F84,#BF3C43,#9D8E70,#C9BE91,#A0B965,#908F84,#BF3C43";

    // http://www.colourlovers.com/palette/4480786/Classy_Glass
    string colorSt = "#9AB0E9,#C47624,#25269A,#B72202,#00002E,#9AB0E9,#C47624,#25269A";

    // The following is the "Hope" palette:
    // http://www.colourlovers.com/palette/524048/Hope
    //string colorSt =  "#04182B,#5A8C8C,#F2D99D,#738585,#AB1111,#04182B,#5A8C8C,#F2D99D";
    Vector<string>colorsStrVec = stringSplit(colorSt,",");
    for (string color : colorsStrVec) {
        colors.add(convertColorToRGB(trim(color)));
    }
    return colors;
}
