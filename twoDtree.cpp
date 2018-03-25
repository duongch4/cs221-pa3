
/**
 *
 * twoDtree (pa3)
 * slight modification of a Kd tree of dimension 2.
 * twoDtree.cpp
 * This file will be used for grading.
 *
 */

#include "twoDtree.h"

/* given */
twoDtree::Node::Node(pair<int,int> ul, pair<int,int> lr, RGBAPixel a)
	:upLeft(ul),lowRight(lr),avg(a),left(NULL),right(NULL)
	{}

/* given */
twoDtree::~twoDtree(){
	clear();
}

/* given */
twoDtree::twoDtree(const twoDtree & other) {
	copy(other);
}

/* given */
twoDtree & twoDtree::operator=(const twoDtree & rhs){
	if (this != &rhs) {
		clear();
		copy(rhs);
	}
	return *this;
}

twoDtree::twoDtree(PNG & imIn){ /* your code here */ }

twoDtree::Node * twoDtree::buildTree(stats & s, pair<int,int> ul, pair<int,int> lr) {
	/* your code here */
}

PNG twoDtree::render(){
	/* your code here */
}

void twoDtree::prune(double pct, int tol){
	/* your code here */
}

void twoDtree::clear() {
	/* your code here */
	clear(root);
	root = NULL;
	height = 0;
	width = 0;
}


void twoDtree::copy(const twoDtree & other){
	/* your code here */
	root = copy(other.root);
	height = other.height;
	width = other.width;
}



/**
 ******************* Helpers ****************/

/**
 * Helper for clear() */
void twoDtree::clear(Node* node) {
	if (node == NULL) return;
	else {
		clear(node->left);
		clear(node->right);
		delete node;
	}
}

/**
 * Helper for copy(const twoDtree & other) */
twoDtree::Node* twoDtree::copy(const Node* otherNode) {
	if (otherNode == NULL) return NULL;
	else {
		Node* newNode = new Node(otherNode->upLeft, otherNode->lowRight, otherNode->avg);
		newNode->left = copy(otherNode->left);
		newNode->right = copy(otherNode->right);
		
		return newNode;
	}
}