//Created by Zeynep Turkmen on 10.04.2022.

#include <string>
#include <iostream>
#include <chrono> //to track the time
#include <fstream> //for the "data" file
#include "BST.h"
#include "AVL.h"
using namespace std;

/* *******************************************************************************************
 Important Information = I wanted to follow 2 different approaches for the BST and AVL, in order to try new things :3
 I created nested trees for both of them and implemented the ITEM_NOT_FOUND value as "-1". Basically the big tree has Nodes that has Trees as its elements and those trees have nodes that has string as their elemets. (They are templated classes but I used strings for this main.cpp)
 Sample = Courseworks -> CS300 CS210 MATH204 ECON201 CS306
 For the BST I added the big title (like "Courseworks" from above example) as an element to the subtree along with CS306,ECON201 etc, because the root node doesn't change by rotation like AVL.
 For the AVL tree, since the rotations may change the location of the root element I added a private member called "title" to store the big title "Courseworks" and added CS306,CS210 as the elements of a subtree.
 Also the input file "data.txt" is hardcoded.
 ********************************************************************************************/

void menu()
{
    cout << "MENU" << endl;
    cout << "Please enter an input between [1 - 6]: " << endl;
    cout << "1- Display the sections [AVL]" << endl;
    cout << "2- Display the sections [BST]" << endl;
    cout << "3- Select a section" << endl;
    cout << "4- Add new section" << endl;
    cout << "5- Delete a section" << endl;
    cout << "6- Exit" << endl;
}

