//  Created by Zeynep Turkmen on 05.05.2022.
#include <iostream>
#include <string>
#include <fstream> //for reading the files
#include <sstream> //to read the user query
#include <chrono> //to compare the time
#include "BST.h" //binary search tree
#include "strutils.h" //turning string into integers vice versa
#include "theHash.h" //hash table

using namespace std;

BinarySearchTree<string> bst("-1"); //using "-1" as the itemNotFound value
HashTable<string> hashTable("-1");

struct wordNinfo //struct to store a word along with its info that consists of =  { a.txt, 6}, { b.txt, 16}, etc..
{
    string word;
    vector<string> info;
}; //This struct is mainly used when checking the user query as I started without reading and implemented my functions differently

string outputInformation(string & fname, vector<wordNinfo> & everything, bool & flag) //everything is the vector that has all the user requested words along with their information. fname is the file that we are currently checking if it exists in everything
{
    string outs = "in Document " + fname; //outs is the string thats gonnna be printed later on
    for (int i=0; i < everything.size(); i++) //while going through everything
    {
        flag = false;
        for (int j=0; j< everything[i].info.size();j++)
       {
            string f = everything[i].info[j].substr(2,everything[i].info[j].find(",")-2);
            if (f == fname) //check the words info to see if it exists on the fname
            {
                flag = true; //sets the word as found
                outs = outs + ", " + everything[i].word + " was found " +  everything[i].info[j].substr(everything[i].info[j].find(",")+2, everything[i].info[j].length()-everything[i].info[j].find(",")-3) + " times"; //append the amount of times it was found
            }
       }
        if (!flag) //if the document doesnt have any of the words
            return "No document contains the given query";
    }
        return outs + ".";
}

void incrementer(string & info) //takes something like { a.txt, 1} as input and increments the number -->  { a.txt, 2}
{
    string f = info.substr((info.find(",")+2), (info.find("}")-info.find(",")-2));
    int the = atoi(f);
    the ++;
    info = info.substr(0, info.find(",")+2) + itoa(the) + "}";
}

void addWords(string & mystring, string fname) //takes mystring aka the word and adds it to the bst and hashtable
{
    if(mystring != "")
    {
        if (bst.find(mystring) == "-1") //it wasnt added before
        {
            string info = "{ " + fname + ", " + "1" + "}"; //creates the information string
            bst.insert(mystring);
            hashTable.insert(mystring);
            bst.changeDescription(mystring, info, true); //adds the info as its description
            hashTable.changeInfo(mystring, info, true); //adds the info as its description
        }
        else
        {
            string info = hashTable.findLatestInfo(mystring); //it gets the latest info on mstring

            if (fname != info.substr(2,info.find(",")-2)) //if the latest info is not the current file
            {
                string f= "{ " + fname + ", " + "1" + "}";
                hashTable.changeInfo(mystring, f, true); //add a new info to the info vector
                bst.changeDescription(mystring, f, true);
            }
            else //if the latest info is the one we are modifying, increment the number
            {   incrementer(info);
                bst.changeDescription(mystring, info, false);
                hashTable.changeInfo(mystring, info, false);
            }
        }
    }
    mystring = "";
}

void createVecBst(vector<wordNinfo> & VEC, string & mystring, long int & b) //creates the everything vector inputting all the information on the string aka word mystring. Tracks the time into b variable
{
    if(mystring != "")
    {
        wordNinfo r;
        r.word = mystring;
        vector<string> mystringInfo;
        int k = 100;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            if (bst.find(mystring) != "-1")
            {
                mystringInfo = bst.AllInfo(mystring); //copies the information vector
            }
        }
        auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start);
        b = b + BSTTime.count() / k;
        r.info = mystringInfo;
        VEC.push_back(r); //adds it to the information vector
    }
}

