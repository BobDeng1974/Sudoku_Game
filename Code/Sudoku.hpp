//
//  Sudoku.hpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Sudoku_hpp
#define Sudoku_hpp


#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <iostream>
#include <queue>
#include <bitset>
#include <unordered_map>
#include <array>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>

#include "Cell.hpp"
#include "SodukuSolver.hpp"

#include "Handler.hpp"



#define HORIZONTAL_AXIS "     A   B   C   D   E   F   G   H   I  "
#define EXTERIOR_BOUND  "   +---+---+---+---+---+---+---+---+---+"


class Sudoku
{
    public:
        Sudoku(SDL_Renderer* renderer, TTF_Font* font,  int screenWidth, int screenHeight);

        // Render sudoku component
        void render(bool modeVerify=false);
    
        // Handle event on sudoku board
        Handler handleEvent( SDL_Event* e);
    
        // Reset board
        void reset();
    
        //Game Functions
        bool insertCellValue(int row, int col, int value);
        virtual bool solveSudoku();
        bool buildFromFile(std::string path);
        void showAndBlockCell();
    
        // Block all Cells
        void blockBoard();
    
        // Get Number of empty blocks
        int getNoEmptyBlock() const;
        // Verify if board is finished and correct
        bool isAllCorrect() const;
    
    private:
        // Board thick line size
        static const int thickLineSize=4;
    
        // Sudoku Screen Position
        int xOffset_;
        int yOffset_;
        int boardWidth_;
        int boardHeight;
        int cellSize_;
    
        // SDL variables
        SDL_Renderer* renderer_ = nullptr;
        TTF_Font* font_ = nullptr;
    
        // Number of Cells per line
        int horizontalCellNo = 9;
        int verticalCellNo = 9;

        // Game board and board with solution
        std::vector<Cell> board_;
        std::vector<Cell> solution_;
    
        // Pointer to clicked cell
        Cell *focusedCell = nullptr;
    
        // Random number generator
        std::minstd_rand0* generator_;
   
        // Creates the board (all cells)
        void createBoard();
    
        // Verifies if an insertion is valid
        bool isValidInsertion(int row, int col, int value) const;
    

    
    

};

#endif /* Sudoku_hpp */
