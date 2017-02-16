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
    for (int i=0; i<9; i++){
        int xdividerSpace = 0;
        if (i%3==0) ydividerSpace+=4;
        
        for (int j=0; j<9; j++){
            if (j%3==0) xdividerSpace+= 4;
            board_[i][j].setValues( renderer_, font_, (xOffset_ + (j*cellSize_)) + xdividerSpace , yOffset_ + (i*cellSize_) + ydividerSpace, cellSize_, i, j);
        }
    }
}


void Sudoku::render()
{
    for (int i=0; i<9; i++){
        for (int j=0; j<9; j++){
            board_[i][j].render();
        }
    }
    
    //Draw blue horizontal line
    SDL_SetRenderDrawColor( renderer_, 0x00, 0x00, 0x00, 0xFF );
    
    // Outer Borders
    
    //SDL_Rect fillRect = { xOffset_, , SCREEN_HEIGHT -(2*13), 4 };
    //SDL_RenderFillRect( renderer_, &fillRect );
  
    int diviserSpace=0;
    
    // Draw vertical lines
    for (int i =0; i<10; i++){
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
    for (int i =0; i<10; i++){
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


void Sudoku::handleEvent(SDL_Event *e)
{
    //If mouse event happened
    if( e->type == SDL_MOUSEBUTTONDOWN)
    {
        int row=0, col=0;
        bool inside = false;
        for (int i=0; i<9; i++){
            for (int j=0; j<9; j++){
                if( (inside=board_[i][j].handleFocusEvent(e)) ){
                    row = i;
                    col = j;
                    i=9;
                    j=9;
                }
            }
        }
        
        //Mouse clicked outside cells
        if ( !inside){
            if( focusedCell!=nullptr) focusedCell->setFocused(false);
        }
        //Mouse clicked inside cell
        else
        {
            if(focusedCell != &board_[row][col]){
                if(focusedCell!=nullptr)focusedCell->setFocused(false);
                focusedCell = &board_[row][col];
                focusedCell->setFocused(true);
            }
            /*//Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                    break;
            }*/
        }
    }
    else if( e->type == SDL_KEYDOWN )
    {
        if( focusedCell!=nullptr) focusedCell->handleInputEvent(e);
    }
}



// Game Logic

bool Sudoku::insertCellValue(int row, int col, int value)
{
    if(!isValidInsertion(row, col, value)) return false;
    board_[row][col].setValue(value);
    board_[row][col].setBlocked(true);
    return true;
}

bool Sudoku::isValidInsertion(int row, int col, int value) const
{
    for(int i=0; i< boardSize_; i++){
        if( board_[row][i].getValue()==value) return false;
    }
    
    for(int i=0; i< boardSize_; i++){
        if( board_[i][col].getValue()==value) return false;
    }
    
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( board_[i][j].getValue()==value) return false;
        }
    }
    return true;
}

bool Sudoku::solveSudoku()
{
    computeInitialBoardState();
    
    while (!cellToBeSolved.empty())
    {
        Coordinates coords = cellToBeSolved.front();
        cellToBeSolved.pop();
        
        std::cout << "Viewing (" << coords.getRow() << "," << coords.getCol() << ") . missing " << cellToBeSolved.size() << std::endl;
        
        Cell::State action = board_[coords.getRow()][coords.getCol()].getMode();
        board_[coords.getRow()][coords.getCol()].setRemovedQueue();
        
        switch(action){
                
            case Cell::STATE_SOLVED:{
                int value = board_[coords.getRow()][coords.getCol()].getValue();
                
                updateBoardState( coords.getRow(),coords.getCol(),value);
                break;
            }
            case Cell::STATE_ROW :
                verifyRowConditionals(coords.getRow());
                break;
                
            case Cell::STATE_COL:
                verifyColConditionals(coords.getCol());
                break;
                
            case Cell::STATE_NOK:
                break;
                
            case Cell::STATE_ROWCOL:
                verifyColConditionals(coords.getCol());
                verifyRowConditionals(coords.getRow());
                
                std::bitset<9> blockState = getBlockState(coords.getRow(),coords.getCol()); // View missing values from block
                
                for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
                    if( !blockState[value]){
                        Coordinates* cell = verifyIfOnlyPoss(coords.getRow()/3, coords.getCol()/3, value+1);
                        if( cell != nullptr){
                            board_[cell->getRow()][cell->getCol()].setValue(value+1);
                            if( !board_[cell->getRow()][cell->getCol()].isQueued()) cellToBeSolved.push( (*cell) );
                            board_[cell->getRow()][cell->getCol()].setQueued(Cell::STATE_SOLVED);
                        }
                    }
                }
                
                break;
        }
    }
    searchAlgorithm();
    
    
    return true;
}



