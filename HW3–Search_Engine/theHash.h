//  Created by Zeynep Turkmen on 05.05.2022.
#ifndef theHash_h
#define theHash_h

#include "List.h"
#include <vector>

using namespace std;

template <class HashedObj>
class HashTable
{
    public:
        HashTable( const HashedObj & notFound, //constructor
                  int size = 101, int count = 0); //count counts the unique words
        HashTable( const HashTable & rhs )
        : ITEM_NOT_FOUND( rhs.ITEM_NOT_FOUND ),
        theLists( rhs.theLists ), count(rhs.count) { } //copy constructor
        const HashedObj & find( const HashedObj & x ) const;
        void makeEmpty( ); //makes the table empty
        void insertList(List<HashedObj> & l); //inserts the values of a list into the table
        void insert( const HashedObj & x); //inserts x into the hashtable
        string findLatestInfo(const HashedObj & x); //finds the latest info on the information vector
        const vector<string> & findAllInfo(const HashedObj & x); //returns the information vector
        void remove( const HashedObj & x ); //removes an item
        void changeInfo(const HashedObj & x, const string & y, const bool &); //modifies the information vector that the list has
        void rehash(); //changes the size of the vector
        const HashTable & operator =( const HashTable & rhs); //helps copy
        void stats(); //prints the current load factor and the count of words
        void PrintItAll(); //prints everything (perfect debugging)
        ~HashTable(); //destructor
    
    private:
        vector<List<HashedObj>> theLists; // The vector of Lists
        const HashedObj ITEM_NOT_FOUND;
        int count;
};

bool isPrime( int n ) //checks if n is a prime number
{
    if ( n == 2 || n == 3 )
        return true;
    
    if ( n == 1 || n % 2 == 0 )
        return false;
    
    for ( int i = 3; i * i <= n; i += 2 )
        if ( n % i == 0 )
            return false;
    return true;
}
int nextPrime( int n ) //finds the next prime after n if n isnt a prime
{
    if ( n % 2 == 0 )
        n++;
    for ( ; ! isPrime( n ); n += 2 )
        ;
    
    return n;
}

long long int hashh(const string & S, int tableSize) //hashing function
{
    long long int L = 0;
    int d = 26;
    
    for (int i=0; i < S.length(); i++) //read char by char and multiply the ascii with a number
    {
        L = L + d * S[i];
        d = d * 43;
    }
    L = L % tableSize;
    if (L < 0) //if theres an overflow
        L = L + tableSize;
    return L;

}

template <class HashedObj>
HashTable<HashedObj>::HashTable(const HashedObj & notFound, int size, int count ) //constructor
: ITEM_NOT_FOUND( notFound ), theLists( nextPrime(size ) )
{
}

template <class HashedObj>
void HashTable<HashedObj>::insertList(List<HashedObj> & l)
{   //inserts the items of the list l into the hash table if its not empty
    if (l.isEmpty())
        return;
    ListItr<HashedObj> itr = l.first();
    while (!itr.isPastEnd())
    {
        string s = itr.retrieve();
        insert(s);
        for (int i=0; i< itr.infosize(); i++) //modifies the information vector within the item
        {
            changeInfo(s, itr.infAt(i), true);
        }
        itr.advance();
    }
}

template <class HashedObj>
void HashTable<HashedObj>::rehash()
{
    vector<List<HashedObj>> temp = theLists;
    theLists.clear();
    count = 0; //makes the count 0 again
    theLists.resize(nextPrime(temp.size()*2)); //doubles the size of the vector makes it a prime
    for (int i = 0; i < temp.size(); i ++)
    {
        insertList(temp[i]);
    }
    
}

template <class HashedObj>
void HashTable<HashedObj>::makeEmpty( )
{  //destroys the lists
    for( int i = 0; i < theLists.size( ); i++ )
        theLists[ i ].makeEmpty( );
}

template <class HashedObj>
void HashTable<HashedObj>::insert(const HashedObj & x)
{    //inserts x into the table
    // hash the given object and locate the list it should be on
    List<HashedObj> & whichList = theLists[hashh( x, theLists.size())];
    // locate the object in the list (using List’s find)
    ListItr<HashedObj> itr = whichList.find( x );
    // insert the new item at the head of the list if not found!
    if (itr.isPastEnd( ))
    {
        whichList.insert(x, whichList.zeroth( ));
        count++;
    }
    double load = double(count)/theLists.size();
    if (load > 0.9) //if the load factor exceeds 0.8 starts rehashing
    {
        int prev = theLists.size();
        rehash();
        load = double(count)/theLists.size();
        cout << "rehashed..." << endl;
        cout << "previous table size: " << prev << ", new table size: " << theLists.size() << ", current unique word count " << count << ", current load factor: " << load << endl;
    }
}

template <class HashedObj>
void HashTable<HashedObj>::changeInfo(const HashedObj & x, const string & y, const bool & t)
{   //changes the word x's information to y, t tracks if we're gonna add a new item or modify it
    List<HashedObj> & whichList = theLists[hashh( x, theLists.size( ) ) ];
    ListItr<HashedObj> itr = whichList.find( x );
    if (!itr.isPastEnd( ))
        whichList.changeInfo(itr, y, t);
}

template <class HashedObj>
string HashTable<HashedObj>::findLatestInfo(const HashedObj & x) //returns the latest info
{
    List<HashedObj> & whichList = theLists[hashh( x, theLists.size( ) ) ];
    ListItr<HashedObj> itr = whichList.find( x );
    return itr.retrieveInfo();
}

template <class HashedObj>
const vector<string> & HashTable<HashedObj>::findAllInfo(const HashedObj & x)
{   //returns a reference to the information vector of the word x
    List<HashedObj> & whichList = theLists[hashh( x, theLists.size( ) ) ];
    ListItr<HashedObj> itr = whichList.find( x );
    return itr.AllInfo();
}


template <class HashedObj>
void HashTable<HashedObj>::PrintItAll() //prints everything (debugging)
{
    for (int i = 0; i < theLists.size(); i ++)
    {
        cout << "HASH[" << i << "] = " << endl;
        theLists[i].printer();
        cout << endl;
    }
}

template <class HashedObj>
void HashTable<HashedObj>::remove( const HashedObj & x )
{
    // remove x from the appropriate list
    theLists[hashh( x, theLists.size( ) )].remove( x );
    count--; //reduce the count
}

template <class HashedObj>
void HashTable<HashedObj>::stats()
{
    double load = double(count)/theLists.size();
    cout << "After preprocessing, the unique word count is " << count << ". Current load ratio is " << load << "." << endl;
}

template <class HashedObj>
const HashedObj & HashTable<HashedObj>::find (const HashedObj & x) const
{   //finds x in the hashtable
    ListItr<HashedObj> itr;
    itr = theLists[ hashh( x, theLists.size( ) ) ].find( x );
    if (itr.isPastEnd( ) )
        return ITEM_NOT_FOUND;
    return itr.retrieve( );
}

template <class HashedObj>
HashTable<HashedObj>::~HashTable()
{   //destroyer of the worlds
    makeEmpty();
}

#endif
