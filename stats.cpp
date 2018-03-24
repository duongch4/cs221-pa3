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
    return 0.0; 
}

/* returns the sums of squares of all pixel values across a given color channel.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr) {
    return 0.0;
}

// initialize the private vectors so that, for each color,  entry 
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats::stats(PNG & im) {

    cout << "Original Red" << endl;
    for (unsigned i = 0; i < 5; i++) {
        for (unsigned j = 0; j < 5; j++) {
            //cout << "Element ( " << i << " , " << j << " ) = " << im.getPixel(i,j)->r;
            cout << (int) im.getPixel(j,i)->r << " ";
            //printf("%d", )
        }
        cout << endl;
    }

    long accR = 0;
    long accG = 0;
    long accB = 0;
    long accSqR = 0;
    long accSqG = 0;
    long accSqB = 0;

    resizeSixVectors_NumberOfRowsToImgHeight(im);
    
    for ( unsigned i = 0; i < im.height(); i++ ) {
        resizeSixVectors_NumberOfColsToImgWidth(im, i);
        RGBAPixel* pixel = im.getPixel(0, i);
        updateAcc(pixel, accR, accG, accB, accSqR, accSqG, accSqB);
        updateSixVectorsBaseCase(i, 0, accR, accG, accB, accSqR, accSqG, accSqB);
    }

    resetAcc(accR, accG, accB, accSqR, accSqG, accSqB);

    for ( unsigned j = 0; j < im.width(); j++ ) {
        RGBAPixel* pixel = im.getPixel(j, 0);
        updateAcc(pixel, accR, accG, accB, accSqR, accSqG, accSqB);
        updateSixVectorsBaseCase(0, j, accR, accG, accB, accSqR, accSqG, accSqB);
    }

    for ( unsigned i = 1; i < im.height(); i++ ) {
        for ( unsigned j = 1; j < im.width(); j++ ) {
            RGBAPixel* pixel = im.getPixel(j, i);
            updateSixVectorsMainCase(i, j, pixel);
        }
    }

    cout << "\n";
    cout << "first 5x5 sumRed:" << endl;
    printStats(sumRed);

    cout << "\n";
    cout << "first 5x5 sumsqRed:" << endl;
    printStats(sumsqRed);
    
}

// given a rectangle, compute its sum of squared deviations from 
// mean, over all color channels. Will be used to make split when
// building tree.
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getScore(pair<int,int> ul, pair<int,int> lr) {
    return 0.0;
}

// given a rectangle, return the average color value over the 
// rectangle as a pixel.
/* Each color component of the pixel is the average value of that 
* component over the rectangle.
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr) {
    return RGBAPixel(2,2,2);
}

// given a rectangle, return the number of pixels in the rectangle
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::rectArea(pair<int,int> ul, pair<int,int> lr) {
    return 0.0;
}


template <class T>
void stats::printStats(vector< vector<T> >& vec) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}

void stats::resizeSixVectors_NumberOfRowsToImgHeight(PNG& im) {
    sumRed.resize(im.height());
    sumGreen.resize(im.height());
    sumBlue.resize(im.height());
    sumsqRed.resize(im.height());
    sumsqGreen.resize(im.height());
    sumsqBlue.resize(im.height());
}

void stats::resizeSixVectors_NumberOfColsToImgWidth(PNG& im, int row) {
    sumRed[row].resize(im.width());
    sumGreen[row].resize(im.width());
    sumBlue[row].resize(im.width());
    sumsqRed[row].resize(im.width());
    sumsqGreen[row].resize(im.width());
    sumsqBlue[row].resize(im.width());
}

void stats::updateAcc(RGBAPixel*& pixel, long& accR, long& accG, long& accB, long& accSqR, long& accSqG, long& accSqB) {
    accR += pixel->r;
    accG += pixel->g;
    accB += pixel->b; 
    accSqR += (pixel->r) * (pixel->r);
    accSqG += (pixel->g) * (pixel->g);
    accSqB += (pixel->b) * (pixel->b);
}

void stats::updateSixVectorsBaseCase(unsigned i, unsigned j, long& accR, long& accG, long& accB, long& accSqR, long& accSqG, long& accSqB) {
    sumRed[i][j] = accR;
    sumGreen[i][j] = accG;
    sumBlue[i][j] = accB;
    sumsqRed[i][j] = accSqR;
    sumsqGreen[i][j] = accSqG;
    sumsqBlue[i][j] = accSqB;
}

void stats::resetAcc(long& accR, long& accG, long& accB, long& accSqR, long& accSqG, long& accSqB) {
    accR = 0;
    accG = 0;
    accB = 0;
    accSqR = 0;
    accSqG = 0;
    accSqB = 0;
}

void stats::updateSixVectorsMainCase(unsigned i, unsigned j, RGBAPixel*& pixel) {
    sumRed[i][j] = pixel->r + sumRed[i-1][j] + sumRed[i][j-1] - sumRed[i-1][j-1];
    sumGreen[i][j] = pixel->g + sumGreen[i-1][j] + sumGreen[i][j-1] - sumGreen[i-1][j-1];
    sumBlue[i][j] = pixel->b + sumBlue[i-1][j] + sumBlue[i][j-1] - sumBlue[i-1][j-1];
    sumsqRed[i][j] = (pixel->r)*(pixel->r) + sumsqRed[i-1][j] + sumsqRed[i][j-1] - sumsqRed[i-1][j-1];
    sumsqGreen[i][j] = (pixel->g)*(pixel->g) + sumsqGreen[i-1][j] + sumsqGreen[i][j-1] - sumsqGreen[i-1][j-1];
    sumsqBlue[i][j] = (pixel->b)*(pixel->b) + sumsqBlue[i-1][j] + sumsqBlue[i][j-1] - sumsqBlue[i-1][j-1];
}