bool Sudoku::computeInitialBoardState(){
    
    // Compute the bitset possibilities for each row
    std::vector<std::bitset<9>> horizontalStates;
    for(int i=0; i<boardSize_; i++) horizontalStates.push_back( getHorizontalState(i));
    
    //Compute the bitset possibilities for each column and
    std::vector<std::bitset<9>> verticalStates;
    for(int i=0; i<boardSize_; i++){ verticalStates.push_back(getVerticalState(i)); }
    
    
    //Compute the bitset possibilities for each Block
    for(int i=0; i< boardSize_; i++){
        std::bitset<9> blockState;
        for(int j=0; j < boardSize_; j++){
            if(j%3==0) blockState = getBlockState(i,j); // When entering a block(3x3) compute bitset
            
            if( board_[i][j].getValue()==0){ //Only compute bitset for unsolved cells
                
                std::bitset<9> finalState = ( horizontalStates[i] | verticalStates[j] | blockState);
                board_[i][j].setValuePossibility(finalState);
                
                // Cells with just one possibility are queued, state updated
                if( board_[i][j].getNumberPossibilities()==1){
                    board_[i][j].fixValue();
                    if(!board_[i][j].isQueued()  )cellToBeSolved.push( Coordinates(i,j));
                    board_[i][j].setQueued(Cell::STATE_SOLVED);
                }
            }
            
            if(i%3==2 && j%3==2){ // When entering a block for the last time
                for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
                    if( !blockState[value]){
                        Coordinates* cell = verifyIfOnlyPoss(i/3, j/3, value+1);
                        if( cell != nullptr){
                            board_[cell->getRow()][cell->getCol()].setValue(value+1);
                            if(!board_[cell->getRow()][cell->getCol()].isQueued()  )cellToBeSolved.push( (*cell));
                            board_[cell->getRow()][cell->getCol()].setQueued(Cell::STATE_SOLVED);
                            
                        }
                    }
                }
            }
            
        }
    }
    
    for(int i=0; i<boardSize_; i++){
        verifyRowConditionals(i);
        verifyColConditionals(i);
    }
    
    return true;
}

void Sudoku::verifyRowConditionals(int row)
{
    std::unordered_map< std::bitset<9>, int> values;
    std::vector< std::bitset<9> > existantValues;
    
    // create map and vec with existant bitsets
    for (int i=0; i<boardSize_; i++) {
        if( board_[row][i].getValue()==0){
            if(values.find(board_[row][i].getPossibleValues()) != values.end()){
                values[board_[row][i].getPossibleValues()]++;
            }
            else{
                values[board_[row][i].getPossibleValues()]=1;
                existantValues.push_back(board_[row][i].getPossibleValues());
            }
        }
    }
    // verify if there is exclusive
    for (auto it : values){
        //std::cout << " " << it.first << ":" << it.second << std::endl;
        if( (9-it.first.count())== it.second ) std::cout << it.first << " is exclusive\n";
    }
    
    // Exclusive Groups with occurance == existantbits
    for (int i=0; i<boardSize_; i++) {
        std::bitset<9> exclusiveSet = board_[row][i].getPossibleValues();
        if( values[exclusiveSet] == board_[row][i].getNumberPossibilities()){
            std::cout << "found exclusive pair = ("<< row << "," << i << ")\n";
            std::bitset<9> exclusiveSetFlipped = exclusiveSet;
            exclusiveSetFlipped.flip();
            // found exclusive group
            for (int j=0; j<boardSize_; j++) {
                // update all remaining
                if( board_[row][j].getValue()==0 && board_[row][j].getPossibleValues()!= exclusiveSet){
                    std::bitset<9> previousSet = board_[row][j].getPossibleValues();
                    board_[row][j].setValuePossibility( previousSet |exclusiveSetFlipped);
                    previousSet ^=board_[row][j].getPossibleValues();
                    if(board_[row][j].getNumberPossibilities()==1){ // add is final
                        board_[row][j].fixValue();
                        if(!board_[row][j].isQueued()) cellToBeSolved.push( Coordinates(row,j));
                        board_[row][j].setQueued(Cell::STATE_SOLVED);
                    }else if (previousSet.any()){ // add if narrowed possibilities
                        if(!board_[row][j].isQueued()  )cellToBeSolved.push( Coordinates(row,j));
                        board_[row][j].setQueued(Cell::STATE_COL);
                    }
                }
            }
        }
    }
    
    // Check is group of exclusive
    std::bitset<9>* result;
    if( (result=checkExclusiveGroup(existantValues))!=nullptr){
        std::bitset<9> toRemove = *result;
        toRemove.flip();
        for(int i=0; i<9; i++){
            bool valid = ((*result) | board_[row][i].getPossibleValues()) == board_[row][i].getPossibleValues(); // is part of the group
            if(!valid && board_[row][i].getValue()==0){ // update remaining not from group
                std::bitset<9> previousSet = board_[row][i].getPossibleValues();
                board_[row][i].setValuePossibility(previousSet |toRemove);
                previousSet = previousSet^board_[row][i].getPossibleValues();
                if(board_[row][i].getNumberPossibilities()==1){
                    board_[row][i].fixValue();
                    if(!board_[row][i].isQueued()) cellToBeSolved.push( Coordinates(row,i));
                    board_[row][i].setQueued(Cell::STATE_SOLVED);
                }else if ( previousSet.any()){
                    if(!board_[row][i].isQueued()) cellToBeSolved.push( Coordinates(row,i));
                    board_[row][i].setQueued(Cell::STATE_COL);
                }
                
            }
        }
    }
    
}

