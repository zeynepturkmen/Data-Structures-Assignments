
//  Created by Zeynep Turkmen on 13.04.2022.
#ifndef AVL_h
#define AVL_h

#include <string>
#include <iostream>
using namespace std;

template <class Comparable>
class AvlTree;
template <class Comparable>
class AvlNode
{
    Comparable element;
    string info; //description of the element
    AvlNode *left;
    AvlNode *right;
    int height;
    AvlNode(const Comparable & theElement, const string & i = "",  AvlNode *lt = NULL, AvlNode *rt = NULL, int h = 0)
    : element(theElement), info(i), left(lt), right(rt), height(h){ }
    friend class AvlTree<Comparable>;
};

template <class Comparable>
class AvlTree
{
    public:
        explicit AvlTree(const Comparable & notFound, const string & t = ""); //constructor
        AvlTree(const AvlTree & rhs); //copy constructor
        ~AvlTree( ); //destructor
        const bool isEmpty() const; //checks if the tree is empty
        const int balanceCheck(AvlNode<Comparable> *N); //checks the balance (for debugging)
        const string & getTitle() const; //returns the title of the AVL tree
        void changeRoot(const AvlTree<Comparable> & t); //changes the info value to the given one
        const Comparable & find (const Comparable & x) const; //finds the inputted obj and returns a reference to it
        const Comparable & findInfo(const Comparable & x) const; //same as find but returns the info instead of the element
        const Comparable & findSubSection( const Comparable & x) const; //looks for subsections in nested trees
        const Comparable & findMin() const; //finds the min obj and returns a reference to it
        const Comparable & findMax() const; //finds the max obj and returns a reference to it
        void PrintTrees(); //prints everything big trees and subtrees (helps debugging)
        void printTree(); //prints the tree
        void displaySections() const; //prints the titles of subtrees
        void makeEmpty(); //clears the treee
        void changeDescription(Comparable & x, const string & d); //changes the info to the given one
        void insert(const Comparable & x); //inserts a new element (big tree)
        void insertToSubTree(const Comparable &, const string &); //inserts new element with its info and everything to the subtrees
        void remove(const Comparable & x); //removes the inputted element
        void removeSubTree(const Comparable & x); //removes from the big tree
        bool operator < (AvlTree<Comparable> & rhs); //helps comparisons
        const AvlTree & operator = (const AvlTree & rhs); //makes lhs and rhs equal with deep copy
        friend bool operator < (const Comparable  & lhs, const Comparable  & rhs)
        {   //helps comparisons
            return (lhs.root->element < rhs.root->element);
        }
       
    private:
        //private functions help to make recursions for the public fuctions
        string title;
        AvlNode<Comparable> *root;
        const Comparable ITEM_NOT_FOUND;
        void removeSubTree( const Comparable & x, AvlNode<Comparable> * & t) const;
        void insertToSubTree(const Comparable & x, const string & y,  AvlNode<Comparable> * & t);
        const Comparable & elementAt( AvlNode<Comparable> *t ) const;
        const Comparable & infoAt(AvlNode<Comparable> * t) const;
        void displaySections(AvlNode<Comparable> * t) const;
        void PrintTrees(AvlNode<Comparable> * t);
        void insert( const Comparable & x, AvlNode<Comparable> * & t ) const;
        void remove(const Comparable & x, AvlNode<Comparable> * & t ) const;
        AvlNode<Comparable> * findMin( AvlNode<Comparable> *t ) const;
        AvlNode<Comparable> * findMax( AvlNode<Comparable> *t ) const;
        AvlNode<Comparable> * findSubSection( const Comparable &  x, AvlNode<Comparable> *t ) const;
        AvlNode<Comparable> * find( const Comparable & x, AvlNode<Comparable> *t ) const;
        void makeEmpty( AvlNode<Comparable> * & t ) const;
        void printTree( AvlNode<Comparable> *t ) ;
        AvlNode<Comparable> * clone(AvlNode<Comparable> *t) const;
        int height( AvlNode<Comparable> *t ) const;
        void rotateWithLeftChild( AvlNode<Comparable> * & k2 ) const;
        void rotateWithRightChild( AvlNode<Comparable> * & k1 ) const;
        void doubleWithLeftChild( AvlNode<Comparable> * & k3 ) const;
        void doubleWithRightChild( AvlNode<Comparable> * & k1 ) const;
};

#include "AVL.cpp"
#endif 
