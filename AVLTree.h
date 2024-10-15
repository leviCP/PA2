

#include <iostream>
#include <limits>
#include <vector>
using namespace std;


template <typename Comparable>
class AVLTree
{
public:
    AVLTree();
    ~AVLTree();

    void makeEmpty();
    const Comparable & findMin() const;
    const Comparable & findMax() const;

    bool contains(const Comparable & x) const;
    void insert(const Comparable & x);
    void remove(const Comparable & x);
    int treeSize() const;
    int computeHeight() const;
    int readRootHeight() const;
    bool isBalanced() const;
    bool isBST() const;

    double averageDepth() const;
    void removeByRank(int rank);

    // the next line follows textbook Figure 4.42, Line 19
    static const int ALLOWED_IMBALANCE = 1;
private:
    struct AVLNode  // refer to textbook, Figure 4.40
    {
        Comparable element;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode( const Comparable & theElement, AVLNode *lt, AVLNode *rt ): element(theElement), left(lt), right(rt) {}
        AVLNode( Comparable && theElement, AVLNode *lt, AVLNode *rt ): element(move(theElement)), left(lt), right(rt) {}
    };

    AVLNode *root;

    AVLNode * findMin( AVLNode * t ) const;
    AVLNode * findMax( AVLNode * t ) const;
    void makeEmpty( AVLNode * & t );
    bool contains_h(const Comparable &x, AVLNode *rt);
    void insert_h(const Comparable & x, AVLNode *& rt, int ht_cnt);
    void remove_h(const Comparable & x, AVLNode *& rt);
    int treeSize_h( AVLNode *& rt);
    int findHeight(AVLNode* rt);

    void balance(AVLNode * & t);
    void rotateWithLeftChild( AVLNode * & t );
    void rotateWithRightChild( AVLNode * & t );
    void doubleWithLeftChild( AVLNode * & t);
    void doubleWithRightChild( AVLNode * & t);
    bool isBalanced_h(AVLNode * rt);

    int height( AVLNode *t );

    bool isBSTutil(AVLNode* rt, AVLNode* min_nod, AVLNode* max_nod);
};

// constructor
template <class Comparable>
AVLTree<Comparable>::AVLTree() : root(NULL) {}

// destructor
template <class Comparable>
AVLTree<Comparable>::~AVLTree()
{
    makeEmpty();
}

// public makeEmpty: follow the makeEmpty in BST, referring to textbook, Figure 4.27
template <typename Comparable>
void AVLTree<Comparable>::makeEmpty() {
    makeEmpty(root);
}

// private recursive makeEmpty: follow the makeEmpty in BST, referring to textbook, Figure 4.27
template <typename Comparable>
void AVLTree<Comparable>::makeEmpty(AVLNode * & t) {
    if ( t != NULL ) {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = NULL;
    }
}



// public findMin: follow the findMin in BST, referring to textbook, Figure 4.20
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMin() const {
    if (root == NULL) {
        throw underflow_error("Tree is empty");
    }
    return findMin(root)->element;
}

// private findMin: follow the findMin in BST, referring to textbook, Figure 4.20
template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMin(AVLNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->left == NULL) {
        return t;
    } else {
        return findMin(t->left);
    }
}

// public findMax: follow the findMax in BST, referring to textbook, Figure 4.21
template <typename Comparable>
const Comparable & AVLTree<Comparable>::findMax() const {
    if (root == NULL) {
        throw underflow_error("Tree is empty");
    }
    return findMax(root)->element;
}

// private findMax: follow the findMax in BST, referring to textbook, Figure 4.21
template <typename Comparable>
typename AVLTree<Comparable>::AVLNode* AVLTree<Comparable>::findMax(AVLNode * t) const {
    if ( t == NULL ) {
        return NULL;
    } else if (t->right == NULL) {
        return t;
    } else {
        return findMax(t->right);
    }
}

// start our implementation:
// public contains: follow the contains in BST, referring to textbook, Figure 4.17 and Figure 4.18
template<typename Comparable>
bool AVLTree<Comparable>::contains( const Comparable & x ) const {
    
    return contains_h(x, root);
}

//personal code
template <typename Comparable>
inline bool AVLTree<Comparable>::contains_h(const Comparable &x, AVLNode *rt)
{
    if(rt == nullptr) //base case false
        {return false;}
    else if( x < rt->element) //go left
    {
        return contains_h(x, rt->left);
    }
    else if( x > rt->element) //go right 
    {
        return contains_h(x, rt->right);
    }
    else
        return true; //match

}



// public insert: following BST, referring to textbook, Figure 4.17 and Figure 4.23
template<typename Comparable>
void AVLTree<Comparable>::insert(const Comparable & x) {

   insert_h(x, root , 0);
}

template <typename Comparable>
inline void AVLTree<Comparable>::insert_h(const Comparable &x, AVLNode *&rt, int ht_cnt)
{
    if(rt == nullptr){
        rt = new AVLNode(x, nullptr, nullptr, ht_cnt);  }
    else if(x < rt->element){   //go left
    insert_h(x, rt->left, ht_cnt++);    }
    else if(x > rt->element){   //go right
    insert_h(x, rt->right, ht_cnt++);    }
    else
    {//nothing
    }
}

// public remove: refer to textbook, Figure 4.17 and Figure 4.26
template<typename Comparable>
void AVLTree<Comparable>::remove( const Comparable & x ) {

    remove_h(x, root );
}

