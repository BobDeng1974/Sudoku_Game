//
//  Sudoku.cpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Sudoku.hpp"

Sudoku::Sudoku(SDL_Renderer* renderer, TTF_Font* font, int screenWidth, int screenHeight)
{
    this->renderer_ = renderer;
    this->font_ = font;
    
    int smallerSize = (screenHeight<screenWidth)? screenHeight: screenWidth;

    cellSize_ = smallerSize/10;
    
    xOffset_ = (cellSize_ - ( (thickLineSize-1)*4))/2;
    yOffset_ = xOffset_;
    
    boardHeight = (cellSize_*9)+ ( (thickLineSize)*4);
    boardWidth_ = boardHeight;
    
    createBoard();
}


void Sudoku::createBoard()
{
    int ydividerSpace = 0;
    for (int i=0; i<horizontalCellNo; i++){
        int xdividerSpace = 0;
        if (i%3==0) ydividerSpace+=4;
        
        for (int j=0; j<verticalCellNo; j++){
            if (j%3==0) xdividerSpace+= 4;
            board_.push_back( Cell(renderer_, font_, (xOffset_ + (j*cellSize_)) + xdividerSpace , yOffset_ + (i*cellSize_) + ydividerSpace, cellSize_, i, j));
        }
    }
}


void Sudoku::render(bool modeVerify)
{
    for (int i=0; i<horizontalCellNo; i++){
        for (int j=0; j<verticalCellNo; j++){
            board_[(i*9) + j].render( modeVerify, solution_[(i*9) + j].getValue());
        }
    }
    
    
    SDL_SetRenderDrawColor( renderer_, 0x00, 0x00, 0x00, 0xFF );
    
     int diviserSpace=0;
    
    // Draw vertical lines
    for (int i =0; i<horizontalCellNo+1; i++){
        if (i%3!=0) SDL_RenderDrawLine(
                                      renderer_,
                                      xOffset_ + (i*cellSize_) + diviserSpace,
                                      yOffset_,
                                      xOffset_ + (i*cellSize_) + diviserSpace,
                                      yOffset_+boardHeight -1);
        else {
            SDL_Rect fillRect = {
                xOffset_ + (i*cellSize_) + diviserSpace,
                yOffset_,
                thickLineSize,
                boardHeight
            };
            SDL_RenderFillRect( renderer_, &fillRect );

            diviserSpace+=4;
        }
    }
    
    // Draw horizontal lines
    diviserSpace=0;
    for (int i =0; i<verticalCellNo+1; i++){
        if (i%3!=0){
            SDL_RenderDrawLine(
                               renderer_,
                               xOffset_,
                               yOffset_ + (i*cellSize_) + diviserSpace,
                               xOffset_+boardWidth_-1,
                               yOffset_ + (i*cellSize_) + diviserSpace);
        }
        else {
            SDL_Rect fillRect = {
                xOffset_ ,
                yOffset_ + (i*cellSize_) + diviserSpace,
                boardWidth_ ,
                thickLineSize
            };
            SDL_RenderFillRect( renderer_, &fillRect );
            
            diviserSpace+=4;
        }
    }
}


Handler Sudoku::handleEvent(SDL_Event *e)
{
    // return Handler
    Handler handler(Handler::EVENT_IGNORE);
    
    //If mouse event happened
    if( e->type == SDL_MOUSEBUTTONDOWN)
    {
        int row=0, col=0;
        bool inside = false;
        for (int i=0; i<horizontalCellNo; i++){
            for (int j=0; j<verticalCellNo; j++){
                if( (inside=board_[ (i*9)+j].handleFocusEvent(e)) ){
                    row = i;
                    col = j;
                    i=9;
                    j=9;
                }
            }
        }
        
        //Mouse clicked outside cells
        if ( !inside){
            if( focusedCell!=nullptr){
                focusedCell->setFocused(false);
                handler.setEvent(Handler::EVENT_INPUT);
            }
        }
        //Mouse clicked inside cell
        else
        {
            if(focusedCell != &board_[(row*9)+col]){
                if(focusedCell!=nullptr)focusedCell->setFocused(false);
                focusedCell = &board_[(row*9)+col];
                focusedCell->setFocused(true);
                handler.setEvent(Handler::EVENT_INPUT);
            }
        }
    }
    else if( e->type == SDL_KEYDOWN )
    {
        if( focusedCell!=nullptr){
            focusedCell->handleInputEvent(e);
            handler.setEvent(Handler::EVENT_INPUT);
        }
    }
    return handler;
}


bool Sudoku::buildFromFile( std::string path)
{
    std::ifstream f_input;
    f_input.open(path);
    if (!f_input.is_open()) return false;
    
    std::string line;
    while ( std::getline(f_input,line) ) {
        
        std::stringstream ss(line);
        
        bool hasFailed=false;
        char row, col, value;
        ss >> row;
        if(ss.fail() || (ss.peek()!=' ' && !ss.eof() && ss.peek()!='\t') )  hasFailed=true;
        ss >> col;
        if(ss.fail() || (ss.peek()!=' ' && !ss.eof() && ss.peek()!='\t') ) hasFailed=true;
        ss >> value;
        if(ss.fail() || (ss.peek()!=' ' && !ss.eof() && ss.peek()!='\t') ) hasFailed=true;
        
        if(!insertCellValue(row - '1',  col - 'A', value - '0')){
            return false;
        }
    }
    f_input.close();
    return true;
}


bool Sudoku::insertCellValue(int row, int col, int value)
{
    if(!isValidInsertion(row, col, value)) return false;
    board_[(row*9)+col].setValue(value);
    board_[(row*9)+col].setBlocked(true);
    return true;
}

bool Sudoku::isValidInsertion(int row, int col, int value) const
{
    for(int i=0; i< horizontalCellNo; i++){
        if( board_[(row*9)+i].getValue()==value) return false;
    }
    
    for(int i=0; i< verticalCellNo; i++){
        if( board_[(i*9)+col].getValue()==value) return false;
    }
    
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( board_[(i*9)+j].getValue()==value) return false;
        }
    }
    return true;
}


bool Sudoku::solveSudoku()
{
    SudokuSolver solver(9, 9);
    
    solution_ = solver.solvePuzzle(board_);
    
    return true;
}


void Sudoku::reset()
{
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            board_[(i*9) + j].reset();
        }
    }
}










