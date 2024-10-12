

#include "AVLTree.h"
#include "experimentFunctions.h"

using namespace std;



int main()
{
    // experiment 1: test BST order and AVL height condition
    cout << "================================ Experiment 1 ================================" << endl;
    int numIntegers = 20;
    experiment1(numIntegers);
    cout << endl;

    // experiment 2: test whether the average depth of AVL trees is invariant to random insert/remove pair operations
    cout << "============================ Experiment 2, Stage 1 ============================" << endl;

    // Stage 1: insert random integers into AVL BST, as Figure 4.29 of textbook
    numIntegers = 20;
    AVLTree<int>*avl = new AVLTree<int>();
    stage1(avl, numIntegers);
    cout << endl;

    // Stage 2: 500^2 times of random insert/delete pairs for this AVL tree, as per textbook, Figure 4.30
    int numRandomInsertRemove = 250000;
    cout << "========== Experiment 2, Stage 2 (after " << numRandomInsertRemove << " random insert/delete) ==========" << endl;
    stage2(avl, numRandomInsertRemove);

    // delete this avl
    delete avl;

    return 0;

}