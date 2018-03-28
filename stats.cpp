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
    else return getSum(sumBlue, ul, lr);
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
    else return getSum(sumsqBlue, ul, lr);
}

// initialize the private vectors so that, for each color,  entry 
// (x,y) is the cumulative sum of the the color values from (0,0)
// to (x,y). Similarly, the sumSq vectors are the cumulative
// sum of squares from (0,0) to (x,y).
stats::stats(PNG & im) {

    initChannel(im, 'r', sumRed, sumsqRed);
    initChannel(im, 'g', sumGreen, sumsqGreen);
    initChannel(im, 'b', sumBlue, sumsqBlue);

}

void stats::initChannel(PNG & im, char channel, vector< vector<long> >& vec, vector< vector<long> >& vecSq) {
    long acc = 0;
    long accSq = 0;

    vec.resize(im.height());
    vecSq.resize(im.height());
    
    for ( unsigned i = 0; i < im.height(); i++ ) {
        vec[i].resize(im.width());
        vecSq[i].resize(im.width());
        RGBAPixel* pixel = im.getPixel(0, i);
        acc += chooseChannel(channel, pixel);
        accSq += chooseChannel(channel, pixel) * chooseChannel(channel, pixel);
        vec[i][0] = acc;
        vecSq[i][0] = accSq;
    }

    acc = 0;
    accSq = 0;

    for ( unsigned j = 0; j < im.width(); j++ ) {
        RGBAPixel* pixel = im.getPixel(j, 0);
        acc += chooseChannel(channel, pixel);
        accSq += chooseChannel(channel, pixel) * chooseChannel(channel, pixel);
        vec[0][j] = acc;
        vecSq[0][j] = accSq;
    }

    for ( unsigned i = 1; i < im.height(); i++ ) {
        for ( unsigned j = 1; j < im.width(); j++ ) {
            RGBAPixel* pixel = im.getPixel(j, i);
            vec[i][j] = chooseChannel(channel, pixel) + vec[i-1][j] + vec[i][j-1] - vec[i-1][j-1];
            vecSq[i][j] = chooseChannel(channel, pixel) * chooseChannel(channel, pixel) 
                            + vecSq[i-1][j] + vecSq[i][j-1] - vecSq[i-1][j-1];
        }
    }
}

long stats::chooseChannel(char channel, RGBAPixel*& pixel) {
    if (channel == 'r') {
        return pixel->r;
    }
    else if (channel == 'g') {
        return pixel->g;
    }
    else {
        return pixel->b;
    }
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

    return width * height;
}


/**
 * Private Helpers*/

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