
//  Created by Zeynep Turkmen on 13.04.2022.

#include "AVL.h"

template <class Comparable>
AvlTree<Comparable>::AvlTree(const Comparable & notFound, const string & t): //constructs an empty tree with title t
ITEM_NOT_FOUND(notFound), root(NULL), title(t){}

template <class Comparable>
AvlTree<Comparable>::AvlTree(const AvlTree<Comparable> & rhs): root(NULL), ITEM_NOT_FOUND(rhs.ITEM_NOT_FOUND), title(rhs.title)
{   //copy constructor
    *this = rhs;
}

template <class Comparable>
AvlTree<Comparable>::~AvlTree( )
{   //destructor
    makeEmpty(root);
}


template <class Comparable>
bool AvlTree<Comparable>:: operator < (AvlTree<Comparable> & rhs)
{   //helps comparisons for nested trees
    return (root->element < rhs.root->element);
}

template <class Comparable>
const bool AvlTree<Comparable>::isEmpty() const
{   //checks if the tree is empty
    if (root == NULL)
        return true;
    return false;
}

template <class Comparable>
const int AvlTree<Comparable>::balanceCheck(AvlNode<Comparable> *N) //for debugging
{
  if (N == NULL)
    return 0;
  return height(N->left) - height(N->right);
}

