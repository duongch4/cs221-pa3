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
    if (channel == 'r') return getSum(sumRed, ul, lr);
    else if (channel == 'g') return getSum(sumGreen, ul, lr);
    else if (channel == 'b') return getSum(sumBlue, ul, lr);
}

/* returns the sums of squares of all pixel values across a given color channel.
* useful in computing the score of a rectangle
* PA3 function
* @param channel is one of r, g, or b
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getSumSq(char channel, pair<int,int> ul, pair<int,int> lr) {
    if (channel == 'r') return getSum(sumsqRed, ul, lr);
    else if (channel == 'g') return getSum(sumsqGreen, ul, lr);
    else if (channel == 'b') return getSum(sumsqBlue, ul, lr);
}

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

    // cout << "\n";
    // cout << "first 5x5 sumRed:" << endl;
    // printStats(sumRed);

    // cout << "\n";
    // cout << "first 5x5 sumsqRed:" << endl;
    // printStats(sumsqRed);
    
    cout << "\n";
    pair<int,int> ul (0,0);
    pair<int,int> lr (0,0);
    //cout << "getSum on Red Channel: " << getSumSq('r', ul, lr) << endl;

    // cout << "\n";
    // ul.first = 0;
    // ul.second = 2;
    // cout << "getSum on Red Channel: " << getSumSq('r', ul, lr) << endl;
    
    // cout << "\n";
    // ul.first = 3;
    // ul.second = 0;
    // cout << "getSum on Red Channel: " << getSumSq('r', ul, lr) << endl;
    
    // cout << "\n";
    // ul.first = 1;
    // ul.second = 3;
    // cout << "getSum on Red Channel: " << getSumSq('r', ul, lr) << endl;

    cout << "\n";
    cout << getSumSq('r', ul, lr) << ":" << getSumSq('g', ul, lr) << ":" << getSumSq('b', ul, lr) << endl;
    cout << getSum('r', ul, lr) << ":" << getSum('g', ul, lr) << ":" << getSum('b', ul, lr) << endl;
    cout << rectArea(ul, lr) << endl;
    cout << getScore('r', ul, lr) << ":" << getScore('g', ul, lr) << ":" << getScore('b', ul, lr) << endl;
    cout << getScore(ul, lr) << endl;

    // cout << "\n";
    // RGBAPixel avgPixel = getAvg(ul, lr);
    // cout << (int)avgPixel.r << ":" << (int)avgPixel.g << ":" << (int)avgPixel.b << endl;
}

// given a rectangle, compute its sum of squared deviations from 
// mean, over all color channels. Will be used to make split when
// building tree.
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::getScore(pair<int,int> ul, pair<int,int> lr) {
    return ( getScore('r', ul, lr) + getScore('g', ul, lr) + getScore('b', ul, lr) );
}

// given a rectangle, return the average color value over the 
// rectangle as a pixel.
/* Each color component of the pixel is the average value of that 
* component over the rectangle.
* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
RGBAPixel stats::getAvg(pair<int,int> ul, pair<int,int> lr) {
    long avgRed = getSum('r', ul, lr) / rectArea(ul, lr);
    long avgGreen = getSum('g', ul, lr) / rectArea(ul, lr);
    long avgBlue = getSum('b', ul, lr) / rectArea(ul, lr);
    return RGBAPixel(avgRed, avgGreen, avgBlue);
}

// given a rectangle, return the number of pixels in the rectangle
/* @param ul is (x,y) of the upper left corner of the rectangle 
* @param lr is (x,y) of the lower right corner of the rectangle */
long stats::rectArea(pair<int,int> ul, pair<int,int> lr) {
    long width = lr.first - ul.first + 1;
    long height = lr.second - ul.second + 1;
    if (width == 0 && height == 0) return 1;
    else if (width == 0) return height; 
    else if (height == 0) return width;
    else return width * height;
}



/**
 * Private Helpers*/

/**
 * Print any 2D-vectors */
template <class T>
void stats::printStats(vector< vector<T> >& vec) {
    for (int i = 0; i < 250; i++) {
        for (int j = 0; j < 462; j++) {
            cout << vec[i][j] << " ";
        }
        cout << endl;
    }
}

/**
 * Resize vectors since we know their sizes beforehand
 * Used in stats(PNG& im) */
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

/**
 * Update and reset accumulators methods
 * Used in stats(PNG& im) */