void Sudoku::verifyColConditionals(int col)
{
    std::unordered_map< std::bitset<9>, int> values;
    std::vector< std::bitset<9> > existantValues;
    for (int i=0; i<boardSize_; i++) {
        if( board_[i][col].getValue()==0){
            if(values.find(board_[i][col].getPossibleValues()) != values.end()){
                values[board_[i][col].getPossibleValues()]++;
            }
            else{
                values[board_[i][col].getPossibleValues()]=1;
                existantValues.push_back(board_[i][col].getPossibleValues());
            }
        }
    }
    
    
    for (auto it : values){
        //std::cout << " " << it.first << ":" << it.second << std::endl;
        if( (9-it.first.count())== it.second ) std::cout << it.first << " is exclusive\n";
    }
    
    
    for (int i=0; i<boardSize_; i++) {
        std::bitset<9> exclusiveSet = board_[i][col].getPossibleValues();
        if( values[exclusiveSet] == board_[i][col].getNumberPossibilities()){
            std::cout << "found exclusive pair = ("<< i << "," << col << ")\n";
            std::bitset<9> exclusiveSetFlipped = exclusiveSet;
            exclusiveSetFlipped.flip();
            for (int j=0; j<boardSize_; j++) {
                if( board_[j][col].getValue()==0 && board_[j][col].getPossibleValues()!= exclusiveSet){
                    std::bitset<9> previousSet = board_[j][col].getPossibleValues();
                    board_[j][col].setValuePossibility( previousSet|exclusiveSetFlipped);
                    previousSet^=board_[j][col].getPossibleValues();
                    if(board_[j][col].getNumberPossibilities()==1){
                        board_[j][col].fixValue();
                        if(!board_[j][col].isQueued()) cellToBeSolved.push( Coordinates(j,col));
                        board_[j][col].setQueued(Cell::STATE_SOLVED);
                    }else if (previousSet.any()){ // add if narrowed possibilities
                        if(!board_[j][col].isQueued()  )cellToBeSolved.push( Coordinates(j,col));
                        board_[j][col].setQueued(Cell::STATE_ROW);
                    }
                }
            }
        }
    }
    
    std::bitset<9>* result;
    if( (result=checkExclusiveGroup(existantValues))!=nullptr){
        std::bitset<9> toRemove = *result;
        toRemove.flip();
        for(int i=0; i<9; i++){
            bool valid = ((*result) | board_[i][col].getPossibleValues()) == board_[i][col].getPossibleValues();
            if(!valid && board_[i][col].getValue()==0){
                std::bitset<9> previousSet = board_[i][col].getPossibleValues();
                board_[i][col].setValuePossibility( previousSet|toRemove);
                previousSet ^= board_[i][col].getPossibleValues();
                if(board_[i][col].getNumberPossibilities()==1){
                    board_[i][col].fixValue();
                    if(!board_[i][col].isQueued()) cellToBeSolved.push( Coordinates(i,col));
                    board_[i][col].setQueued(Cell::STATE_SOLVED);
                }else if( previousSet.any()){
                    if(!board_[i][col].isQueued()) cellToBeSolved.push( Coordinates(i,col));
                    board_[i][col].setQueued(Cell::STATE_ROW);
                }
            }
        }
    }
}



