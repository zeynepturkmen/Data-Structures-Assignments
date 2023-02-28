
//Created by Zeynep Turkmen on 12.04.2022.
#include "BST.h"

template <class Comparable>
bool operator < (const Comparable & lhs, const Comparable & rhs) //helps comparisons for nested trees
{
return (lhs.root->element < rhs.root->element);
}

template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const Comparable & notFound ) : ITEM_NOT_FOUND(notFound), root(NULL) //constructor for the tree
{
}


template <class Comparable>
bool BinarySearchTree<Comparable>::isEmpty() const //checks if the tree is empty
{
    if (root == NULL)
        return true;
    return false;
}

template <class Comparable>
const BinarySearchTree<Comparable> & BinarySearchTree<Comparable>::operator=(const BinarySearchTree<Comparable> & rhs) //makes a deep copy by assigning it to the right input
{
    if (this != &rhs)
    {
        makeEmpty();
        root = rhs.clone(rhs.root);
    }
    return *this;
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::elementAt(BinaryNode<Comparable> * t) const
{   //returns the element in node t (or not found if failed)
    return t == NULL ? ITEM_NOT_FOUND : t->element;
}

template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::find(const Comparable & x, BinaryNode<Comparable> * t) const
{   //finds x from t recursively
    if ( t == NULL )
        return NULL;
    else if(x < t->element)
        return find(x, t->left);
    else if(t->element < x)
        return find(x, t->right);
    else
        return t; //FOUND!
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::find(const Comparable & x) const
{   //public face of find
    return elementAt(find( x, root));
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::infoAt(BinaryNode<Comparable> * t) const
{   //returns the information in node t (or not found if failed)
    return t == NULL ? ITEM_NOT_FOUND : t->info;
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findInfo(const Comparable & x) const
{   //same as find but it returns the information
    return infoAt(find(x, root));
}

template <class Comparable>
void BinarySearchTree<Comparable>::changeDescription(Comparable & x, string & d)
{   //finds item x and changes its information accordingly to the input
    find(x, root)->info = d;
}

template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::findMin( BinaryNode<Comparable> *t ) const
{   //recursively finds and returns a reference to the smallest item
    if( t == NULL )
        return NULL;
    if( t->left == NULL )
        return t;
    return findMin( t->left );
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMin()const
{   //public face of findmin
    return elementAt(findMin(root));
}

template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>::findMax(BinaryNode<Comparable> *t)const
{   //recursively finds and returns a reference to the biggest item
    if( t != NULL )
        while( t->right != NULL )
            t = t->right;
    return t;
}

template <class Comparable>
const Comparable & BinarySearchTree<Comparable>::findMax() const
{   //public face of findMax
    return elementAt(findMax(root));
}

template <class Comparable>
void BinarySearchTree<Comparable>::insert(const Comparable & x, BinaryNode<Comparable> * & t ) const
{   //traveles arounf the binary tree nodes (t) and inserts x in the right place
    if( t == NULL ) //create a new node
        t = new BinaryNode<Comparable>(x);
    else if(x < t->element)
        insert( x, t->left); //insert to left subtree
    else if(t->element < x)
        insert(x, t->right); //insert to right subtree
    else
        ;
}

template <class Comparable>
void BinarySearchTree<Comparable>::insert( const Comparable & x )
{   //public face of insert
    insert(x, root);
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertToSubTree(const Comparable & x, const string & y, BinaryNode<Comparable> * & t ) const
{   //same as insert but helps insert into subtrees with their information and everything
    if( t == NULL )
    {
        t = new BinaryNode<Comparable>(x,y);
    }
    else if(x < t->element)
        insertToSubTree(x, y, t->left); //insert to left subtree
    else if(t->element < x)
        insertToSubTree(x, y, t->right); //insert to right subtree
    else
        ;
}

template <class Comparable>
void BinarySearchTree<Comparable>::insertToSubTree( const Comparable & x, const string & y )
{
    //public face of insertToSubTree
    insertToSubTree(x,y, root);
}

template <class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x, BinaryNode<Comparable> * & t) const
{
    if(t == NULL)
        return;  //item wasn't found
    if (x < t->element)
        remove(x, t->left); //remove from left subtree
    else if(t->element < x)
        remove(x, t->right); //remove from right subtree
    else if(t->left != NULL && t->right != NULL) //Two child node
    {
        t->element = findMin( t->right )->element;
        remove( t->element, t->right );
    }
    else //one or no children
    {
        BinaryNode<Comparable> *oldNode = t;
        t = (t->left != NULL) ? t->left : t->right;
        delete oldNode;
    }
}

template <class Comparable>
void BinarySearchTree<Comparable>::remove(const Comparable & x)
{   //public face of remove
    remove(x, root);
}

template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( BinaryNode<Comparable> * & t ) const
{   //clears the allocated memory recursively
    if( t != NULL )
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = NULL;
}

template <class Comparable>
void BinarySearchTree<Comparable>::makeEmpty( )
{   //public face of makeEmpty()
    makeEmpty(root);
}

 template <class Comparable>
 void BinarySearchTree<Comparable>::printTree(BinaryNode<Comparable> * t) const
 {     //prints the tree recursively
       if ( t != NULL )
       {
             printTree (t->left);
             cout << t->element << endl;
             printTree( t->right );
       }
 }

template <class Comparable>
void BinarySearchTree<Comparable>::printTree() const
{
    //public face of printTree
    if(isEmpty())
        cout << "Empty tree" << endl;
    else
    {
        cout << root->element << endl;
        printTree(root->left);
        printTree(root->right);
    }
}

template <class Comparable>
void BinarySearchTree<Comparable>::printTree2() const
{   //same as printTree but doesnt print the root (due to reasons explained above the main file)
    if(isEmpty())
        cout << "Empty tree" << endl;
    else
    {
        printTree(root->left);
        printTree(root->right);
    }
}


template <class Comparable>
void BinarySearchTree<Comparable>::PrintTrees(BinaryNode<Comparable> * t) const
{   //helpes debug the code
    if (t != NULL)
    {
          Comparable a = t->element; //a is now a binary tree itself
          PrintTrees(t->left);
          a.printTree(); //print the binary tree (regular stuff)
          PrintTrees(t->right);
    }
}

template <class Comparable>
void BinarySearchTree<Comparable>::PrintTrees() const
{   //public face of PrintTrees
    if(isEmpty())
        cout << "Empty tree" << endl;
    else
        PrintTrees(root);
}

template <class Comparable>
void BinarySearchTree<Comparable>::displaySections(BinaryNode<Comparable> * t) const
{   //only prints the root nodes of subtrees
    if (t != NULL)
    {
          Comparable a = t->element; //a is now a binary tree
          displaySections(t->left);
          cout << a.root->element << endl;
          displaySections(t->right);
    }
}

template <class Comparable>
void BinarySearchTree<Comparable>::displaySections() const
{   //public face of display sections
    displaySections(root);
}

template <class Comparable>
BinarySearchTree<Comparable>::~BinarySearchTree() //destructor
{
    makeEmpty( );
}

template <class Comparable>
BinaryNode<Comparable> * BinarySearchTree<Comparable>:: clone( BinaryNode<Comparable> * t ) const
{   //clones the tree recursively
    if (t == NULL )
        return NULL;
    else
        return new BinaryNode<Comparable>
        ( t->element, t->info,
         clone(t->left),
         clone(t->right));
}

template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const BinarySearchTree<Comparable> & rhs): root( NULL ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND) //copy constructor
{
    *this = rhs;
}

template <class Comparable>
void BinarySearchTree<Comparable>::changeRoot(const BinarySearchTree<Comparable> & t)
{   //clears the allocated memory and changes the root to the root of t
    makeEmpty();
    root = t.root;
}