template <typename Comparable>
inline void AVLTree<Comparable>::remove_h(const Comparable &x, AVLNode *&rt)
{
    if(rt == nullptr){
       return; }

    if(x < rt->element){   //go left
        remove_h(x, rt->left);    }
    else if(rt->element < x){   //go right
        remove_h(x, rt->right);    }
    else if(rt->left != nullptr && rt->right != nullptr) // two childern
    {
        rt->element = findmin( rt->right )->element;
        remove( rt->element, rt->right);
    }
    else
    {
        AVLNode *del_node = rt;
        rt = (rt->left != nullptr) ? rt->left : rt->right;
        delete del_node;
    }

    balance(rt);
}

// private balance: refer to textbook, Figure 4.42, Line 21 - 40
// assume t is the node that violates the AVL condition, and we then identify which case to use (out of 4 cases)
template<typename Comparable>
void AVLTree<Comparable>::balance(AVLNode * & t) {
    if( t == nullptr )
        return;

    if( height( t->left ) - height( t->right ) > ALLOWED_IMBALANCE )
        if( height( t->left->left ) >= height( t->left->right ) )
            rotateWithLeftChild( t );
        else
            doubleWithLeftChild( t );
    else
    if( height( t->right ) - height( t->left ) > ALLOWED_IMBALANCE )
        if( height( t->right->right ) >= height( t->right->left ) )
            rotateWithRightChild( t );
        else
            doubleWithRightChild( t );

    t->height = max( height( t->left ), height( t->right ) ) + 1;
}

// private rotateWithLeftChild: for case 1, referring to textbook, Figure 4.44 (code) and Figure 4.43 (visualization)
template<typename Comparable>
void AVLTree<Comparable>::rotateWithLeftChild(AVLNode * & k2) {
    AVLNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}

// private rotateWithRightChild: for case 4 (the mirrored case of case 1)
template<typename Comparable>
void AVLTree<Comparable>::rotateWithRightChild(AVLNode * & k2) {
    AVLNode *k1 = k2->left;
    k2->right = k1->left;
    k1->left = k2;
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;

}

// private doubleWithLeftChild: for case 2, see textbook, Figure 4.46 (code) and Figure 4.45 (visualization)
template<typename Comparable>
void AVLTree<Comparable>::doubleWithLeftChild(AVLNode * & k3) {
    rotateWithRightChild( k3->left );
    rotateWithLeftChild( k3 );
}

// private doubleWithRightChild: for case 3 (the mirrored case of case 2)
template<typename Comparable>
void AVLTree<Comparable>::doubleWithRightChild(AVLNode * & k3) {
    rotateWithLeftChild( k3->right );
    rotateWithRightChild( k3 );
}

template <typename Comparable>
inline bool AVLTree<Comparable>::isBalanced_h(AVLNode *rt)
{
    if(rt == nullptr)
    {
        return true;
    }
    
    int left_height = findHeight(rt->left);
    int right_height = findHeight(rt->right);

    int bal_num = left_height + right_height;

    if(abs(bal_num) > 1){
        return false;
    }

    return isBalanced_h(rt->left) && isBalanced_h(rt->right);

}

// public isBalanced
template <class Comparable>
bool AVLTree<Comparable>::isBalanced() const {
    
    return false;
}

// public isBST
template <class Comparable>
bool AVLTree<Comparable>::isBST() const {
    isBSTutil(root,nullptr,nullptr);
}

template <typename Comparable>
inline bool AVLTree<Comparable>::isBSTutil(AVLNode *rt, AVLNode* min_nod, AVLNode* max_nod )
{
    if(rt == nullptr)
    {
        return true;
    }

    if((min_nod != nullptr && rt->element <= n=min_nod->element) ||
    (max_nod != nullptr && rt->element >= max_nod->element))
    {
        return false;
    }

    return isBSTutil(rt->left, min_nod, rt) && isBSTutil(rt->right,rt, max_nod);

}

// public treeSize
template <typename Comparable>
int AVLTree<Comparable>::treeSize() const {
    
    return 0;
}

template <typename Comparable>
inline int AVLTree<Comparable>::treeSize_h(AVLNode *& rt)
{
    if(!rt)
	{
		return 0;
	}
	else
	{
		return 1 + size_helper(rt->left) + size_helper(rt->right);
	}
}

template <typename Comparable>
inline int AVLTree<Comparable>::findHeight(AVLNode *rt)
{
    if (rt == nullptr)
    {
        return 0;
    }
    return rt->height;
}

// public computeHeight. See Figure 4.61 in Textbook
template <typename Comparable>
int AVLTree<Comparable>::computeHeight() const {
    height(root);
}

template <typename Comparable>
inline int AVLTree<Comparable>::height(AVLNode *t)
{
   if( t == nullptr )
        return -1;
    else
        return 1 + max( height( t->left ), height( t->right ) );
}



// public readRootHeight
template <typename Comparable>
int AVLTree<Comparable>::readRootHeight() const {
    cout << "TODO: readRootHeight function" << endl;
    return -1;
}

// public averageDepth
template <typename Comparable>
double AVLTree<Comparable>::averageDepth() const {
    cout << "TODO: averageDepth function" << endl;
    return 0.0;
}

// public removeByRank
template <typename Comparable>
void AVLTree<Comparable>::removeByRank(int rank) {
    cout << "TODO: removeByRank function" << endl;
}

