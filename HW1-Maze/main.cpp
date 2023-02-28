//  RatRace
//  Created by Zeynep Turkmen on 24.03.2022.
#include <iostream>
#include <vector> //to hold maze matrices and all mazes together
#include "theStack.h" //just the simplest templated stack
#include "randgen.h" //to break walls by random (taken from CS204 files)
#include <fstream>
#include <string>

using namespace std;

//* Quick important information. All the code was written accordinly to a matrix structure.
//* The 0,0 is the top left corner in every step. I only modified them while writing the output files to the requested format.

struct mazeCell
{   //all walls are present in the beginning.
    int bottomWall = 1;
    int topWall = 1;
    int rightWall = 1;
    int leftWall = 1;
    bool visited = false; //to track if we visited a cell while implementing the maze
    bool visited2 = false; //to track if we visited a cell while solving the maze
    int x; //to store its location values in the matrix
    int y;
    bool tryDown = false; //to track if we failed to go in a direction
    bool tryUp = false;  //while implementing the maze
    bool tryRight = false;
    bool tryLeft = false;
};

//pops the bottom wall of the inputted cell if its not already passed through
bool popBottom(vector<vector<mazeCell>> & maze, mazeCell & temp)
{   //checks if popping bottom is out of scope or if its already passed through
    if (temp.x+1 <= maze.size()-1 && !maze[temp.x+1][temp.y].visited)
    {
        maze[temp.x][temp.y].bottomWall = 0; //breaks the walls down
        maze[temp.x+1][temp.y].visited = true; //and sets it as visited
        maze[temp.x+1][temp.y].topWall  = 0;
        return true;
    }
    return false;
}

bool popTop(vector<vector<mazeCell>> & maze, mazeCell & temp)
{   //checks if popping top is out of scope or if its already passed through
    if (temp.x-1 >= 0 && !maze[temp.x-1][temp.y].visited)
    {
        maze[temp.x][temp.y].topWall = 0; //breaks the walls down
        maze[temp.x-1][temp.y].visited = true; //and sets it as visited
        maze[temp.x-1][temp.y].bottomWall = 0;
        return true;
    }
    return false;
}

bool popRight(vector<vector<mazeCell>> & maze, mazeCell & temp)
{   //checks if popping right is out of scope or if its already passed through
    if (temp.y+1 <= maze[0].size()-1 && !maze[temp.x][temp.y+1].visited)
    {
        maze[temp.x][temp.y].rightWall = 0; //breaks the walls down
        maze[temp.x][temp.y+1].visited = true; //sets is as visited
        maze[temp.x][temp.y+1].leftWall = 0;
        return true;
    }
    return false;
}

bool popLeft(vector<vector<mazeCell>> & maze, mazeCell & temp)
{   //checks if popping left is out of scope or if its already passed through
    if (temp.y-1 >= 0 && !maze[temp.x][temp.y-1].visited)
    {
        maze[temp.x][temp.y].leftWall = 0; //breaks the walls down
        maze[temp.x][temp.y-1].visited = true; //sets it as visited
        maze[temp.x][temp.y-1].rightWall = 0;
        return true;
    }
    return false;
}

bool popRandom(vector<vector<mazeCell>> & maze, mazeCell & temp, theStack<mazeCell> & m, bool & failed)
{   //pops cell walls in a random manner. Keeps going until all options are tried.
    int r;
    RandGen random;
    r  = random.RandInt(1,4);
    
    if (r == 1) //try popping the bottom
    {
        if (popBottom(maze, temp)) //if popping bottom works
        {
            temp = maze[(temp.x+1)][temp.y]; //modify the temp as the bottom cell
            m.push(maze[temp.x][temp.y]); //and push it to the stack
            return true;
        }
        temp.tryDown = true;
    }
    else if (r == 2) //pop left
    {
        if (popLeft(maze, temp)) //if popping left works
        {
            temp = maze[temp.x][temp.y-1]; //modify the temp as the left cell
            m.push(maze[temp.x][temp.y]); //and push it to the stack
            return true;
        }
        temp.tryLeft = true;
    }
    else if (r == 3) // uppp
    {
        if (popTop(maze, temp)) //if popping up works
        {
            temp = maze[temp.x-1][temp.y]; //modify the temp as the top cell
            m.push(maze[temp.x][temp.y]); //and push it to the stack
            return true;
        }
        temp.tryUp = true;
    }
    else
    {
       if (popRight(maze, temp)) //if popping right works
       {
           temp = maze[temp.x][temp.y+1]; //modify the temp as the right cell
           m.push(maze[temp.x][temp.y]); //and push it to the stack
           return true;
       }
       temp.tryRight = true;
    }
    if (temp.tryRight && temp.tryUp && temp.tryLeft && temp.tryDown)
    {   //if we tried every direction and none of them worked (either out of scope or visited before)
        failed = true; //declare we failed to go in any direction in that cell
        return true; //escape the loop
    }
    return false;
}

void ReverseStackPrinter(theStack<mazeCell> & stack, ofstream & pout, const int & M)
{   //appends the numbers to the output file in a reverse manner
    //so that the last element of the stack is the first step of the requested path
    mazeCell t;
    if(stack.isEmpty())
    {
        return;
    }
    stack.pop(t);
    ReverseStackPrinter(stack, pout, M); //pout --> "path out"
    //since my path is also accordingly to the matrix structure, i convert it to the requested structure here
    pout << t.y << " " << (M-t.x-1) << endl;
    //makes x show columns and y shows rows and turns int upside down
}

