//Created by Zeynep Turkmen on 12.04.2022.
#ifndef BST_h
#define BST_h

#include <iostream>
#include <string>
#include <vector>

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
        vector<string> info; //to store the description of an item
        BinaryNode(const Comparable & theElement,  BinaryNode *lt = NULL, BinaryNode *rt = NULL): element(theElement), left(lt), right(rt)
        {}
    friend class BinarySearchTree<Comparable>;
};

template <class Comparable>
class BinarySearchTree
{
    public:
        explicit BinarySearchTree(const Comparable & notFound ); //constructor
        BinarySearchTree(const BinarySearchTree & rhs); //copy constructor
        ~BinarySearchTree( ); //destructor
    vector<string> & AllInfo(Comparable & y);
        const Comparable & findMin() const;  //finds the min val ans returns a reference to it
        const Comparable & findMax() const;  //finds the max val ans returns a reference to it
        const Comparable & find(const Comparable & x ) const; //finds the inputted obj and returns a reference to it
        bool isEmpty( ) const; //checks if its empty
    const Comparable & infoAt(BinaryNode<Comparable> * t) const;
    const Comparable & findInfo(const Comparable & x) const;
        void printTree( ) const; //prints the binary tree
        void makeEmpty(); //clears the tree
        void insert( const Comparable & x ); //adds a new element
        void remove(const Comparable & x); //removes the inputted element
    const BinarySearchTree<Comparable> & operator=(const BinarySearchTree<Comparable> & rhs);
    void changeDescription(Comparable & x, string & d, const bool & a);
    
    private:
       //private functions help to make recursions for the public fuctions
        const Comparable ITEM_NOT_FOUND;
        BinaryNode<Comparable> *root;
        void displaySections(BinaryNode<Comparable> * t) const;
        const Comparable & elementAt(BinaryNode<Comparable> *t) const;
        void insert( const Comparable & x, BinaryNode<Comparable> * & t ) const;
        void remove( const Comparable & x, BinaryNode<Comparable> * & t ) const;
        BinaryNode<Comparable> * findMin( BinaryNode<Comparable> *t ) const;
        BinaryNode<Comparable> * findMax( BinaryNode<Comparable> *t ) const;
        
        BinaryNode<Comparable> * find( const Comparable & x, BinaryNode<Comparable> *t ) const;
        void makeEmpty(BinaryNode<Comparable> * & t) const;
        void printTree(BinaryNode<Comparable> *t) const;
        BinaryNode<Comparable> * clone( BinaryNode<Comparable> *t ) const;
};

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
           string a = "{";
           for (int i = 0; i < t->info.size(); i++)
           {
               a = a + " " + t->info[i];
           }
           a = a + "}";
           cout << a << endl;
           printTree(t->right);
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
        printTree(root);
    }
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
    {
        BinaryNode<Comparable> temp = new BinaryNode<Comparable> (t->element);
        for(int i=0; i < t->info.size(); i++) //copies the information vector into the new thing
        {
            temp->info.push_back(t->info[i]);
        }
        temp->left = clone(t->left);
        temp->right = clone(t->right);
        return temp;
    }
}

template <class Comparable>
BinarySearchTree<Comparable>::BinarySearchTree(const BinarySearchTree<Comparable> & rhs): root( NULL ), ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND) //copy constructor
{
    *this = rhs;
}

template <class Comparable>
void BinarySearchTree<Comparable>::changeDescription(Comparable & word, string & newInfo, const bool & flag)
{   //finds item x and changes its information accordingly to the input
    //flags helps check if the item is to be modified or to be added
    BinaryNode<Comparable> * location = find(word, root);
    if (location != NULL ) //if its found
    {
        unsigned long int idx = location->info.size() - 1;
        if (!flag)
            location->info[idx] = newInfo;
        else
            location->info.push_back(newInfo);
    }
}

template <class Comparable>
vector<string> & BinarySearchTree<Comparable>::AllInfo(Comparable & y)
{   //returns a reference to the information vector of the word "y"
    BinaryNode<Comparable> * c = find(y,root);
    return c->info;
}

#endif
