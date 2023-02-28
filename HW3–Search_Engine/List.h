//  Created by Zeynep Turkmen on 05.05.2022.
#ifndef _LIST_H
#define _LIST_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <class Comparable>
class ListItr;
template <class Comparable>
class List;

template <class Object>
        class ListNode
        {
            ListNode( const Object & theElement = Object( ), ListNode * n = NULL)
                  : element( theElement ), next( n ){ }

            Object   element;
            vector<string> info; //vector of informations such as { a.txt, 5}
            ListNode *next;
            friend class List<Object>;
            friend class ListItr<Object>;
        };

template <class Object>
 class ListItr
 {
      public:
         ListItr( ) : current( NULL ) { }
         bool isPastEnd( ) const
         {   return current == NULL;   }
         void advance( )
         {   if ( ! isPastEnd( ) )   current = current->next;   }
         const Object & retrieve( ) const //retrieves the element at the current itr
         {   
             return current->element;
         }
         const string & retrieveInfo( ) const //retrieves the latest info
         {
             return current->info[current->info.size()-1];
         }
         const int infosize( ) const //returns the size of information string
         {
             return current->info.size();
         }
         void pushInfo(string s) const //pushes s into the informationVector
         {
             return current->next->info.push_back(s);
         }
         const string & infAt(int a) const //returns the information at index a
         {
             return current->info[a];
         }
         const vector<string> & AllInfo() const
         {
             return current->info;
         }

       private:
           ListNode<Object> *current;    // Current position

         ListItr( ListNode<Object> *theNode )
                     : current( theNode ) { }

           friend class List<Object>;    // Grant access to constructor
  };

template <class Object>
class List
{
      public:
            List( );
            List( const List & rhs );
            ~List( );
            bool isEmpty( ) const;
            void makeEmpty( );
            ListItr<Object> zeroth( ) const;
            ListItr<Object> first( ) const;
            void insert( const Object & x, const ListItr<Object> & p);
            void changeInfo(const ListItr<Object> & p, const string & y, const bool &);
            ListItr<Object> find( const Object & x ) const;
            ListItr<Object> findPrevious( const Object & x ) const;
            void remove( const Object & x );
            void printer();
            const List<Object> & operator=( const List<Object> & rhs );

       private:
            ListNode<Object> *header;
 };

template <class Object>
List<Object>::List( )
{
    header = new ListNode<Object>;
}

template <class Object>
bool List<Object>::isEmpty( ) const
{ //checks if the list is empty
    return header->next == NULL;
}

template <class Object>
void List<Object>::insert( const Object & x, const ListItr<Object> & p)
{   //inserts x after the location p
    if ( p.current != NULL )
        p.current->next = new ListNode<Object>( x, p.current->next);
}

template <class Object>
void List<Object>::printer() //prints the list along with its information
{
    ListNode<Object> *itr = header->next;
    while ( itr != NULL)
    {
        cout << itr->element << " ";
        for (int g=0; g < itr->info.size(); g++)
            cout  << itr->info[g] << " ";
        cout << endl;
        itr = itr->next;
    }
}

template <class Object>
void List<Object>::changeInfo(const ListItr<Object> & p, const string & newinfo, const bool & t)
{  //changes or modifies the information of p into newinfo, t tracks if its to be modified or to be added
      if ( p.current != NULL )
      {
          int idx = p.current->info.size() - 1;
          if (!t)
              p.current->info[idx] = newinfo;
          else
              p.current->info.push_back(newinfo);
      }
}

template <class Object>
ListItr<Object> List<Object>::find( const Object & x ) const
{  //Return iterator pointing to the first occurence of x
    ListNode<Object> *itr = header->next; // Initialize
    while ( itr != NULL && itr->element != x )
        itr = itr->next;
    return ListItr<Object>( itr );
}

template <class Object>
ListItr<Object> List<Object>::findPrevious( const Object & x ) const
{ //find the previous node of x
     ListNode<Object> *itr = header;
     while ( itr->next != NULL && itr->next->element != x )
             itr = itr->next;
    return ListItr<Object>( itr );
}

template <class Object>
void List<Object>::remove( const Object & x )
{  // Remove the first x in the list
    ListItr<Object> p = findPrevious( x ); // Locate previous of x, if any
    if ( p.current->next != NULL )
    {
        ListNode<Object> *oldNode = p.current->next;
        p.current->next = p.current->next->next; // Bypass deleted node
        delete oldNode;
    }
}

template <class Object>
ListItr<Object> List<Object>::zeroth( ) const
{ //returns an iterator that points to the first element
   return ListItr<Object>( header );
}

template <class Object>
ListItr<Object> List<Object>::first( ) const
{  //returns an iterator that points to the first element
    return ListItr<Object>( header->next );
}

 template <class Object>
 void List<Object>::makeEmpty( )
 { //destroys the list
        while ( ! isEmpty( ) )
               remove( first( ).retrieve( ) );
 }

 template <class Object>
 const List<Object> & List<Object>::operator=( const List<Object> & rhs )
 { //helps deep copy
     if ( this != &rhs )
     {
           makeEmpty( );

           ListItr<Object> ritr = rhs.first( );
           ListItr<Object> itr = zeroth( );
         for (  ;  ! ritr.isPastEnd( ); ritr.advance( ), itr.advance( ) )
         {
             insert( ritr.retrieve( ), itr );
             for (int i=0; i < ritr.infosize(); i++) //copies the information vector
             {
                 string s = ritr.infAt(i);
                 itr.pushInfo(s);
             }
         }
      }
    return *this;
 }

 template <class Object>
 List<Object>::List( const List<Object> & rhs )
 {
        header = new ListNode<Object>;
       *this = rhs;  // makes Deep Copy
 }

template <class Object>
List<Object>::~List()
{
    makeEmpty();
}
#endif 
