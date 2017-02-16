//
//  Coordinates.hpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 06/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Coordinates_hpp
#define Coordinates_hpp

#include <stdio.h>

class Coordinates
{
    public:
        Coordinates();
        Coordinates(int row,int col);
    
        int getRow();
        int getCol();
    
        void setRow(int row);
        void setCol(int col);
    
    private:
        int row_=-1;
        int col_=-1;
    
};

#endif /* Coordinates_hpp */
