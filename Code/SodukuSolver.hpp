//
//  SodukuSolver.hpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 16/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef SodukuSolver_hpp
#define SodukuSolver_hpp


#include <stdio.h>
#include <vector>
#include <queue>

#include "Cell.hpp"
#include "Coordinates.hpp"

class SudokuSolver
{
    public:
        SudokuSolver(int boardWidth, int boardHeight);

        virtual std::vector<Cell> solvePuzzle( std::vector<Cell> &board);
    
    protected:
        virtual bool computeInitialBoardState();
        virtual bool updateCells( int row, int col, int value);
    
    private:
        std::vector<Cell> solution_;
        std::queue <Coordinates> cellToBeSolved;

        int boardWidth_;
        int boardHeight_;
    
        std::bitset<9> getHorizontalState(int row) const;
        std::bitset<9> getVerticalState(int col) const;
        std::bitset<9> getBlockState(int row, int col) const;
        Coordinates* verifyIfOnlyPoss(int row, int col, int value) const;

    
        void searchAlgorithm();
        bool searchRecursive( std::vector<Cell> curr_board , int remain, bool first, Coordinates coords);
    
};

#endif /* SodukuSolver_hpp */
