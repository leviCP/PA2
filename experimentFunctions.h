
#include <algorithm>
#include <vector>
#include <iostream>
#include <stdlib.h>
#include <random>
#include <cstdlib> // For rand() and srand()
#include <ctime>   // For time()
#include <chrono>
#include <cmath>

// util function for log2
double log2(double d);

// test functions for experiment 1
bool testBST(AVLTree<int>* avl);
bool testBalanced(AVLTree<int>* avl);
bool testContains(AVLTree<int>*avl, int numData);

// test functions for experiment 2
bool testSize(AVLTree<int> * avl, int targetNumIntegers);
bool testHeight(AVLTree<int>* avl);

// random insert/remove for experiment 2
void insertRandomIntegers(AVLTree<int>* avl, int numIntegers);
void deleteRandomIntegers(AVLTree<int>* avl, int numDelete);

// directly perform experiments
void experiment1(int numIntegers);
void stage1(AVLTree<int>* avl, int numIntegers);
void stage2(AVLTree<int>* avl, int numRandomInsertRemove);
