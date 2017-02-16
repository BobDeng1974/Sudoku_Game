//
//  Sudoku.hpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Sudoku_hpp
#define Sudoku_hpp

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <queue>
#include <bitset>
#include <unordered_map>
#include <array>
#include <vector>

#include "Cell.hpp"


#define HORIZONTAL_AXIS "     A   B   C   D   E   F   G   H   I  "
#define EXTERIOR_BOUND  "   +---+---+---+---+---+---+---+---+---+"


class Sudoku
{
    public:
        Sudoku(SDL_Renderer* renderer, TTF_Font* font,  int screenWidth, int screenHeight);

        void render();
    
        void handleEvent( SDL_Event* e);
    
        //Game Functions
        bool insertCellValue(int row, int col, int value);
        virtual bool solveSudoku();
        virtual void displaySolution() const;

    
    protected:
        virtual bool computeInitialBoardState();
        void verifyRowConditionals(int row);
        void verifyColConditionals(int col);
        virtual void updateBoardState( int row, int col, int value);
    
    
    private:
        static const int thickLineSize=4;
    
        int xOffset_;
        int yOffset_;
        int boardWidth_;
        int boardHeight;
        int cellSize_;
    
        SDL_Renderer* renderer_ = nullptr;
        TTF_Font* font_ = nullptr;
    
        Cell board_[9][9];
        Cell solution_[9][9];
        Cell *focusedCell = nullptr;
    
        void createBoard();
    
        //Solving Logic
        int boardSize_=9;
        std::queue< Coordinates > cellToBeSolved;
    
        bool isValidInsertion(int row, int col, int value) const;
        Coordinates* verifyIfOnlyPoss(int row, int col, int value) const;
    
        std::bitset<9>* checkExclusiveGroup( std::vector< std::bitset<9> > vec);
    
        void searchAlgorithm();
        bool searchRecursive(std::array<Cell, 81> curr_board, int remain, bool first , Coordinates coords);
    
        std::bitset<9> getHorizontalState(int row) const;
        std::bitset<9> getVerticalState(int col) const;
        std::bitset<9> getBlockState(int row, int col) const;
    
    

};

#endif /* Sudoku_hpp */