std::bitset<9> Sudoku::getHorizontalState(int row) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i=0; i< boardSize_; i++){
        cellValue = board_[row][i].getValue();
        if( cellValue !=0) lineState[cellValue-1]=true;
    }
    return lineState;
}

std::bitset<9> Sudoku::getVerticalState(int col) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i=0; i< boardSize_; i++){
        cellValue = board_[i][col].getValue();
        if( cellValue !=0) lineState[cellValue-1]=true;
    }
    return lineState;
}

std::bitset<9> Sudoku::getBlockState(int row, int col) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            cellValue = board_[i][j].getValue();
            if( cellValue!=0) lineState[cellValue-1]=true;
        }
    }
    return lineState;
}


Coordinates* Sudoku::verifyIfOnlyPoss(int blockRow, int blockCol, int value) const
{
    int row=-1,col=-1;
    int val_counter=0;
    for(int i= blockRow*3; i< (blockRow+1)*3; i++){
        for(int j= blockCol*3; j< (blockCol+1)*3; j++){
            if(!board_[i][j].getValuePossibility(value) && board_[i][j].getValue()==0){
                val_counter++;
                row = i;
                col = j;
            }
        }
    }
    if( val_counter==1) return new Coordinates(row,col);
    else return nullptr;
}



void Sudoku::updateBoardState(int row, int col, int value)
{
    for(int i=0; i< boardSize_; i++){
        if( board_[row][i].getValue()==0){
            if(board_[row][i].setValuePossibility(value, true) ){
                if( !board_[row][i].isQueued() )  cellToBeSolved.push( Coordinates(row,i));
                board_[row][i].setQueued(Cell::STATE_ROWCOL);
            }
            if( board_[row][i].getNumberPossibilities()==1 && !board_[row][i].isQueued()){
                if( !board_[row][i].isQueued() )cellToBeSolved.push( Coordinates(row,i));
                board_[row][i].setQueued(Cell::STATE_SOLVED);
            }
        }
    }
    
    for(int i=0; i< boardSize_; i++){
        if( board_[i][col].getValue()==0){
            if(board_[i][col].setValuePossibility(value, true)){
                if( !board_[i][col].isQueued() )  cellToBeSolved.push( Coordinates(i,col));
                board_[i][col].setQueued(Cell::STATE_ROWCOL);
            }
            if( board_[i][col].getNumberPossibilities()==1){
                if( !board_[i][col].isQueued() )  cellToBeSolved.push( Coordinates(i,col));
                board_[i][col].setQueued(Cell::STATE_SOLVED);
            }
        }
    }
    
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( board_[i][j].getValue()==0 ){
                if(board_[i][j].setValuePossibility(value, true)){
                    if( !board_[i][j].isQueued()) cellToBeSolved.push( Coordinates(i,j));
                    board_[i][j].setQueued(Cell::STATE_ROWCOL);
                    
                }
                if( board_[i][j].getNumberPossibilities()==1){
                    if( !board_[i][j].isQueued()) cellToBeSolved.push( Coordinates(i,j));
                    board_[i][j].setQueued(Cell::STATE_SOLVED);
                }
            }
        }
    }
    
    
    std::bitset<9> blockState = getBlockState(row,col); // View missing values from block
    
    for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
        if( !blockState[value]){
            Coordinates* cell = verifyIfOnlyPoss(row/3, col/3, value+1);
            if( cell != nullptr){
                board_[cell->getRow()][cell->getCol()].setValue(value+1);
                if( !board_[cell->getRow()][cell->getCol()].isQueued()) cellToBeSolved.push( (*cell) );
                board_[cell->getRow()][cell->getCol()].setQueued(Cell::STATE_SOLVED);
            }
        }
    }
}

