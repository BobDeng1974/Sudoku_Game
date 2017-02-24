//
//  Coordinates.cpp
//  SudokuSolver
//
//  Created by Ricardo Martins on 06/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Coordinates.hpp"

Coordinates::Coordinates(){}

Coordinates::Coordinates(int row, int col)
{
    row_=row;
    col_=col;
}

void Coordinates::setRow(int row){ row_=row; }
void Coordinates::setCol(int col){ col_=col; }

int Coordinates::getRow(){ return row_;}
int Coordinates::getCol(){ return col_;}
