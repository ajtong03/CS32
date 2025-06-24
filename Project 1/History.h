#ifndef History_h
#define History_h
#include "globals.h"

class History
    {
      public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
        
        private:
            char location[MAXROWS][MAXCOLS]; //2D array to display history
            int m_row; //num rows History object has
            int m_col; //num cols History object has
            
    };
#endif /* History_h */
