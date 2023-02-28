// Created by Zeynep Türkmen on 28.05.2022.
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector> //to store the data
#include <chrono> //to count the time
#include "strutils.h"
using namespace std;

struct contact //it stores the data and is comparable with the overloaded operators
{
    string name;
    string surname;
    string number;
    string city;
    bool operator < (contact & c) //it first orders them by name, surname and then the number
    {
        if (name != c.name)
            return (name < c.name);
        else if (surname != c.surname)
            return (surname < c.surname);
        else
            return (number < c.number);
    }
    bool operator <= (contact & c)
    {
        if (name != c.name)
            return (name <= c.name);
        else
            return (surname <= c.surname);
    }
    bool operator > (contact & c)
    {
        return (! (*this <= c));
    }
    bool operator == (string & c) //it says its equal if the inputted word is located in the beginnning of name and surname no matter how long it is
    {
        string s2 = name + " " + surname;
        if (s2.find(c) == 0)
        {
            return true;
        }
        return false;
    }
    bool operator < (string & c)
    {
        string s2 = name + " " + surname;
        return (s2 < c);
    }
};

void contactPrint(contact & c, string & printme) //modifies the inputted string as needed
{
   printme = printme + UpperString(c.name) + " " + UpperString(c.surname) + " " + c.number + " " + c.city + "\n";
}

template <class Comparable>
void bsearch(vector<Comparable>& list, string & key, string & printme) //modified binary search algorithm
{
    int mid, high = list.size()-1, low = 0;
    bool flag = true;
    while (low <= high && flag)
    {
        mid = (low + high)/2;
        if (list[mid] == key) //if it finds one mathing thing
        {
            int prev = mid;
            while (list[prev] == key) //it goes back to the first occurence of that word
            {
                prev --;
            }
            prev ++;
            while (list[prev] == key)
            {
                contactPrint(list[prev], printme); //and writes the string till the last occurence
                prev ++ ;
            }
            flag = false;
        }
        else if (list[mid] < key)
            low = mid + 1;
        else
            high = mid - 1;
    }
}

template <class Comparable>
void ssearch(vector<Comparable> & a, string & s, string & printme) //classic sequential search algorithm, only it stops iterating after finding the last occurence as the list is sorted (no need to waste supplies)
{
    int k;
    for(k=0; k < a.size(); k++)
    {
        while (a[k] == s)
        {
            contactPrint(a[k], printme);
            k++;
        }
    }
}

inline int leftChild( int i )
{
    return 2*i+1;
}

template <class Comparable>
void heapsort(vector<Comparable> & a) //classic heap sort taken from cs300 slides
{
    for (int i = a.size()/2; i >=0; i--)
        percDown(a, i, a.size());
    for (int j = a.size() -1; j >0; j--)
    {
           swap(a[0], a[j]);    // swap max to the last pos.
          percDown(a, 0, j); // re-form the heap
    }
}

template <class Comparable> //another helper function for heap sort taken from cs300 slides
void percDown( vector<Comparable> & a, int i, int n )
{
    int child;
    Comparable tmp;
    
    for (tmp=a[i] ; leftChild(i) < n; i = child )
    {
        child = leftChild(i);
        if ( child != n-1 && a[ child  ] < a[ child+1 ] )
            child++;
        if ( a[child ] > tmp )
            a[i] = a[ child ];
        else
            break;
    }
    a[ i ] = tmp;
}

template <class Comparable>
void insertionSort( vector<Comparable> & a, int left, int right ) //classic insertion sort taken from cs300 slides
{ //only soring from the left index to the right index
    for ( int p = left + 1; p <= right; p++ )
    {
        Comparable tmp = a[ p ];
        int j;
        
        for ( j = p; j > left && tmp < a[ j - 1 ]; j-- )
            a[ j ] = a[ j - 1 ];
        a[ j ] = tmp;
    }
}

template <class Comparable>
inline void swapME( Comparable & obj1, Comparable & obj2 ) //classic swapping function
{
     Comparable tmp = obj1;
     obj1 = obj2;
     obj2 = tmp;
}

template <class Comparable>
const Comparable & median3( vector<Comparable> & a, int left, int right ) //finds the median of 3 taken from cs300 slides
 {
       int center = ( left + right ) / 2;
       if ( a[ center ] < a[ left ] )
         swap( a[ left ], a[ center ] );
       if ( a[ right ] < a[ left ] )
         swap( a[ left ], a[ right ] );
       if ( a[ right ] < a[ center ] )
         swap( a[ center ], a[ right ] );

       swap( a[ center ], a[ right - 1 ] );
       return a[ right - 1 ];
 }

template <class Comparable>
void quicksort( vector<Comparable> & a, int left, int right )
{
    if ( left + 10 <= right )
    {
        Comparable pivot = median3( a, left, right );
        int i = left, j = right - 1;
        for ( ; ; )
        {
            while ( a[ ++i ] < pivot ) { }
            
            while ( pivot < a[ --j ] ) { }
            
            if ( i < j )
                swapME( a[ i ], a[ j ] );
            else
                break;
        }
        swapME( a[ i ], a[ right - 1 ] );   // fixing the pivor
        quicksort( a, left, i - 1 );       // sorting smaller side
        quicksort( a, i + 1, right );    // sorting the larger side
    }
    else  // do insertion sort if the size is smaller than 10
        insertionSort( a, left, right );
}
// Helps debugging by printing the vector with its data
void printVEC(vector<contact> & vec2)
{
    for (int i=0; i < vec2.size(); i++)
    {
        cout << vec2[i].name << " " << vec2[i].surname << " " << vec2[i].number << " " << vec2[i].city << endl;
    }
}

