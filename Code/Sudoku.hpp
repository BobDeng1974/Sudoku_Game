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
#include "SodukuSolver.hpp"


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

    
    private:
        static const int thickLineSize=4;
    
        int xOffset_;
        int yOffset_;
        int boardWidth_;
        int boardHeight;
        int cellSize_;
    
        SDL_Renderer* renderer_ = nullptr;
        TTF_Font* font_ = nullptr;
    
        std::vector<Cell> board_;
        std::vector<Cell> solution_;
    
        Cell *focusedCell = nullptr;
    
        void createBoard();
    
        //Solving Logic
        int boardSize_=9;
    
        bool isValidInsertion(int row, int col, int value) const;
    

    
    

};

#endif /* Sudoku_hpp */