void createVecHash(vector<wordNinfo> & VEC2, string & mystring, long int & h)
{   //exactly the same stuff as above
    if(mystring != "")
    {
        int k = 100;
        wordNinfo r2;
        r2.word = mystring;
        vector<string> info2;
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < k; i++)
        {
            if (hashTable.find(mystring) != "-1")
            {
                info2 = hashTable.findAllInfo(mystring);
            }
        }
        auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
                          (std::chrono::high_resolution_clock::now() - start);
        h = h + HTTime.count() / k;
        r2.info = info2;
        VEC2.push_back(r2);
        //ok so we need to insert this {“b.txt”, 4}
    }
}

int main()
{
    int fileAmount, a=1; //a is the loop variable
    cout << "Enter number of input files: ";
    cin >> fileAmount;
    
    string fname, mystring ="";
    ifstream input;
    char ch;
    
    vector<string> fnameVEC; //vector that has all the file names
    
    while (a <= fileAmount)
    {
        cout << "Enter " << a << ". file name: ";
        cin >> fname;
        fnameVEC.push_back(fname);
        a++;
    }
    
    a = 1;
    while (a <= fileAmount) //going through all the files requested
    {
        fname = fnameVEC[a-1];
        input.open(fname.c_str());
        
        while(input.get(ch))
        {
            if ((ch >= 'A') && (ch <= 'Z')) //if uppercase make em lovercase
                   ch = ch - ('A' - 'a');
            if ((ch >= 'a') && (ch <= 'z')) //if its a letter
            {
                mystring = mystring + ch; //add it to mystring
            }
            else //caught a non letter soo mystring is ready to be added
            {
                addWords(mystring, fname);
            }
        }
        a++;
        input.close();
    }
    addWords(mystring, fname); //adding the last word after escaping the loop
    hashTable.stats(); //prints the current load factor and the amount of words in the table
    string line;
    cin.ignore();
    cout << "Enter queried words in one line: "; //gets the query from the user
    getline(cin, line); //gets the query from the user into the string "line"
    istringstream ss(line); //makes line readible char by char
    
    mystring = "";
    vector<wordNinfo> vecBST, vecHASH;
    
    long int b = 0, h = 0; //set the timer stuff to 0
    while (ss.get(ch))
    {
        if ((ch >= 'A') && (ch <= 'Z')) //make em lovercase
               ch = ch - ('A' - 'a');
        if ((ch >= 'a') && (ch <= 'z')) //if its a letter
        {
            mystring = mystring + ch;
        }
        else //caught a non letter soo the same idea as above
        {
            createVecBst(vecBST, mystring, b);
            createVecHash(vecHASH, mystring, h);
            mystring = "";
        }
    }
    createVecBst(vecBST, mystring, b); //reading the last words
    createVecHash(vecHASH, mystring, h);

    bool flag = false;
    int count = 0; //to track if all files doesnt have all the words
    
    for (int i = 0; i < fnameVEC.size(); i++) //going through the BST solution (aka the vector obtained from BST)
    {
        flag = false;
        mystring = outputInformation(fnameVEC[i], vecBST, flag); //what needs to be printed is in mystring now
        if (flag) //if the file had all the words
            cout << mystring << endl; //print its information
        else
            count ++;
    }
    if (count == fnameVEC.size()) //if all words gave flag false
        cout << mystring << endl; //print "No document contains the given query" (only once!)

    count = 0; //below is the same as above only for HASH
    for (int i = 0; i<fnameVEC.size(); i++) //going through the HASH solution (aka the vector obtained from HASHtable)
    {
        flag = false;
        mystring = outputInformation(fnameVEC[i], vecHASH, flag);
        if (flag)
            cout << mystring << endl;
        else
            count ++;
    }
    if (count == fnameVEC.size())
        cout << mystring << endl;
    
    cout << "\nTime: " << b << "\n"; //print the total time taken by BST
    cout << "\nTime: " << h << "\n"; //print the total tme taken by HASH
    cout << "Speed Up: " << double(b) / h << endl;
    
    return 0;
}