template <class Comparable>
void merge(vector<Comparable> & a, int left, int center, int right)//merge function taken from cs300 slides modified to not allocate new memory
{
    int start = center + 1;
    if (a[center] <= a[start]) //if its already sorted
        return;
 
    while (left <= center && start <= right)
    {
        if (a[left] <= a[start]) {
            left++;
        }
        else
        {
            Comparable info = a[start];
            int idx = start;
            //does the necessary shifting
            while (idx != left) {
                a[idx] = a[idx - 1];
                idx--;
            }
            a[left] = info;
            left++; //fixing the pointers
            center++;
            start++;
        }
    }
}

template <class Comparable>
void mergeSort(vector<Comparable> & a, int left, int right)//classic mergesort algorithm taken from cs300 slides
{
    if (left < right) //recursively calls the mergesort function
    {
        int center = (left + right) / 2;
        mergeSort(a, left, center); //first half
        mergeSort(a, center + 1, right); //last half
        merge(a, left, center, right);
    }
}

int main()
{
    string fname, word;
    cout << "Please enter the contact file name: " << endl;
    cin >> fname;
    
    cin.ignore();
    cout << "Please enter the word to be queried: " << endl;
    getline(cin, word);

    ifstream input;
    
    input.open(fname.c_str());
    
    string line;
    
    vector<contact> vec,vec2;
    
    if(!input.fail())
    {
        while (getline(input, line))
        {
            contact c;
            istringstream ss(line);
            ss >> c.name >> c.surname >> c.number >> c.city;
            vec.push_back(c);
        }
    }
    else
        cout << "Failed to open the file!" << endl;

    input.close();
    //ok so at this point we have a vector full of contacts now its time to sort them
    //I need to copy it in order to use different sorting algorithms so thats fun. WAIT its actually pretty easy nevermind
    
    long int quick = 0, insertion = 0, merge = 0, heap = 0, binary = 0, sequential = 0;
    int vecSize = vec.size()-1;
    
    cout << endl << "Sorting the vector copies" << endl;
    cout << "======================================" << endl;
    
    //------------------------------- QUICK SORT -----------------------------------
    //➔ In Quick Sort Algorithm, you are expected to choose the pivot as the median.
    int k = 20;
    for (int i = 0; i < k; i++)
    {
        vec2 = vec;
        auto start = std::chrono::high_resolution_clock::now();
        quicksort(vec2, 0, vecSize);
        auto Time = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        quick = quick + Time.count();
    }
    quick = quick/k;
    cout << "Quick Sort Time: " << quick << " Nanoseconds" << endl;
    
    //----------------------------- INSERTION SORT ---------------------------------
    for (int i = 0; i < k; i++)
    {
         vec2 = vec;
         auto start = std::chrono::high_resolution_clock::now();
         insertionSort(vec2, 0, vecSize);
         auto Time = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        insertion = insertion + Time.count();
    }
    insertion = insertion/k;
    cout << "Insertion Sort Time: " << insertion << " Nanoseconds" << endl;

    //------------------------------- MERGE SORT -----------------------------------
    //➔In Merge Sort, you are expected to have an in-place version. Means no extra memory usage.
    for (int i = 0; i < k; i++)
    {
         vec2 = vec;
         auto start = std::chrono::high_resolution_clock::now();
         mergeSort(vec2, 0, vecSize);
         auto Time = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
        merge = merge +  Time.count();
    }
    merge = merge/k;
    cout << "Merge Sort Time: " << merge << " Nanoseconds" << endl;
    
    //------------------------------- HEAP SORT ------------------------------------
    for (int i = 0; i < k; i++)
    {
         vec2 = vec;
         auto start = std::chrono::high_resolution_clock::now();
         heapsort(vec2);
         auto Time = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
         heap = heap + Time.count();
    }
    heap = heap/k;
    cout << "Heap Sort Time: " << heap << " Nanoseconds" << endl;
    
    //------------------------------ BINARY SEARCH --------------------------------------
    string outPrint;
    cout << endl << "Searching for " << word << endl;
    cout << "======================================" << endl;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        outPrint = "";
        bsearch(vec2, word, outPrint);
    }
    auto Time = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
    binary = Time.count()/double(k);
    if (outPrint != "")
        cout << outPrint;
    else
        cout <<  UpperString(word) << " does NOT exist in the dataset" << endl;
    cout << "Binary Search Time: " << binary << " Nanoseconds" << endl << endl;
    
    //----------------------------- SEQUENTIAL SEARCH ------------------------------------
    cout << "Search results for Sequential Search: " << endl;
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
        outPrint = "";
        ssearch(vec2, word, outPrint);
    }
    Time = std::chrono::duration_cast<std::chrono::nanoseconds> (std::chrono::high_resolution_clock::now() - start);
    sequential = Time.count()/double(k);
    if (outPrint != "")
        cout << outPrint;
    else
        cout << UpperString(word) << " does NOT exist in the dataset" << endl;
    cout << "Sequential Search Time: " << sequential << " Nanoseconds" << endl;
   
    //------------------------------- SPEED UP ---------------------------------------
    cout << endl << "SpeedUp between Search Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Sequential Search/ Binary Search) SpeedUp = " << double(sequential)/binary << endl;
    
    //------------------------------- SPEED UP ---------------------------------------
    cout << endl << "SpeedUps between Sorting Algorithms" << endl;
    cout << "======================================" << endl;
    cout << "(Insertion Sort/ Quick Sort) SpeedUp = " << double(insertion)/quick << endl;
    cout << "(Merge Sort / Quick Sort) SpeedUp = " << double(merge)/quick << endl;
    cout << "(Heap Sort / Quick Sort) SpeedUp = " << double(heap)/quick << endl;
    
    return 0;
}
