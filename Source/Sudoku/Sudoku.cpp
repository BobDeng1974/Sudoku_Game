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
    
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator_ = new std::minstd_rand0(seed);
}

// Creates the board
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

// Renders the board
void Sudoku::render(bool modeVerify)
{
    // Render each cell
    for (int i=0; i<horizontalCellNo; i++){
        for (int j=0; j<verticalCellNo; j++){
            if(isCustomMode())
                 board_[(i*9) + j].render( modeVerify, 0);
            else board_[(i*9) + j].render( modeVerify, solution_[(i*9) + j].getValue());
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

// Handle board events
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
                if( (inside=board_[ (i*9)+j].handleFocusEvent(e, customMode_)) ){
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
    else if( e->type == SDL_KEYDOWN ) // input events, only affect focused cell
    {
        if( focusedCell!=nullptr){
            handleInputEvent(e);
            handler.setEvent(Handler::EVENT_INPUT);
        }
    }
    return handler;
}

// Handles input events on focused cell
void Sudoku::handleInputEvent( SDL_Event* e)
{
    Coordinates coords = focusedCell->getCoordinates();
    switch( e->key.keysym.sym )
    {
        case SDLK_1:    // pressed 1
            updateCellValue(coords.getRow(), coords.getCol(), 1);
            break;
            
        case SDLK_2:    // pressed 2
            updateCellValue(coords.getRow(), coords.getCol(), 2);
            break;
            
        case SDLK_3:    // pressed 3
            updateCellValue(coords.getRow(), coords.getCol(), 3);
            break;
            
        case SDLK_4:    // pressed 4
            updateCellValue(coords.getRow(), coords.getCol(), 4);
            break;
            
        case SDLK_5:    // pressed 5
            updateCellValue(coords.getRow(), coords.getCol(), 5);
            break;
            
        case SDLK_6:    // pressed 6
            updateCellValue(coords.getRow(), coords.getCol(), 6);
            break;
            
        case SDLK_7:    // pressed 7
            updateCellValue(coords.getRow(), coords.getCol(), 7);
            break;
            
        case SDLK_8:    // pressed 8
            updateCellValue(coords.getRow(), coords.getCol(), 8);
            break;
            
        case SDLK_9:    // pressed 9
            updateCellValue(coords.getRow(), coords.getCol(), 9);
            break;
        case SDLK_BACKSPACE:    //backspace -> erase value
            focusedCell->setValue(0);
            focusedCell->setBlocked(false);
            break;
    }
}
// Updates Cell value, permissions depend on active modes
void Sudoku::updateCellValue(int row, int col, int value)
{
    
    if( easyMode_){ // Allows only if value is possible
        if( isValidInsertion( row, col, value))
            focusedCell->setValue( value );
    }
    else focusedCell->setValue(value); // Allows any value
    
    if( customMode_) focusedCell->setBlocked(true); // If custom input is considered part of puzzle configuration
}

// Build board from file
bool Sudoku::buildFromFile( std::string path)
{
    std::ifstream f_input;
    f_input.open(path);
    if (!f_input.is_open()) return false;
    
    // Read line by line
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
        if(!insertCellValue(row - '1',  col - 'A', value - '0')){ // Confirms input is valid
            return false;
        }
    }
    f_input.close();
    
    initialState_ = board_;
    return true;
}

// Inserts cell value if is valid
bool Sudoku::insertCellValue(int row, int col, int value)
{
    if(!isValidInsertion(row, col, value)) return false;
    board_[(row*9)+col].setValue(value);
    board_[(row*9)+col].setBlocked(true);
    return true;
}

// Verifies if a value insertion is valid according to sudoku rules
bool Sudoku::isValidInsertion(int row, int col, int value) const
{
    // verifies column
    for(int i=0; i< horizontalCellNo; i++){
        if( board_[(row*9)+i].getValue()==value) return false;
    }
    // verifies rows
    for(int i=0; i< verticalCellNo; i++){
        if( board_[(i*9)+col].getValue()==value) return false;
    }
    // verifies block
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( board_[(i*9)+j].getValue()==value) return false;
        }
    }
    return true;
}

// Call for solving sudoku
bool Sudoku::solveSudoku()
{
    SudokuSolver solver(9, 9);
    solution_ = solver.solvePuzzle(board_);
    return true;
}

// Reset sudoku to initial state or completly if eraseAll
void Sudoku::reset( bool eraseAll)
{
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            board_[(i*9) + j].reset();
            if( !eraseAll && initialState_[(i*9) + j].getValue()!=0){
                board_[(i*9) + j].setBlocked(true);
                board_[(i*9) + j].setValue(initialState_[(i*9) + j].getValue());
            }
        }
    }
    
}
// Function for Hint, shows and blocks a cell
void Sudoku::showAndBlockCell()
{
    int missing= getNoEmptyBlock();
    
    // Chosen one of the missing to show
    int chosen = (*generator_)() % missing;
    missing = 0;
    for (std::size_t i = 0; i< board_.size(); i++) {
        if( board_[i].getValue()==0){
            if(missing==chosen){
                board_[i].setBlocked(true);
                board_[i].setValue( solution_[i].getValue());
                i = board_.size();
            }
            else missing++;
        }
    }
}
// Blocks the whole board, when puzzle is correct and finished
void Sudoku::blockBoard()
{
    for(std::size_t i = 0; i < board_.size(); i++){
        board_[i].setBlocked(true);
        board_[i].setFocus(false);
    }
}

// getter for number of missing cells
int Sudoku::getNoEmptyBlock() const
{
    int missing=0;
    for(Cell cell:board_)
        if( cell.getValue()==0)
            missing++;
    return missing;
}
// verifies if puzzle is correct
bool Sudoku::isAllCorrect() const
{
    bool success = true;
    for(std::size_t i = 0 ; i<board_.size() && success==true; i++)
        if(board_[i].getValue() != solution_[i].getValue())
            success=false;
    return success;
}

// Setters
void Sudoku::setEasyMode(bool easyMode) { easyMode_ = easyMode; }
void Sudoku::setCustomMode(bool customMode) { customMode_ = customMode; }
void Sudoku::setInitialStateBoard(){ initialState_ = board_;}
// Getters
bool Sudoku::isEasyMode() const { return easyMode_; }
bool Sudoku::isCustomMode() const { return customMode_; }




