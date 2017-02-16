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
    
    
    Sudoku game( gRenderer, gwindow.getFont(), SCREEN_HEIGHT,SCREEN_HEIGHT);
    
    buildFromFile( game, std::string("Puzzles/Hard/hard3.txt"));
    game.solveSudoku();
    game.displaySolution();
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
                }
            }
            
            
            gwindow.clearScreen();
            
            game.render();
            
            
            /*
             //Render red filled quad
             SDL_Rect fillRect = { SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
             SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0x00, 0xFF );
             SDL_RenderFillRect( gRenderer, &fillRect );
             
             //Render green outlined quad
             SDL_Rect outlineRect = { SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3 };
             SDL_SetRenderDrawColor( gRenderer, 0x00, 0xFF, 0x00, 0xFF );
             SDL_RenderDrawRect( gRenderer, &outlineRect );
             
             
             //Draw vertical line of yellow dots
             SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0x00, 0xFF );
             for( int i = 0; i < SCREEN_HEIGHT; i += 4 )
             {
             SDL_RenderDrawPoint( gRenderer, SCREEN_WIDTH / 2, i );
             }
           
            
            
            // left corner viewport
            SDL_Rect topLeftViewport;
            topLeftViewport.x = 0;
            topLeftViewport.y = 0;
            topLeftViewport.w = (SCREEN_WIDTH/4)*3;
            topLeftViewport.h = SCREEN_HEIGHT;
            SDL_RenderSetViewport( gRenderer, &topLeftViewport );
            
            
            //Draw blue horizontal line
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
            
            // Outer Borders
            
            SDL_Rect fillRect = { 13, 13, SCREEN_HEIGHT -(2*13), 4 };
            SDL_RenderFillRect( gRenderer, &fillRect );

            for(int i =0; i<11; i++){
                SDL_RenderDrawLine( gRenderer, 15 + (i*50), 15, 15 + (i*50),SCREEN_HEIGHT-15);
            }
            
            for(int i =0; i<11; i++){
                SDL_RenderDrawLine( gRenderer, 15, 15 + (i*50), SCREEN_HEIGHT-15, 15 + (i*50));
            }
            
            
            //Right viewport
            SDL_Rect bottomViewport;
            bottomViewport.x = (SCREEN_WIDTH/4)*3;
            bottomViewport.y = 0;
            bottomViewport.w = SCREEN_WIDTH/4;
            bottomViewport.h = SCREEN_HEIGHT;
            SDL_RenderSetViewport( gRenderer, &bottomViewport );
            
            //Render texture to screen
            SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );*/
            
            
            //Update screen
            SDL_RenderPresent( gRenderer );
            
        }
    }

    
    
    
    return 0;
}