int main()
{
    BinarySearchTree<BinarySearchTree<string>> BST(BinarySearchTree<string> ("-1")); //creates a nested Binary Tree (main tree)
    AvlTree<AvlTree<string>> AVL(AvlTree<string>("-1")); //creates a nested AVL Tree (main tree)
    
    ifstream input;
    string filename = "data.txt", line, secName; //secName = name of the section
    input.open(filename.c_str());
    cout << "Welcome to the Notebook!" << endl << endl;
    
    long long timerBST = 0, timerAVL = 0; //to track time
    
    if(!input.fail())
    {
        getline(input, line); //gets first section name into "line"
        
        while(!input.eof())
        {
            BinarySearchTree<string> * b = new BinarySearchTree<string>("-1"); //new BST substree for a new section
            AvlTree<string> * a = new AvlTree<string>("-1", line); //new AVL substree for a new section
            
            if (line.at(0) != '-') //means its a new notebook section, hence its another binary tree itself
            {
                secName = line;
                
                auto start = std::chrono::high_resolution_clock::now();
                b->insert(line); //section name inserted
                auto end = std::chrono::high_resolution_clock::now();
                timerBST = (end - start).count() / 1000.0;
                
                getline(input, line); //got another line
                while(line.at(0) == '-' && !input.eof()) //if it starts with - add it to the current binary tree
                {
                    start = std::chrono::high_resolution_clock::now();
                    b->insertToSubTree(line.substr(1,line.rfind("-")-1), line.substr(line.rfind("-")+1)); //inserts the information and the item to the BST subtree
                    end = std::chrono::high_resolution_clock::now(); //ending the timer
                    timerBST = timerBST + ((end - start).count() / 1000.0);
                    
                    auto start2 = std::chrono::high_resolution_clock::now();
                    a->insertToSubTree(line.substr(1,line.rfind("-")-1), line.substr(line.rfind("-")+1)); //inserts the information and the item to the AVL subtree
                    auto end2 = std::chrono::high_resolution_clock::now();
                    timerAVL = timerAVL + ((end2 - start2).count() / 1000.0); //time for adding the previous insertions to the subtree
                    
                    getline(input, line);
                }
                auto start2 = std::chrono::high_resolution_clock::now();
                AVL.insert(*a); //inserting subtree to the big tree
                auto end2 = std::chrono::high_resolution_clock::now();
                timerAVL = timerAVL + ((end2 - start2).count() / 1000.0); //total time for AVL insertion
                
                cout << "Section \"" << secName << "\" has been inserted into the AVL notebook." << endl;
                cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
                
                start = std::chrono::high_resolution_clock::now();
                BST.insert(*b); //inserting subtree to the big tree
                end = std::chrono::high_resolution_clock::now();
                timerBST = timerBST + ((end - start).count() / 1000.0); //total time for BST insertion
                cout << "Section \"" << secName << "\" has been inserted into the BST notebook." << endl;
                cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl << endl;
                
                
            }
        }
        int i = 0; //for options of the menu
        menu();
        
        while (i != 6)
        {
            cout << "Input: ";
            cin >> i;
            
            if (i == 1) //display the sections AVL
            {
                cout << endl << "********" << endl;
                AVL.displaySections(); //pretty self explanatory
                cout << "********";
                cout << endl << endl;
            }
            else if (i == 2) //display the sections BST
            {
                cout << endl << "********" << endl;
                BST.displaySections();
                cout << "********";
                cout << endl << endl;
            }
            else if (i == 3) //Selecting, editing sections
            {
                string section;
                cout << "Enter the title of the section: ";
                cin >> section;
                int s = 0; //to go through the submenu
                
                //dummy pointers to attach to the subtrees from the big trees
                BinarySearchTree<string> * dummyBST = new BinarySearchTree<string>("-1");
                dummyBST->insert(section); //to be able to use the find function
                AvlTree<string> * dummyAVL = new AvlTree<string>("-1", section); //(section becomes the title of the tree by the constructor)
                
                if (BST.find(*dummyBST).isEmpty()) //it cant be found basically
                {
                    cout << "Invalid title!" << endl << endl;
                    delete dummyBST; //freeing the memory
                    delete dummyAVL;
                }
                else
                {
                    cout << "Selected section -> " << section << endl;
                    //since operator = is overloaded, i made functions to free the previous memory and attach the root of dummy to the subtree from the big tree - to be able to edit them
                    dummyBST->changeRoot(BST.find(*dummyBST));
                    dummyAVL->changeRoot(AVL.findSubSection(*dummyAVL));
                    
                    cout << "Please enter an input between [1 - 7]: " << endl;
                    cout << "1- Display the items [AVL]" << endl;
                    cout << "2- Display the items [BST]" << endl;
                    cout << "3- Display the information of a item" << endl;
                    cout << "4- Add new item" << endl;
                    cout << "5- Update the information of a item" << endl;
                    cout << "6- Delete an item" << endl;
                    cout << "7- Return to main menu" << endl;
                    
                    while (s != 7)
                    {
                        cout << "Input: ";
                        cin >> s;
                        
                        if (s == 1) //display items AVL
                        {
                            cout << endl << "*****" << endl;
                            dummyAVL->printTree();
                            cout << "*****" << endl << endl;
                        }
                        else if (s == 2) //display items BST
                        {
                            cout << endl << "*****" << endl;
                            dummyBST->printTree2();
                            cout << "*****" << endl << endl;
                        }
                        else if (s == 3) //displaying the information
                        {
                            string title, found;
                            cout << "Enter the title of the item: ";
                            cin >>title;
                            
                            auto start2 = std::chrono::high_resolution_clock::now();
                            found = dummyAVL->find(title); //look if the item exists
                            auto end2 = std::chrono::high_resolution_clock::now();
                            timerAVL = (end2 - start2).count() / 1000.0;
                            
                            auto start = std::chrono::high_resolution_clock::now();
                            found = dummyBST->find(title);
                            auto end = std::chrono::high_resolution_clock::now();
                            timerBST = (end - start).count() / 1000.0;
                    
                            
                            if (found == "-1") //it doesn't exist
                            {   cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
                                cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;
                                cout << "Invalid title." << endl << endl;
                            }
                            else //it exists
                            {
                                string info;
                
                                start2 = std::chrono::high_resolution_clock::now();
                                info = dummyAVL->findInfo(title); //basically the same find function from the lecture but instead of the element, it returns the information
                                end2 = std::chrono::high_resolution_clock::now();
                                timerAVL = timerAVL + ((end2 - start2).count() / 1000.0);
                                
                                start = std::chrono::high_resolution_clock::now();
                                info = dummyBST->findInfo(title);
                                end = std::chrono::high_resolution_clock::now();
                                timerBST = timerBST + ((end - start).count() / 1000.0);
                                
                                cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
                                cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;
                                cout << info << endl << endl;
                            }
                        }
                        else if (s == 4) //adding a new item
                        {
                            string title, found;
                            cout << "Enter a title for the item: ";
                            cin >> title;
                            
                            auto start2 = std::chrono::high_resolution_clock::now();
                            found = dummyAVL->find(title); //checking if it exists
                            auto end2 = std::chrono::high_resolution_clock::now();
                            timerAVL = (end2 - start2).count() / 1000.0;
                            
                            auto start = std::chrono::high_resolution_clock::now();
                            found = dummyBST->find(title);
                            auto end = std::chrono::high_resolution_clock::now();
                            timerBST = timerBST + (end - start).count() / 1000.0;
                            
                            if (found == "-1") //if it doesnt exist
                            {
                                string description;
                                cout << "Enter a description for the item: ";
                                cin >> description;
                                
                                if (dummyAVL->isEmpty()) //if the root gives NULL, it acts weird and doesn't attach the items to the big tree (cuz the previous thing aka .changeRoot() doesnt work as the root is NULL)
                                {
                                    AVL.removeSubTree(*dummyAVL); //so i first remove it
                                    start2 = std::chrono::high_resolution_clock::now();
                                    dummyAVL->insertToSubTree(title, description); //add an element to it
                                    end2 = std::chrono::high_resolution_clock::now();
                                    AVL.insert(*dummyAVL); //and then attach it to the big tree
                                }
                                else //if the root isnt NULL there is no issue - regular insertion
                                {
                                    start2 = std::chrono::high_resolution_clock::now();
                                    dummyAVL->insertToSubTree(title, description);
                                    end2 = std::chrono::high_resolution_clock::now();
                                }
                                timerAVL = timerAVL + (end2 - start2).count() / 1000.0;
                                
                                start = std::chrono::high_resolution_clock::now();
                                dummyBST->insertToSubTree(title, description); //add to the subtree
                                end = std::chrono::high_resolution_clock::now();
                                timerBST = timerBST + (end - start).count() / 1000.0;
                
                                cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
                                cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;
                                cout << "The new item \"" << title << "\" has been inserted." << endl << endl;
                            }
                            else
                            {
                                cout << "Item \"" << title << "\" already exist in the \"" << section << "\"." << endl << endl;
                            }
                        }
                        else if (s == 5) //Updating the information
                        {
                            string title, found;
                            cout << "Enter a title of the item: ";
                            cin >> title;
                            
                            auto start2 = std::chrono::high_resolution_clock::now();
                            found = dummyAVL->find(title);
                            auto end2 = std::chrono::high_resolution_clock::now();
                            timerAVL = (end2 - start2).count() / 1000.0;
                            
                            auto start = std::chrono::high_resolution_clock::now();
                            found = dummyBST->find(title);
                            auto end = std::chrono::high_resolution_clock::now();
                            timerBST = timerBST + (end - start).count() / 1000.0;
                            
                            if (found != "-1")
                            {
                                string description;
                                cout << "Enter the new information: ";
                                cin >> description;
                                
                                start2 = std::chrono::high_resolution_clock::now();
                                dummyBST->changeDescription(title, description); //changes the description accordingly to the input
                                end2 = std::chrono::high_resolution_clock::now();
                                timerAVL = timerAVL + (end2 - start2).count() / 1000.0;
                                
                                start = std::chrono::high_resolution_clock::now();
                                dummyAVL->changeDescription(title, description);
                                end = std::chrono::high_resolution_clock::now();
                                timerBST = timerBST + (end - start).count() / 1000.0;
                                
                                cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
                                cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;
                                cout << "The content \"" << title << "\" has been updated." << endl << endl;
                            }
                            else
                            {
                                cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
                                cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;
                                cout << "Item \"" << title << "\" does not exist in the " << section << "." << endl << endl;
                            }
                        }
                        else if (s == 6) //deleting an item
                        {
                            string title, found;
                            cout << "Enter the title of the item: ";
                            cin >> title;
                            
                            auto start2 = std::chrono::high_resolution_clock::now();
                            found = dummyAVL->find(title); //same as aboveee
                            auto end2 = std::chrono::high_resolution_clock::now();
                            timerAVL = (end2 - start2).count() / 1000.0;
                            
                            auto start = std::chrono::high_resolution_clock::now();
                            found = dummyBST->find(title);
                            auto end = std::chrono::high_resolution_clock::now();
                            timerBST = timerBST + (end - start).count() / 1000.0;
                            
                            if (found != "-1")
                            {
                                start2 = std::chrono::high_resolution_clock::now();
                                dummyBST->remove(title); //removes the item "title"
                                end2 = std::chrono::high_resolution_clock::now();
                                timerAVL = timerAVL + (end2 - start2).count() / 1000.0;
                                
                                start = std::chrono::high_resolution_clock::now();
                                dummyAVL->remove(title); //removes the item "title"
                                end = std::chrono::high_resolution_clock::now();
                                timerBST = timerBST + (end - start).count() / 1000.0;
                                
                                cout << "[AVL] Elapsed time: " << timerAVL << " microseconds" << endl;
                                cout << "[BST] Elapsed time: " << timerBST << " microseconds" << endl;
                                cout << "The item \"" << title << "\" has been deleted." << endl << endl;
                            }
                            else
                            {
                                cout << "Item \"" << title << "\" does not exist in the \"" << section << "\"." << endl << endl;
                            }
                        }
                        else
                            menu();
                    }
                }
            }
            else if (i == 4) //adding a new section
            {
                string title;
                cout << "Enter the title of the item: ";
                cin >> title;
                BinarySearchTree<string> * dummy = new BinarySearchTree<string>("-1");
                dummy->insert(title);
                AvlTree<string> * dummy2 = new AvlTree<string>("-1", title);
                
                if (BST.find(*dummy).isEmpty()) //it cant be found
                {
                    BST.insert(*dummy); //insert it to the big trees
                    AVL.insert(*dummy2);
                    cout << "The new section \"" << title << "\" has been inserted." << endl << endl;
                }
                else
                {
                    cout << "Section \"" << title << "\" already exists." << endl << endl;
                    delete dummy;
                    delete dummy2;
                }
            }
            else if (i == 5) //delete a section
            {
                string title;
                cout << "Enter the title of the section: ";
                cin >> title;
                BinarySearchTree<string> * dummy = new BinarySearchTree<string>("-1");
                dummy->insert(title);
                AvlTree<string> * dummy2 = new AvlTree<string>("-1", title);
                
                if (!BST.find(*dummy).isEmpty()) //if found
                {
                    BST.remove(*dummy); //remove function from lecture
                    AVL.removeSubTree(*dummy2); //same remove function, only the comparisons are done accordingly to the private member "title"
                    cout << "The section has been deleted." << endl << endl;
                }
                else
                {
                    cout << "Section \"" << title << "\" does not exist." << endl << endl;
                    delete dummy;
                    delete dummy2;
                }
            }
        }
    }
    else
        cout << "Failed to open the file!" << endl;
    
    return 0;
}
