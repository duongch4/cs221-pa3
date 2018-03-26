
#ifndef _STATS_H
#define _STATS_H

#include "cs221util/PNG.h"
#include "cs221util/RGBAPixel.h"
#include <utility>
#include <vector>
using namespace std;
using namespace cs221util;

class stats
{

  private:
	vector<vector<long>> sumRed;
	vector<vector<long>> sumGreen;
	vector<vector<long>> sumBlue;
	vector<vector<long>> sumsqRed;
	vector<vector<long>> sumsqGreen;
	vector<vector<long>> sumsqBlue;

	/* returns the sums of all pixel values across all color channels.
	* useful in computing the score of a rectangle
	* PA3 function
	* @param channel is one of r, g, or b
	* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
	long getSum(char channel, pair<int, int> ul, pair<int, int> lr);

	/* returns the sums of squares of all pixel values across all color channels.
	* useful in computing the score of a rectangle
	* PA3 function
	* @param channel is one of r, g, or b
	* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
	long getSumSq(char channel, pair<int, int> ul, pair<int, int> lr);

	/**
	 * Private Helpers*/

	/**
	 * Print any 2D-vectors */
	template <class T>
	void printStats(vector<vector<T>> &vec);

	/**
	 * Resize vectors since we know their sizes beforehand
	 * Used in stats(PNG& im) */
	void resizeSixVectors_NumberOfRowsToImgHeight(PNG &im);
	void resizeSixVectors_NumberOfColsToImgWidth(PNG &im, int row);

	/**
	 * Update and reset accumulators methods
	 * Used in stats(PNG& im) */
	void updateAcc(RGBAPixel *&pixel, long &accR, long &accG, long &accB, long &accSqR, long &accSqG, long &accSqB);
	void resetAcc(long &accR, long &accG, long &accB, long &accSqR, long &accSqG, long &accSqB);

	/**
	 * Update base case for the private vectors
	 * Used in stats(PNG& im) */
	void updateSixVectorsBaseCase(unsigned row, unsigned col, long &accR, long &accG, long &accB, long &accSqR, long &accSqG, long &accSqB);

	/**
	 * Update main case for the private vectors (which are Summed-Area-Table or SAT)
	 * Used in stats(PNG& im) 
	 * SAT(i,j) = Original(i,j) + SAT(i-1,j) + SAT(i,j-1) - SAT(i-1,j-1)*/
	void updateSixVectorsMainCase(unsigned i, unsigned j, RGBAPixel *&pixel);

	/**
	 * Get the upperLeft, lowerLeft, and upperRight components of a SAT
	 * Used in getSum and getSumSq */
	long upperLeftComponent(vector<vector<long>> &vec, pair<int, int> ul);
	long lowerLeftComponent(vector<vector<long>> &vec, pair<int, int> ul, pair<int, int> lr);
	long upperRightComponent(vector<vector<long>> &vec, pair<int, int> ul, pair<int, int> lr);

	/**
	 * getSum helper */
	long getSum(vector<vector<long>> &vec, pair<int, int> ul, pair<int, int> lr);

	/**
	 * getScore helper: score for individual channel */
	long getScore(char channel, pair<int, int> ul, pair<int, int> lr);

	void initChannel(
		char channel,
		vector<vector<long>> &sumChannel,
		vector<vector<long>> &sumSqChannel,
		PNG &im);

	void initChannel(char channel, PNG &im);

	char getChannelValue(char channel, RGBAPixel *pixel);

  public:
	// initialize the private vectors so that, for each color,  entry
	// (x,y) is the cumulative sum of the the color values from (0,0)
	// to (x,y). Similarly, the sumSq vectors are the cumulative
	// sum of squares from (0,0) to (x,y).
	stats(PNG &im);

	// given a rectangle, compute its sum of squared deviations from
	// mean, over all color channels. Will be used to make split when
	// building tree.
	/* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
	long getScore(pair<int, int> ul, pair<int, int> lr);

	// given a rectangle, return the average color value over the
	// rectangle as a pixel.
	/* Each color component of the pixel is the average value of that 
	* component over the rectangle.
	* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
	RGBAPixel getAvg(pair<int, int> ul, pair<int, int> lr);

	// given a rectangle, return the number of pixels in the rectangle
	/* @param ul is (x,y) of the upper left corner of the rectangle 
	* @param lr is (x,y) of the lower right corner of the rectangle */
	long rectArea(pair<int, int> ul, pair<int, int> lr);
};

#endif
