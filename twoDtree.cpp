
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"
#include "iostream"
#include "string"

// using std::////cout;
// using std::endl;

/* given */
twoDtree::Node::Node(pair<int, int> ul, pair<int, int> lr, RGBAPixel a)
	: upLeft(ul), lowRight(lr), avg(a), left(NULL), right(NULL)
{
}

/* given */
twoDtree::~twoDtree()
{
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree &other)
{
	copy(other);
}

/* given */
twoDtree &twoDtree::operator=(const twoDtree &rhs)
{
	if (this != &rhs)
	{
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG &imIn)
{
	/* your code here */
	stats imIn_stats(imIn);

	height = imIn.height();
	width = imIn.width();

	pair<int, int> imIn_upperLeft(0, 0);
	pair<int, int> imIn_lowerRight(width - 1, height - 1);

	root = buildTree(imIn_stats, imIn_upperLeft, imIn_lowerRight);
}

twoDtree::Node *twoDtree::buildTree(stats &s, pair<int, int> ul, pair<int, int> lr)
{
	/* your code here */
	////cout << "a"<< s.rectArea(ul,lr) << endl;
	Node *node = new Node(ul, lr, s.getAvg(ul, lr));

	node->left = NULL;
	node->right = NULL;

	if (ul.first == lr.first && ul.second == lr.second)
	{
		return node;
	}

	// Declare two new points: Lower Right of R1, and Upper Left of R2
	pair<int, int> newLowerRight_R1;
	pair<int, int> newUpperLeft_R2;

	// Split
	splitTree(s, ul, lr, newLowerRight_R1, newUpperLeft_R2);
	//cout << newLowerRight_R1.first << ":" << newLowerRight_R1.second << endl;
	// // Recursively build tree
	node->left = buildTree(s, ul, newLowerRight_R1);
	node->right = buildTree(s, newUpperLeft_R2, lr);

	return node;
}

// count - total
pair<int, int> twoDtree::shouldBePruned(Node *baseNode, Node *node, int tol)
{
	if (node->left == NULL && node->right == NULL)
	{
		int r = (node->avg.r - baseNode->avg.r) * (node->avg.r - baseNode->avg.r);
		int g = (node->avg.g - baseNode->avg.g) * (node->avg.g - baseNode->avg.g);
		int b = (node->avg.b - baseNode->avg.b) * (node->avg.b - baseNode->avg.b);
		if (r + g + b <= tol)
		{
			return pair<int, int>(1, 1);
		}
		else
		{
			return pair<int, int>(0, 1);
		}
	}

	pair<int, int> leftPruneStat = shouldBePruned(baseNode, node->left, tol);
	pair<int, int> rightPruneStat = shouldBePruned(baseNode, node->right, tol);
	int totalBelowTolerance = leftPruneStat.first + rightPruneStat.first;
	int totalChildNode = leftPruneStat.second + rightPruneStat.second;
	return pair<int, int>(totalBelowTolerance, totalChildNode);
}

// count - total
void twoDtree::pruneANode(Node *node, double pct, int tol)
{
	if (node->left == NULL && node->right == NULL)
	{
		return;
	}

	pair<int, int> whyShouldBePruned = shouldBePruned(node, node, tol);
	int totalBelowTolerance = whyShouldBePruned.first;
	int totalChildNode = whyShouldBePruned.second;
	if (totalBelowTolerance * 100 / totalChildNode >= pct * 100)
	{
		node->left = NULL;
		node->right = NULL;
	}
	else
	{
		pruneANode(node->left, pct, tol);
		pruneANode(node->right, pct, tol);
	}
}

void twoDtree::prune(double pct, int tol)
{
	pruneANode(root, pct, tol);
}

void twoDtree::clear()
{
	/* your code here */
	clear(root);
	root = NULL;
	height = 0;
	width = 0;
}

void twoDtree::copy(const twoDtree &other)
{
	/* your code here */
	root = copy(other.root);
	height = other.height;
	width = other.width;
}

/**
 ******************* Helpers ****************/

/**
 * Helper for clear() */
void twoDtree::clear(Node *node)
{
	if (node == NULL)
		return;
	else
	{
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

/**
 * Helper for copy(const twoDtree & other) */
twoDtree::Node *twoDtree::copy(const Node *otherNode)
{
	if (otherNode == NULL)
		return NULL;
	else
	{
		Node *newNode = new Node(otherNode->upLeft, otherNode->lowRight, otherNode->avg);
		newNode->left = copy(otherNode->left);
		newNode->right = copy(otherNode->right);

		return newNode;
	}
}

/**
 * Row-wise split R into R1 and R2  */
pair<int, long> twoDtree::rowWiseSplit(stats &s, pair<int, int> ul, pair<int, int> lr)
{

	if (ul.second == lr.second) return make_pair(0, 0);
	int minRowWise = ul.second;
	int minRowWiseScore = s.getScore(ul, make_pair(lr.first, ul.second)) 
							+ s.getScore(make_pair(ul.first, ul.second + 1), lr);
	for (int i = ul.second; i < lr.second; i++)
	{
		pair<int, int> newLowerRight_R1(lr.first, i);	//R1: same ul, new lower right
		pair<int, int> newUpperLeft_R2(ul.first, i + 1); //R2: new upper left, same lr
		long score_i = s.getScore(ul, newLowerRight_R1) + s.getScore(newUpperLeft_R2, lr);

		if (score_i < minRowWiseScore)
		{
			minRowWise = i;
			minRowWiseScore = score_i;
		}
	}

	//cout << "a" << minRowWiseScore << endl;
	return make_pair(minRowWise, minRowWiseScore);
}

/**
 * Col-wise split R into R1 and R2 */
pair<int, long> twoDtree::colWiseSplit(stats &s, pair<int, int> ul, pair<int, int> lr)
{

	if (ul.first == lr.first) return make_pair(0, 0);

	int minColWise = ul.first;
	int minColWiseScore = s.getScore(ul, pair<int, int>(ul.first, lr.second)) 
							+ s.getScore(pair<int, int>(ul.first + 1, ul.second), lr);
	for (int i = ul.first; i < lr.first; i++)
	{
		pair<int, int> newLowerRight_R1(i, lr.second);	//R1: same ul, new lower right
		pair<int, int> newUpperLeft_R2(i + 1, ul.second); //R2: new upper left, same lr
		long score_i = s.getScore(ul, newLowerRight_R1) + s.getScore(newUpperLeft_R2, lr);
		//cout << "i:" << i << ";score_i:" << score_i << ";min:" << minColWiseScore << endl;
		if (score_i < minColWiseScore)
		{
			//cout << "here" << endl;
			minColWise = i;
			minColWiseScore = score_i;
		}
	}

	//cout << "k:" << minColWise << endl;

	return make_pair(minColWise, minColWiseScore);
}

/**
 * split and update the 2 new points: Upper Left of R2, Lower Right of R1 */
void twoDtree::splitTree(stats &s, pair<int, int> ul, pair<int, int> lr,
						 pair<int, int> &newLowerRight_R1, pair<int, int> &newUpperLeft_R2)
{

	// Row-wise split
	pair<int, long> returnFromRowWiseSplit;
	returnFromRowWiseSplit = rowWiseSplit(s, ul, lr);
	int newLowerRight_R1_y = returnFromRowWiseSplit.first;
	long scoreRowWise = returnFromRowWiseSplit.second;

	pair<int, int> newLowerRight_RowWise_R1(lr.first, newLowerRight_R1_y);
	pair<int, int> newUpperLeft_RowWise_R2(ul.first, newLowerRight_R1_y + 1);

	// Col-wise split
	pair<int, long> returnFromColWiseSplit;
	// Compare row-wise and col-wise splits
	returnFromColWiseSplit = colWiseSplit(s, ul, lr);
	int newLowerRight_R1_x = returnFromColWiseSplit.first;
	long scoreColWise = returnFromColWiseSplit.second;

	pair<int, int> newLowerRight_ColWise_R1(newLowerRight_R1_x, lr.second);
	pair<int, int> newUpperLeft_ColWise_R2(newLowerRight_R1_x + 1, ul.second);

	// Compare row-wise and col-wise splits
	if (ul.first == lr.first)
	{
		//cout << "a" << endl;
		newLowerRight_R1 = newLowerRight_RowWise_R1;
		newUpperLeft_R2 = newUpperLeft_RowWise_R2;
	}
	else if (ul.second == lr.second)
	{
		//cout << "b" << endl;
		newLowerRight_R1 = newLowerRight_ColWise_R1;
		newUpperLeft_R2 = newUpperLeft_ColWise_R2;
	}
	else if (scoreRowWise < scoreColWise)
	{
		//cout << "c" << endl;
		newLowerRight_R1 = newLowerRight_RowWise_R1;
		newUpperLeft_R2 = newUpperLeft_RowWise_R2;
	}
	else
	{
		//cout << "d" << endl;
		newLowerRight_R1 = newLowerRight_ColWise_R1;
		newUpperLeft_R2 = newUpperLeft_ColWise_R2;
	}
}

/** 
 * helper for render() */
void twoDtree::render(PNG &png, Node *node)
{
	if (node->left == NULL && node->right == NULL)
	{
		for (int i = node->upLeft.second; i <= node->lowRight.second; i++)
		{
			for (int j = node->upLeft.first; j <= node->lowRight.first; j++)
			{
				//cout << "i:" << i << ";j:" << j << ";avg:" << (int)node->avg.r << endl;
				png.getPixel(j, i)->r = node->avg.r;
				png.getPixel(j, i)->g = node->avg.g;
				png.getPixel(j, i)->b = node->avg.b;
			}
		}
		return;
	}

	render(png, node->left);
	render(png, node->right);
}

PNG twoDtree::render()
{
	/* your code here */
	PNG img(width, height);
	render(img, root);
	return img;
}