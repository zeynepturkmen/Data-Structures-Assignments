
//Created by Zeynep Turkmen on 12.04.2022.
#ifndef BST_h
#define BST_h

#include <iostream>
#include <string>
using namespace std;

template <class Comparable>
class BinarySearchTree;

template <class Comparable>
class BinaryNode
{
public:
    Comparable element;
    BinaryNode  *left;
    BinaryNode  *right;
    string info; //to store the description of an item
    BinaryNode(const Comparable & theElement, const string & theinfo = "", BinaryNode *lt = NULL, BinaryNode *rt = NULL): element(theElement), left(lt), right(rt), info(theinfo)
    {}
    friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree
{
    public:
        explicit BinarySearchTree(const Comparable & notFound ); //constructor
        BinarySearchTree(const BinarySearchTree & rhs ); //copy constructor
        ~BinarySearchTree( ); //destructor
        void changeRoot(const BinarySearchTree<Comparable> & t); //changes the root to the root of the inputted one (kinda like shallow copy)
        const Comparable & findMin() const;  //finds the min val ans returns a reference to it
        const Comparable & findMax() const;  //finds the max val ans returns a reference to it
        const Comparable & find(const Comparable & x ) const; //finds the inputted obj and returns a reference to it
        void insertToSubTree(const Comparable & x, const string & y); //inserts to subtrees
        bool isEmpty( ) const; //checks if its empty
        void printTree2() const; //same as printTree but doesnt print the root
        void printTree( ) const; //prints the binary tree
        void makeEmpty(); //clears the tree
        void insert( const Comparable & x ); //adds a new element
        friend bool operator < (const Comparable  & lhs, const Comparable  & rhs); //to make comparasions easy with nested trees
        void insertTree(BinarySearchTree<Comparable> &); //inserts a subtree in a big tree
        void remove(const Comparable & x); //removes the inputted element
        const BinarySearchTree & operator = (const BinarySearchTree & rhs); //helps deep copy
        void PrintTrees() const; //prints the big tree and the small ones within it (helps debugging)
        void displaySections() const; //only prints the section names aka roots of each subtree
        const Comparable & findInfo(const Comparable & x) const; //same as find but returns the info instead of the element
        void changeDescription(Comparable & x, string & d); //changes the info value to the given one
        BinaryNode<Comparable> *root;
    private:
       //private functions help to make recursions for the public fuctions
        const Comparable ITEM_NOT_FOUND;
        void displaySections(BinaryNode<Comparable> * t) const;
        void PrintTrees(BinaryNode<Comparable> * t) const;
        void insertToSubTree(const Comparable & x, const string & y, BinaryNode<Comparable> * & t ) const;
        const Comparable & elementAt(BinaryNode<Comparable> *t) const;
        const Comparable & infoAt(BinaryNode<Comparable> * t) const;
        void insert( const Comparable & x, BinaryNode<Comparable> * & t ) const;
        void insertTree(BinaryNode<Comparable> * & m , BinaryNode<Comparable> * & t); //adds a new element
        void remove( const Comparable & x, BinaryNode<Comparable> * & t ) const;
        void printTree2(BinaryNode<Comparable> * t) const;
        BinaryNode<Comparable> * findMin( BinaryNode<Comparable> *t ) const;
        BinaryNode<Comparable> * findMax( BinaryNode<Comparable> *t ) const;
        BinaryNode<Comparable> * find( const Comparable & x, BinaryNode<Comparable> *t ) const;
        void makeEmpty(BinaryNode<Comparable> * & t) const;
        void printTree(BinaryNode<Comparable> *t) const;
        BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const;
};

#include "BST.cpp"
#endif