int main()
{
    int K, M, N;
    
    cout << "Enter the number of mazes: ";
    cin >> K;
    
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N ;
    
    vector<vector<vector<mazeCell>>> vec; //a vector of mazes, mazes stored in matrices
    
    for (int i = 1; i <= K; i++) //generate K number of mazes
    {
        vector<vector<mazeCell>> maze(M,vector<mazeCell>(N)); //creates a maze with all walls closed
        for (int x = 0; x < maze.size(); x++) //sets the x y values of the struct, to use for convenience later on
        {
            for (int y = 0; y < maze[0].size(); y++)
            {
                maze[x][y].x = x;
                maze[x][y].y = y;
            }
        }
        theStack<mazeCell> mazeMaker; //stack to make the maze
        mazeMaker.push(maze[0][0]); //pushing 0 0 to it
        maze[0][0].visited = true;  //set 0 0 as visited
        mazeCell temp = maze[0][0]; //set a temporary mazeCell to work on
        
        while (!mazeMaker.isEmpty()) //until the mazeMaker stack is empty, which means all cells will be visited and all possible directions will be covered
         {
             bool done = false; //to let a cell try its every possible path
             bool failed = false; //to track if we failed to go in any direction from the current cell
             while (!done) //until all possible ways are tried
             {
                 done = popRandom(maze, temp, mazeMaker, failed); //keep trying to pop a wall randomly
             }
             if (failed == true) //if theres no way out of that cell
                 mazeMaker.pop(temp); //pop to go a step back
         }
        vec.push_back(maze); //push the maze to the maze vector
        ofstream out;
        string filename = "maze_" + to_string(i) + ".txt"; //tostring converts integers into strig
        out.open(filename.c_str());
        out << maze.size() << " " << maze[0].size() << endl;

        //output as the requested format which means my 0 0 is its 0 amountOfRows-1
        int z = M-1;
        for (int x = 0; x < maze.size(); x++)
        {
            for (int y = 0; y < maze[0].size(); y++)
            {
                out << "x=" << y  << " y=" << x << " l=" << maze[z][y].leftWall << " r=" << maze[z][y].rightWall << " u=" << maze[z][y].topWall << " d=" << maze[z][y].bottomWall << endl;
            }
            z--;
        } //this changes x and y and flips upside down
        out.close(); //close the output file
    }
    cout << "All mazes are generated." << endl;
    
    //Since all the mazes are generated its time to solve them!
    int myPick,x1,x2,y1,y2;
    
    cout << "Enter a maze ID between 1 and " << K << " inclusive to find a path: ";
    cin >> myPick;
    cout << endl;
    
    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> x1 >> y1;
    cout << endl;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> x2 >> y2;
    cout << endl;
    
    theStack<mazeCell> solve; //solving stack
    vector<vector<mazeCell>> maze; //maze that we will solve
    mazeCell temp; //temporary current cell
    mazeCell prev; //temporary previous cell -helps avoid going to the previous cell that we came from-
  
    //my maze has a matrix structure so i need to convert the given coordinates accordingly to it
    int a,b;
    a = M-1-y1;
    b = x1;
    maze = vec[myPick-1];
    temp = maze[a][b]; //set it as the starting point
    solve.push(temp); //push it to the stack
    //again point conversion
    a = M-1-y2;
    b = x2;
    while (!(temp.x == a && temp.y == b)) //until we reach the requested ending point
    {
        bool failed = false; //to track if it fails trying all possible directions
        //going up if (the upper wall is open, its not the step we came from if it wasnt visited before
        if (maze[temp.x][temp.y].topWall== 0 && !(prev.x == temp.x-1 && prev.y == temp.y) && !maze[temp.x-1][temp.y].visited2)
        {
            maze[temp.x-1][temp.y].visited2 = true; //set it as visited
            prev = temp; //update the preview
            temp = maze[temp.x-1][temp.y]; //update the temp as the new upper cell
            solve.push(maze[temp.x][temp.y]); //push the new cell to the stack
        }
        //going down (same idea as above)
        else if (maze[temp.x][temp.y].bottomWall== 0 && !(prev.x == temp.x+1 && prev.y == temp.y) && !maze[temp.x+1][temp.y].visited2)
        {
            maze[temp.x+1][temp.y].visited2 = true;
            prev = temp;
            temp = maze[temp.x+1][temp.y];
            solve.push(maze[temp.x][temp.y]);
        }
        //going right
        else if (maze[temp.x][temp.y].rightWall== 0 && temp.y+1 <= N && !(prev.x == temp.x && prev.y == temp.y+1) && !maze[temp.x][temp.y+1].visited2)
        {
            maze[temp.x][temp.y+1].visited2 = true;
            prev = temp;
            temp = maze[temp.x][temp.y+1];
            solve.push(maze[temp.x][temp.y]);
        }
        //going left
        else if (maze[temp.x][temp.y].leftWall== 0 && !(prev.x == temp.x && prev.y == temp.y-1) && !maze[temp.x][temp.y-1].visited2)
        {
            maze[temp.x][temp.y-1].visited2 = true;
            prev = temp;
            temp = maze[temp.x][temp.y-1];
            solve.push(maze[temp.x][temp.y]);
        }
        else
            failed = true; //if all directions have failed
    
        if (failed == true)
        {
            solve.pop(temp); //get rid of that cell
            solve.pop(temp); //take the previous one as temp to work on
            solve.push(temp); //and put it back
        }
    }
    ofstream pout; //for path_out
    string filename = "maze_" + to_string(myPick) + "_path_" + to_string(x1) + "_" + to_string(y1) + "_" + to_string(x2) + "_" + to_string(y2) + ".txt"; //set the filename
    pout.open(filename.c_str());
    ReverseStackPrinter(solve, pout, M); //reversely output it from the stack
    pout.close(); //close the output file
    return 0;
}
