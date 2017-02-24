//
//  main.cpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

//Screen dimension constants
const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 100;

#include "GameScreen.hpp"
#include "ResolutionScreen.hpp"


int main(int argc, const char * argv[]) {
    
    bool success = false;
    if(SDL_Init(SDL_INIT_VIDEO) <0){
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else{
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            std::cerr << "Warning: Linear texture filtering not enabled!\n";
        }
    }
    
    
    //Create Resolution Screen
    ResolutionScreen* resScreen = new ResolutionScreen(SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if( resScreen->init() ){
        success = resScreen->start();
    }
    
    if( success){
        int width;
        int height;
        resScreen->getResolution( width, height);
        delete resScreen;
        
        // Create Game Screen
        GameScreen gameScreen (width, height);
        
        if( gameScreen.init() ){
            gameScreen.start();
        }

    }
    
    SDL_Quit();

    
    return 0;
}