template <class Comparable>
const string & AvlTree<Comparable>::getTitle() const //returns the title
{
    return title;
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::elementAt(AvlNode<Comparable> * t) const
{
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
int AvlTree<Comparable>::height(AvlNode<Comparable> *t)  const //returns the height of t
{
    if (t == NULL)
        return -1;
    return t->height;
}

template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::find(const Comparable & x, AvlNode<Comparable> *t) const
{   //finds x recursively and returns a reference to it
    if ( t == NULL )
        return NULL;
    else if(x < t->element)
        return find(x, t->left); //finds from left subtree
    else if(t->element < x)
        return find(x, t->right); //finds from right subtree
    else
        return t;
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::find(const Comparable & x) const
{   //public face of find
    return elementAt(find(x, root));
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::infoAt(AvlNode<Comparable> * t) const
{
    return t == NULL ? ITEM_NOT_FOUND : t->info;
}
template <class Comparable>
const Comparable & AvlTree<Comparable>::findInfo(const Comparable & x) const
{
   return infoAt(find(x, root)); //same as find but returns the information instead
}

template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::findSubSection(const Comparable  & x, AvlNode<Comparable> *t) const
{   //same as find but comparisons are made accordingly to the title
    if ( t == NULL )
        return NULL;
    else if(x.getTitle() < t->element.getTitle())
        return findSubSection(x, t->left);
    else if(t->element.getTitle() < x.getTitle())
        return findSubSection(x, t->right);
    else
        return t;
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::findSubSection(const Comparable &  x) const
{   //public face of findSubSection
    return elementAt(findSubSection(x, root));
}

template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::findMin(AvlNode<Comparable> *t) const
{   //finds the minimun element and returns a reference to it
    if( t == NULL )
        return NULL;
    if( t->left == NULL )
        return t;
    return findMin(t->left);
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::findMin() const
{   //public face of find
    return elementAt(findMin(root));
}

template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::findMax(AvlNode<Comparable> *t)const
{   //finds the maximum element and returns a reference to it
    if( t != NULL )
        while( t->right != NULL )
            t = t->right;
    return t;
}

template <class Comparable>
const Comparable & AvlTree<Comparable>::findMax() const
{   //public face of findMax
    return elementAt(findMax(root));
}

template <class Comparable>
const AvlTree<Comparable> & AvlTree<Comparable>::operator =(const AvlTree<Comparable> & rhs)
{   //makes lhs ewual to rhs with deep copy
    if (this != &rhs)
    {
        makeEmpty(); //clear old one
        root = rhs.clone(rhs.root);
    }
    return *this;
}

template <class Comparable>
void AvlTree<Comparable>::changeDescription(Comparable & x, const string & d)
{   //finds x and changes its description to d
    find(x, root)->info = d;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithLeftChild(AvlNode<Comparable> * & k2) const
{   //rotate the node with its left child
    AvlNode<Comparable> *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    //update height
    k2->height = max( height( k2->left ), height( k2->right ) ) + 1;
    k1->height = max( height( k1->left ), k2->height ) + 1;
    k2 = k1;
}

template <class Comparable>
void AvlTree<Comparable>::rotateWithRightChild(AvlNode<Comparable> * & k1) const
{   //rotate the node with its right child
    AvlNode<Comparable> *k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    //update height
    k1->height = max(height(k1->left), height(k1->right)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2;
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithLeftChild(AvlNode<Comparable> * & k3) const
{   //rotate the node with its left child and right child and then the k3 with the new left
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}

template <class Comparable>
void AvlTree<Comparable>::doubleWithRightChild(AvlNode<Comparable> * & k1) const
{   //rotate the node with its right child and left child and then the k1 with the new right
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1);
}

template <class Comparable>
AvlNode<Comparable> * AvlTree<Comparable>::clone(AvlNode<Comparable> * t) const
{   //clones the inputted node recursively
    if ( t == NULL )
        return NULL;
    else
        return new AvlNode<Comparable>(t->element, t->info, clone(t->left), clone(t->right));
}

template <class Comparable>
void AvlTree<Comparable>::makeEmpty(AvlNode<Comparable> * & t) const
{   //clears the tree
    if( t != NULL )
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

template <class Comparable>
void AvlTree<Comparable>::makeEmpty( )
{   //public face of makeEmpty()
    makeEmpty(root);
}

 template <class Comparable>
 void AvlTree<Comparable>::printTree(AvlNode<Comparable> * t)
 {      //prints the tree recursvely
       if ( t != NULL )
       {
             printTree (t->left);
             cout << t->element << endl;
             printTree( t->right );
       }
 }

template <class Comparable>
void AvlTree<Comparable>::printTree()
{   //public face of printTree
    printTree(root);
}

template <class Comparable>
void AvlTree<Comparable>::displaySections(AvlNode<Comparable> * t) const
{   //same with print but only prints the titles
    if (t != NULL)
    {
          Comparable a = t->element;
          displaySections(t->left);
          cout << a.getTitle() << endl;
          displaySections(t->right);
    }
}

template <class Comparable>
void AvlTree<Comparable>::displaySections() const
{   //public face of displaySections()
    displaySections(root);
}

template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable & x, AvlNode<Comparable> * & t) const
{
    if(t == NULL)
        return;
    else if (x < t->element)
    {
        remove( x, t->left); //remove from the left subtree
        if (height(t->right) - height(t->left) == 2 ) //left subtree decreased in height so same idea with right subtree growing, we check the rotations for the right subtreee
        {
            if ( t->right->element < x )
                 doubleWithRightChild( t );
            else
                rotateWithRightChild( t );
        }
    }
    else if(t->element < x)
    {
        remove(x, t->right); //remove from the right subtree
        if ( height(t->left) - height(t->right) == 2) //right subtree decreased in height so same idea with left subtree growing, we check the rotations for the left subtreee
        {
            if (x < t->left->element)
                doubleWithLeftChild(t);
            else
                rotateWithLeftChild(t);
        }
       
    }
    else if(t->left != NULL && t->right != NULL)
    {
        t->element = findMin(t->right)->element;
        remove(t->element, t->right);
    }
    else
    {
        AvlNode<Comparable> *oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }
    if (t != NULL)
                t->height = max(height( t->left ), height( t->right ) ) + 1;
}

template <class Comparable>
void AvlTree<Comparable>::removeSubTree(const Comparable & x, AvlNode<Comparable> * & t) const
{
    if(t == NULL)
        return;  //not found
    else if (x.getTitle() < t->element.getTitle())
    {
        removeSubTree(x, t->left);
        if (height(t->right) - height( t->left ) == 2 ) //left subtree decreased in height so same idea with right subtree growing, we check the rotations for the right subtreee
        {
            if ( t->right->element.getTitle() < x.getTitle())
                 doubleWithRightChild( t );
            else
                rotateWithRightChild( t );
        }
    }
    else if(t->element.getTitle() < x.getTitle())
    {
        removeSubTree(x, t->right);
        if ( height( t->left ) - height( t->right ) == 2 ) //right subtree decreased in height so same idea with left subtree growing, we check the rotations for the left subtreee
        {    if ( x.getTitle() < t->left->element.getTitle() )
                doubleWithLeftChild( t );
            else
                rotateWithLeftChild( t );
        }
    }
    else if(t->left != NULL && t->right != NULL) // Two children
    {
        t->element = findMin( t->right )->element;
        removeSubTree( t->element, t->right );
    }
    else
    {
        AvlNode<Comparable> *oldNode = t;
        t = ( t->left != NULL ) ? t->left : t->right;
        oldNode->element.makeEmpty();
        delete oldNode;
    }
    if (t != NULL)
                t->height = max(height( t->left ), height( t->right ) ) + 1;
}

template <class Comparable>
void AvlTree<Comparable>::removeSubTree(const Comparable & x)
{   //public face for removeSubtree
    removeSubTree(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::insert(const Comparable & x, AvlNode<Comparable> * & t ) const
{   //comparisons are made accordingly to the title of AVL trees
    if ( t == NULL )
        t = new AvlNode<Comparable>(x);
    
    else if ( x.getTitle() < t->element.getTitle() )
    {
        insert( x, t->left );
        if ( height( t->left ) - height( t->right ) == 2 ) //left subtree height increased
        {    if ( x.getTitle() < t->left->element.getTitle() ) // It was added to left-left
                rotateWithLeftChild( t );
            else //It was added to left right
                doubleWithLeftChild( t );
        }
    }
    else if( t->element.getTitle() < x.getTitle() )
    {
        insert( x, t->right );
        if ( height( t->right ) - height( t->left ) == 2 ) //right subtree height increased
        {
            if ( t->right->element.getTitle() < x.getTitle() ) //It was added to right right
                rotateWithRightChild( t );
            else // It was added to right left
                doubleWithRightChild( t );
        }
    }
    else;
    t->height = max( height( t->left ), height( t->right ) ) + 1;
}

template <class Comparable>
void AvlTree<Comparable>::insert( const Comparable & x )
{   //public face of insert
    insert(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::insertToSubTree(const Comparable & x, const string & y,  AvlNode<Comparable> * & t)
{   //same insert but comparisons are made accordingly to the elements and y becomes the description for the nodes
    if ( t == NULL )
        t = new AvlNode<Comparable>(x, y);
    
    else if (x < t->element)
    {
        insertToSubTree(x, y, t->left);
        if ( height( t->left ) - height( t->right ) == 2 )
        {    if (x < t->left->element)
                rotateWithLeftChild( t );
            else
                doubleWithLeftChild( t );
        }
    }
    else if( t->element < x )
    {
        insertToSubTree(x,y, t->right );
        if ( height( t->right ) - height( t->left ) == 2 )
        {
            if ( t->right->element < x  )
                rotateWithRightChild( t );
            else
                doubleWithRightChild( t );
        }
    }
    else;
    t->height = max( height( t->left ), height( t->right ) ) + 1;
}


template <class Comparable>
void AvlTree<Comparable>::insertToSubTree(const Comparable & x, const string & y)
{   //public face of inserttosubtree
    insertToSubTree(x, y, root);
}

template <class Comparable>
void AvlTree<Comparable>::remove(const Comparable & x)
{   //public face of remove
    remove(x, root);
}

template <class Comparable>
void AvlTree<Comparable>::PrintTrees(AvlNode<Comparable> * t)
{  //prints the big tree and its subtrees (helps debugging)
    if (t != NULL)
    {
          Comparable a = t->element; //a is now a AVL tree
          PrintTrees(t->left);
          cout << a.getTitle() << endl;
          a.printTree();
          PrintTrees(t->right);
    }
}

template <class Comparable>
void AvlTree<Comparable>::PrintTrees()
{   //public face of PrintTrees
    PrintTrees(root);
}

template <class Comparable>
void AvlTree<Comparable>::changeRoot(const AvlTree<Comparable> & t)
{   //clears the tree and makes its node point to t's root
    makeEmpty();
    root = t.root;
}
