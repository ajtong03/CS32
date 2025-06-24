#include "History.h"
#include "globals.h"
#include <iostream>

using namespace std;

History::History(int nRows, int nCols)
: m_row(nRows), m_col(nCols)
{
    //fill 2D array with '.' at each element when game starts
    for(int i = 0; i < nRows; i++)
    {
        for(int j = 0; j < nCols; j++)
        {
            location[i][j] = '.';
        }
    }
}

bool History::record(int r, int c)
{
    if(r < 1 || c < 1 || r > m_row || c > m_col)
        return false; //check if r or c is within bounds of Mesa
    else
    {
        if(location[r - 1][c - 1] == '.') //use - 1 because Mesa begins with [1][1]. 2D array begins at [0][0]
        {
            location[r - 1][c - 1] = 'A'; //assign 'A' if first time attacking
        }
        else if(location[r - 1][c - 1] >= 'A' && location[r - 1][c - 1] < 'Z')
        {
            location[r - 1][c - 1]++; //if previously attacked neighboring gark, increment to next letter; must be < 26 times
        }
        
        return true;
    }
}

void History::display() const
{
    clearScreen();
    
    for(int i = 0; i < m_row; i++)
    {
        for(int j = 0; j < m_col; j++)
        {
            cout << location[i][j];
        }
        cout << endl;
    }
    cout << endl;
    
}
