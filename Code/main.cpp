//
//  main.cpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


#include <iostream>
#include <sstream>
#include <fstream>


#include "Window.hpp"
#include "Sudoku.hpp"

#include "Buttons/RectButton.hpp"


bool buildFromFile( Sudoku &sudoku, std::string filename)
{
    std::ifstream f_input;
    f_input.open(filename);
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
        
        if(!sudoku.insertCellValue(row - '1',  col - 'A', value - '0')){
            return false;
        }
    }
    f_input.close();
    return true;
}


std::vector<Button*> loadButtons(SDL_Renderer* renderer, TTF_Font* font){
    std::vector<Button*> vector;
    RectButton* newButton;
    
    newButton = new RectButton(renderer , font, 520, 100, 100,50);
    newButton->setText("Verify");
    vector.push_back(newButton);
    
    return vector;
}



int main(int argc, const char * argv[]) {
    
    Window gwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My fun project!");
    
    gwindow.initTTF();
    gwindow.initPNG();
    gwindow.loadFont("Images/font2.ttf");
    SDL_Renderer* gRenderer = gwindow.getRenderer();
    
    std::vector<Button*> listButtons(loadButtons(gRenderer, gwindow.getFont()));
    
    
    
    Sudoku game( gRenderer, gwindow.getFont(), SCREEN_HEIGHT,SCREEN_HEIGHT);
    
    buildFromFile( game, std::string("Puzzles/Hard/hard3.txt"));
    game.solveSudoku();
    
    // User wants to Quit
    bool quit = false;
    
    // Event Handler
    SDL_Event e;
    
    while( !quit){
        while( SDL_PollEvent( &e) != 0 ){
            if( e.type == SDL_QUIT){
                quit = true;
            }
            else{
                game.handleEvent(&e);
                for( Button* button: listButtons) button->handleEvent(&e);
            }
        }
        
        
        gwindow.clearScreen();
        
        game.render();
        for( Button* button: listButtons) button->render();
        
        
        gwindow.updateScreen();
        
    }
    
    return 0;
}
