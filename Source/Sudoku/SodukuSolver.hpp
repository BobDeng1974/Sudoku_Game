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
        // Computes initial board state
        virtual bool computeInitialBoardState();
        // Updates neighbour cell (line,column,block)
        virtual bool updateCells( int row, int col, int value);
    
    private:
        // Contains the solution
        std::vector<Cell> solution_;
        // Queue for cells with final values
        std::queue <Coordinates> cellToBeSolved;

        // Board size
        int boardWidth_;
        int boardHeight_;
        // State getters
        std::bitset<9> getHorizontalState(int row) const;       // for line
        std::bitset<9> getVerticalState(int col) const;         // for column
        std::bitset<9> getBlockState(int row, int col) const;   // for block
        Coordinates* verifyIfOnlyPoss(int row, int col, int value) const;   // verify if a value in a block can only be in one cell

        // Brute force algorithm
        void searchAlgorithm();
        bool searchRecursive( std::vector<Cell> curr_board , int remain, bool first, Coordinates coords);
    
};

#endif /* SodukuSolver_hpp */
