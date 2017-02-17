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


SDL_Texture* gTexture = nullptr;

SDL_Renderer* gRenderer=nullptr;

SDL_Texture* loadTexture( std::string path)
{
    //The final optimized image
    SDL_Texture* newTexture = NULL;
    
    // Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if( loadedSurface == nullptr){
        std::cerr << "Unable to load image " << path << " ! SDL Error: " << SDL_GetError() << std::endl;
    }
    else{
        // Convert Surface to Screen Format
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if( newTexture == nullptr){
            std::cerr << "Unable to create texture from " << path << " ! SDL Error: " <<SDL_GetError() << std::endl;
        }
        // Free initial surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}


bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load PNG texture
    gTexture = loadTexture( "Images/texture.png" );
    if( gTexture == NULL )
    {
        printf( "Failed to load texture image!\n" );
        success = false;
    }
    
    return success;
}

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



int main(int argc, const char * argv[]) {
   
    Window gwindow(SCREEN_WIDTH, SCREEN_HEIGHT, "My fun project!");
    
    gwindow.initTTF();
    gwindow.initPNG();
    gwindow.loadFont("Images/font2.ttf");
    gRenderer = gwindow.getRenderer();
    
    std::vector<Button*> listButtons;
    
    listButtons.push_back( new RectButton(gRenderer ,520, 100, 100,50));
    
    
    
    
    Sudoku game( gRenderer, gwindow.getFont(), SCREEN_HEIGHT,SCREEN_HEIGHT);
    
    buildFromFile( game, std::string("Puzzles/Hard/hard3.txt"));
    game.solveSudoku();
    //game.displaySolution();
    if( !loadMedia()){
        std::cout << "Failed to load media!\n";
    }else{
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
            for( Button* button: listButtons) button->handleEvent(&e);

            
            gwindow.updateScreen();
                    
        }
    }

    
    
    
    return 0;
}