void stats::updateAcc(RGBAPixel*& pixel, long& accR, long& accG, long& accB, long& accSqR, long& accSqG, long& accSqB) {
    accR += pixel->r;
    accG += pixel->g;
    accB += pixel->b; 
    accSqR += (pixel->r) * (pixel->r);
    accSqG += (pixel->g) * (pixel->g);
    accSqB += (pixel->b) * (pixel->b);
}

void stats::resetAcc(long& accR, long& accG, long& accB, long& accSqR, long& accSqG, long& accSqB) {
    accR = 0;
    accG = 0;
    accB = 0;
    accSqR = 0;
    accSqG = 0;
    accSqB = 0;
}


/**
 * Update base case for the private vectors
 * Used in stats(PNG& im) */
void stats::updateSixVectorsBaseCase(unsigned i, unsigned j, long& accR, long& accG, long& accB, long& accSqR, long& accSqG, long& accSqB) {
    sumRed[i][j] = accR;
    sumGreen[i][j] = accG;
    sumBlue[i][j] = accB;
    sumsqRed[i][j] = accSqR;
    sumsqGreen[i][j] = accSqG;
    sumsqBlue[i][j] = accSqB;
}

/**
 * Update main case for the private vectors (which are Summed-Area-Table or SAT)
 * Used in stats(PNG& im) 
 * SAT(i,j) = Original(i,j) + SAT(i-1,j) + SAT(i,j-1) - SAT(i-1,j-1)*/
void stats::updateSixVectorsMainCase(unsigned i, unsigned j, RGBAPixel*& pixel) {
    sumRed[i][j] = pixel->r + sumRed[i-1][j] + sumRed[i][j-1] - sumRed[i-1][j-1];
    sumGreen[i][j] = pixel->g + sumGreen[i-1][j] + sumGreen[i][j-1] - sumGreen[i-1][j-1];
    sumBlue[i][j] = pixel->b + sumBlue[i-1][j] + sumBlue[i][j-1] - sumBlue[i-1][j-1];
    sumsqRed[i][j] = (pixel->r)*(pixel->r) + sumsqRed[i-1][j] + sumsqRed[i][j-1] - sumsqRed[i-1][j-1];
    sumsqGreen[i][j] = (pixel->g)*(pixel->g) + sumsqGreen[i-1][j] + sumsqGreen[i][j-1] - sumsqGreen[i-1][j-1];
    sumsqBlue[i][j] = (pixel->b)*(pixel->b) + sumsqBlue[i-1][j] + sumsqBlue[i][j-1] - sumsqBlue[i-1][j-1];
}


/**
 * Get the upperLeft, lowerLeft, and upperRight components of a SAT
 * Used in getSum and getSumSq */
long stats::upperLeftComponent(vector< vector<long> >& vec, pair<int,int> ul) {
    return vec[ ul.second - 1 ][ ul.first - 1 ];
}

long stats::lowerLeftComponent(vector< vector<long> >& vec, pair<int,int> ul, pair<int,int> lr) {
    return vec[ lr.second ][ ul.first - 1 ];
}

long stats::upperRightComponent(vector< vector<long> >& vec, pair<int,int> ul, pair<int,int> lr) {
    return vec[ ul.second - 1 ][ lr.first ];
}

/**
 * getSum and getSumSq helper */ 
long stats::getSum(vector< vector<long> >& vec, pair<int,int> ul, pair<int,int> lr) {
    long lowerRightComponent = vec[ lr.second ][ lr.first ];
    if (ul.first == 0 && ul.second == 0) return lowerRightComponent;
    else if (ul.first == 0 && ul.second != 0) return lowerRightComponent - upperRightComponent(vec, ul, lr);
    else if (ul.first != 0 && ul.second == 0) return lowerRightComponent - lowerLeftComponent(vec, ul, lr);
    else return lowerRightComponent + upperLeftComponent(vec, ul) - lowerLeftComponent(vec, ul, lr) - upperRightComponent(vec, ul, lr);
}

/**
 * getScore for individual channel */
long stats::getScore(char channel, pair<int,int> ul, pair<int,int> lr) {
    long sumSq = getSumSq(channel, ul, lr);
    long sum = getSum(channel, ul, lr);
    long area = rectArea(ul, lr);
    return ( sumSq - (sum*sum / area) );
}