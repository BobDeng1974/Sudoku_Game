//
//  SodukuSolver.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 16/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "SodukuSolver.hpp"

SudokuSolver::SudokuSolver( int boardWidth, int boardHeight)
{
    this->boardWidth_ = boardWidth;
    this->boardHeight_ = boardHeight;
}

// Solves puzzle
std::vector<Cell>  SudokuSolver::solvePuzzle( std::vector<Cell> &board)
{
    solution_ = board;

    // Computes initial board state
    computeInitialBoardState();

    // Cycles through queue with cells with fixed value
    while (!cellToBeSolved.empty())
    {
        // Pops cell
        Coordinates coords = cellToBeSolved.front();
        cellToBeSolved.pop();
        
        //std::cout << "Viewing (" << coords.getRow() << "," << coords.getCol() << ") . missing " << cellToBeSolved.size() << std::endl;
        // Update neighbour cells on their possible values
        updateCells(coords.getRow(),coords.getCol(), solution_[ (coords.getRow()*9) + coords.getCol()].getValue() );
   
        /*std::cout << "     A   B   C   D   E   F   G   H   I  " << std::endl;
        std::cout << "   +---+---+---+---+---+---+---+---+---+" << std::endl;
        
        int cellvalue;
        for(int i = 0; i < boardWidth_; i++ ){
            std::cout << " " << (i+1) << " |";
            for(int j = 0; j < boardHeight_; j++){
                cellvalue = solution_[(i*boardWidth_)+j].getValue();
                if( cellvalue == 0){
                    std::cout << "   |";
                }
                else{
                    std::cout << " " << cellvalue << " |";
                }
            }
            std::cout << std::endl;
            std::cout << "   +---+---+---+---+---+---+---+---+---+" << std::endl;
        }*/
    }
    // "Brute Force" Algorithm for puzzle solving
    searchAlgorithm();
    
    // prints puzzle solution
    std::cout << "     A   B   C   D   E   F   G   H   I  " << std::endl;
    std::cout << "   +---+---+---+---+---+---+---+---+---+" << std::endl;
    
    int cellvalue;
    for(int i = 0; i < boardWidth_; i++ ){
        std::cout << " " << (i+1) << " |";
        for(int j = 0; j < boardHeight_; j++){
            cellvalue = solution_[(i*boardWidth_)+j].getValue();
            if( cellvalue == 0){
                std::cout << "   |";
            }
            else{
                std::cout << " " << cellvalue << " |";
            }
        }
        std::cout << std::endl;
        std::cout << "   +---+---+---+---+---+---+---+---+---+" << std::endl;
    }
    
    return solution_;
}

// Computes initial board state, i.e. cell possible values, adds final cells to queue
bool SudokuSolver::computeInitialBoardState(){
    
    // Compute the bitset possibilities for each row
    std::vector<std::bitset<9>> horizontalStates;
    for(int i=0; i<boardWidth_; i++) horizontalStates.push_back( getHorizontalState(i));
    
    //Compute the bitset possibilities for each column and
    std::vector<std::bitset<9>> verticalStates;
    for(int i=0; i<boardHeight_; i++){ verticalStates.push_back(getVerticalState(i)); }
    
    
    //Compute the bitset possibilities for each Block
    for(int i=0; i< boardHeight_; i++){
        std::bitset<9> blockState;
        for(int j=0; j < boardWidth_; j++){
            if(j%3==0) blockState = getBlockState(i,j); // When entering a block(3x3) compute bitset
            
            if( solution_[(i*boardWidth_) + j].getValue()==0){ //Only compute bitset for unsolved cells
                
                std::bitset<9> finalState = ( horizontalStates[i] | verticalStates[j] | blockState);
                solution_[(i*boardWidth_) + j].setValuePossibility(finalState);
                
                // Cells with just one possibility are queued, state updated
                if( solution_[(i*boardWidth_) + j].getNumberPossibilities()==1){
                    solution_[(i*boardWidth_) + j].fixValue();
                    cellToBeSolved.push( Coordinates(i,j));
                }
            }
            
            if(i%3==2 && j%3==2){ // When entering a block for the last time
                blockState = getBlockState(i,j); 
                for(int value=0; value <9; value++){ // analyse if remaining values have only one possibility
                    if( !blockState[value]){
                        Coordinates* cell = verifyIfOnlyPoss(i/3, j/3, value+1);
                        if( cell != nullptr){
                            int index = (cell->getRow()*boardWidth_) + cell->getCol();
                            solution_[ index].setValue(value+1);
                            cellToBeSolved.push( (*cell));
                        }
                    }
                }
            }
            
        }
    }
    return true;
}

