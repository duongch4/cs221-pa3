#include "stats.h"

#include <iostream>
#include <string>

using std::cout;
using std::endl;

/* returns the sums of all pixel values across a given color channel.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSum(char channel, pair<int,int> ul, pair<int,int> lr) {

}

/* returns the sums of squares of all pixel values across a given color channel.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr);

// initialize the private vectors so that, for each color,  entry 
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats::stats(PNG & im) {
    long accR = 0;
    long accG = 0;
    long accB = 0;
    long accSqR = 0;
    long accSqG = 0;
    long accSqB = 0;

    sumRed.resize(im.width());
    sumGreen.resize(im.width());
    sumBlue.resize(im.width());
    sumsqRed.resize(im.width());
    sumsqGreen.resize(im.width());
    sumsqBlue.resize(im.width());
    
    for ( int col = 0; col < im.width(); col++ ) {
        sumRed[col].resize(im.height());
        sumGreen[col].resize(im.height());
        sumBlue[col].resize(im.height());
        sumsqRed[col].resize(im.height());
        sumsqGreen[col].resize(im.height());
        sumsqBlue[col].resize(im.height());

        RGBAPixel* pixel = im.getPixel(col, 0);
        
        accR += pixel->r;
        accG += pixel->g;
        accB += pixel->b; 
        accSqR += (pixel->r) * (pixel->r);
        accSqG += (pixel->g) * (pixel->g);
        accSqB += (pixel->b) * (pixel->b);

        sumRed[col][0] = accR;
        sumGreen[col][0] = accG;
        sumBlue[col][0] = accB;
        sumsqRed[col][0] = accSqR;
        sumsqGreen[col][0] = accSqG;
        sumsqBlue[col][0] = accSqB;
    }

    long accR = 0;
    long accG = 0;
    long accB = 0;
    long accSqR = 0;
    long accSqG = 0;
    long accSqB = 0;

    for ( int row = 0; row < im.height(); row++ ) {
        RGBAPixel* pixel = im.getPixel(0, row);

        accR += pixel->r;
        accG += pixel->g;
        accB += pixel->b; 
        accSqR += (pixel->r) * (pixel->r);
        accSqG += (pixel->g) * (pixel->g);
        accSqB += (pixel->b) * (pixel->b);

        sumRed[0][row] = accR;
        sumGreen[0][row] = accG;
        sumBlue[0][row] = accB;
        sumsqRed[0][row] = accSqR;
        sumsqGreen[0][row] = accSqG;
        sumsqBlue[0][row] = accSqB;
    }

    for ( int col = 1; col < im.width(); col++ ) {
        for ( int row = 1; row < im.height(); row++ ) {
            RGBAPixel* pixel = im.getPixel(col, row);

            sumRed[col][row] = pixel->r + sumRed[col-1][row] + sumRed[col][row-1] + sumRed[col-1][row-1];
            sumGreen[col][row] = pixel->g + sumGreen[col-1][row] + sumGreen[col][row-1] + sumGreen[col-1][row-1];
            sumBlue[col][row] = pixel->b + sumBlue[col-1][row] + sumBlue[col][row-1] + sumBlue[col-1][row-1];
            sumsqRed[col][row] = (pixel->r)*(pixel->r) + sumsqRed[col-1][row] + sumsqRed[col][row-1] + sumsqRed[col-1][row-1];
            sumsqGreen[col][row] = (pixel->g)*(pixel->g) + sumsqGreen[col-1][row] + sumsqGreen[col][row-1] + sumsqGreen[col-1][row-1];
            sumsqBlue[col][row] = (pixel->b)*(pixel->b) + sumsqBlue[col-1][row] + sumsqBlue[col][row-1] + sumsqBlue[col-1][row-1];
        }
    }

    cout << "Original Red" << endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cout << "Element ( " << i << " , " << j << " ) = " << im.getPixel(i,j)->r << endl;
        }
    }

    cout << "\n"
    cout << "first 5x5 sumRed:" << endl;
    printStats(sumRed);
    
}

// given a rectangle, compute its sum of squared deviations from 
// mean, over all color channels. Will be used to make split when
// building tree.
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getScore(pair<int,int> ul, pair<int,int> lr);

// given a rectangle, return the average color value over the 
// rectangle as a pixel.
/* Each color component of the pixel is the average value of that 
* component over the rectangle.
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr);

// given a rectangle, return the number of pixels in the rectangle
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::rectArea(pair<int,int> ul, pair<int,int> lr);


template <class T>
void stats::printStats(vector< vector<T> >& vec) {
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << "Element ( " << i << " , " << j << " ) = " << vec[i][j] << endl;
        }
    }
}