std::bitset<9>* Sudoku::checkExclusiveGroup( std::vector< std::bitset<9> > vec){
    
    std::vector<bool> v(vec.size());
    
    
    for(int i=3 ; i < vec.size(); i++){
        std::fill(v.begin(), v.begin() + i, true);
        
        do {
            
            std::bitset<9> total( std::string("000000000"));
            for (int i = 0; i < vec.size(); ++i) {
                if (v[i]) {
                    std::bitset<9> aux = vec[i];
                    total = total| (aux.flip());
                }
            }
            if( total.count()==i){
                std::cout << "Group Exclusive -> " << total << std::endl;
                std::bitset<9> affected( std::string("000000000"));
                for (int i = 0; i < vec.size(); ++i) {
                    if( v[i]) affected.flip(i);
                }
                return new std::bitset<9>(total.flip());
            }
            
        } while (std::prev_permutation(v.begin(), v.end()));
        
    }
    return nullptr;
}


void Sudoku::searchAlgorithm(){
    int remain=0;
    for(int i=0; i< boardSize_; i++){
        for (int j=0; j< boardSize_; j++) {
            if( board_[i][j].getValue()==0) remain++;
        }
    }
    
    
    std::array<Cell, 81> arr;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            arr[ (9*i) + j] = board_[i][j];
        }
    }
    searchRecursive(arr, remain, true , Coordinates(0,0));
}


bool Sudoku::searchRecursive( std::array<Cell, 81> curr_board , int remain, bool first, Coordinates coords){
    
    if(remain==0){
        for(int i=0; i < boardSize_ ; i++){
            for (int j=0; j<boardSize_; j++) {
                solution_[i][j] = curr_board[(9*i)+j];
            }
        }
        return true;
    }
    if(!first){
        int value = curr_board[(9*coords.getRow())+coords.getCol()].getValue();
        
        std::cout << "Updating (" << coords.getRow() << "," << coords.getCol() << ") with " << value << " ... missing" << remain << std::endl;
        
        for(int i=0; i< boardSize_; i++){
            if( curr_board[(9*coords.getRow())+i].getValue()==0){
                curr_board[(9*coords.getRow())+i].setValuePossibility(value, true);
            }
            if( curr_board[(9*coords.getRow())+i].getNumberPossibilities()==0) return false;
        }
        
        for(int i=0; i< boardSize_; i++){
            if( curr_board[(9*i)+coords.getCol()].getValue()==0){
                curr_board[(9*i)+coords.getCol()].setValuePossibility(value, true);
                if( curr_board[(9*i)+coords.getCol()].getNumberPossibilities()==0) return false;
            }
        }
        
        for(int i = (coords.getRow()/3)*3; i < (((coords.getRow()/3)+1)*3); i++){
            for( int j = (coords.getCol()/3)*3;  j < ((coords.getCol()/3)+1)*3; j++){
                if( curr_board[(9*i)+j].getValue()==0 ){
                    curr_board[(9*i)+j].setValuePossibility(value, true);
                    if( curr_board[(9*i)+j].getNumberPossibilities()==0) return false;
                }
            }
        }
        
    }
    
    
    std::size_t min = 9;
    Coordinates coords_new;
    for(int i=0; i< boardSize_; i++){
        for (int j=0; j< boardSize_; j++) {
            if ( curr_board[(9*i)+j].getValue()==0 && curr_board[(9*i)+j].getNumberPossibilities()<min) {
                min = curr_board[(9*i)+j].getNumberPossibilities();
                coords_new.setRow(i);
                coords_new.setCol(j);
                if (min ==1){
                    i=9;
                    j=9;
                }
            }
        }
    }
    
    int row =coords_new.getRow();
    int col =coords_new.getCol();
    std::bitset<9> cellPoss = curr_board[(row*9)+col].getPossibleValues();
    
    for(int i=0; i< boardSize_; i++){
        if( !cellPoss[i]){
            curr_board[(row*9)+col].setValue(i+1);
            if(searchRecursive(curr_board, remain-1, false, coords_new)) return true;
        }
    }
    return false;
}



void Sudoku::displaySolution() const
{
    std::cout << HORIZONTAL_AXIS << std::endl;
    std::cout << EXTERIOR_BOUND << std::endl;
    
    int cellvalue;
    for(int i = 0; i < boardSize_; i++ ){
        std::cout << " " << (i+1) << " |";
        for(int j = 0; j < boardSize_; j++){
            cellvalue = solution_[i][j].getValue();
            if( cellvalue == 0){
                std::cout << "   |";
            }
            else{
                std::cout << " " << cellvalue << " |";
            }
        }
        std::cout << std::endl;
        std::cout << EXTERIOR_BOUND << std::endl;
    }
}