// Updates neighbour cells with value. Removes value possibility from them
bool SudokuSolver::updateCells( int row, int col, int value)
{
    // Compute the bitset possibilities for each row
    for(int i=0; i<boardWidth_; i++){
        if( solution_[(row*9)+i].getValue()==0){
            solution_[(row*9)+i].setValuePossibility(value, true);
            if(solution_[(row*9)+i].getNumberPossibilities()==1){
                solution_[(row*9)+i].fixValue();
                cellToBeSolved.push(Coordinates(row, i));
            }
        }
    }
    
    //Compute the bitset possibilities for each column and
    for(int i=0; i<boardHeight_; i++){
        if( solution_[(i*9)+col].getValue()==0){
            solution_[(i*9)+col].setValuePossibility(value, true);
            if(solution_[(i*9)+col].getNumberPossibilities()==1){
                solution_[(i*9)+col].fixValue();
                cellToBeSolved.push(Coordinates(i, col));
            }
        }
    }
    
    
    // Update own block with value, Add is final
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            if( solution_[ (i*boardWidth_) + j].getValue()==0){
                solution_[ (i*boardWidth_) + j].setValuePossibility(value, true);
                if(solution_[ (i*boardWidth_) + j].getNumberPossibilities()==1){
                    solution_[ (i*boardWidth_) + j].fixValue();
                    cellToBeSolved.push(Coordinates(i, j));
                }
            }
        }
    }

    return true;
}

// Gets the state of a line
std::bitset<9> SudokuSolver::getHorizontalState(int row) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i=0; i< boardWidth_; i++){
        cellValue = solution_[ (row*boardWidth_)+i].getValue();
        if( cellValue !=0) lineState[cellValue-1]=true;
    }
    return lineState;
}
// Gets state of column
std::bitset<9> SudokuSolver::getVerticalState(int col) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i=0; i< boardHeight_; i++){
        cellValue = solution_[ (i*boardWidth_) + col].getValue();
        if( cellValue !=0) lineState[cellValue-1]=true;
    }
    return lineState;
}
// Gets state of block (3x3)
std::bitset<9> SudokuSolver::getBlockState(int row, int col) const
{
    std::bitset<9> lineState;
    int cellValue;
    for(int i = (row/3)*3; i < (((row/3)+1)*3); i++){
        for( int j = (col/3)*3;  j < ((col/3)+1)*3; j++){
            cellValue = solution_[ (i*boardWidth_) + j].getValue();
            if( cellValue!=0) lineState[cellValue-1]=true;
        }
    }
    return lineState;
}
// Verifies if a value has only a possible cell in a block
Coordinates* SudokuSolver::verifyIfOnlyPoss(int blockRow, int blockCol, int value) const
{
    int row=-1,col=-1;
    int val_counter=0;
    for(int i= blockRow*3; i< (blockRow+1)*3; i++){
        for(int j= blockCol*3; j< (blockCol+1)*3; j++){
            if(!solution_[(i*boardWidth_) + j].getValuePossibility(value) && solution_[(i*boardWidth_) + j].getValue()==0){
                if(++val_counter>1) return nullptr;
                row = i;
                col = j;
            }
        }
    }
    return new Coordinates(row,col);
}

// Brute force algorithm for puzzle solving
void SudokuSolver::searchAlgorithm(){
    int remain=0;
    for(int i=0; i< (boardWidth_*boardHeight_); i++){
        if( solution_[i].getValue()==0) remain++;
    }
    if( remain==0) return;
    
    /*std::array<Cell, 81> arr;
    for(int i=0; i<9; i++){
        for(int j=0; j<9; j++){
            arr[ (9*i) + j] = board_[i][j];
        }
    }*/
    searchRecursive( solution_ , remain, true , Coordinates(0,0));
}

// Algorithms recursive
bool SudokuSolver::searchRecursive( std::vector<Cell> curr_board , int remain, bool first, Coordinates coords){
    // if remain 0 == completed, copy content to solution array
    if(remain==0){
        for(int i=0; i < (boardWidth_*boardHeight_) ; i++){
            solution_[i] = curr_board[i];
        }
        return true;
    }
    // if not the first insertion updates the board on chosen fixed cell value
    if(!first){
        int value = curr_board[(9*coords.getRow())+coords.getCol()].getValue();
        
        //std::cout << "Updating (" << coords.getRow() << "," << coords.getCol() << ") with " << value << " ... missing" << remain << std::endl;
        
        for(int i=0; i< boardWidth_; i++){
            if( curr_board[(9*coords.getRow())+i].getValue()==0){
                curr_board[(9*coords.getRow())+i].setValuePossibility(value, true);
            }
            if( curr_board[(9*coords.getRow())+i].getNumberPossibilities()==0) return false;
        }
        
        for(int i=0; i< boardHeight_; i++){
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
    
    // After updating , find cell with least number of possible values
    std::size_t min = 9;
    Coordinates coords_new;
    for(int i=0; i< boardWidth_; i++){
        for (int j=0; j< boardHeight_; j++) {
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
    
    // Try every single value of chosen cell
    int row =coords_new.getRow();
    int col =coords_new.getCol();
    std::bitset<9> cellPoss = curr_board[(row*9)+col].getPossibleValues();
    
    for(int i=0; i< 9; i++){
        if( !cellPoss[i]){
            curr_board[(row*9)+col].setValue(i+1);
            if(searchRecursive(curr_board, remain-1, false, coords_new)) return true;
        }
    }
    return false;
}

