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

#include "GameScreen.hpp"


int main(int argc, const char * argv[]) {
    
    // Create Game Screen
    GameScreen gameScreen (SCREEN_WIDTH, SCREEN_HEIGHT);
    
    if( gameScreen.init() ){
        gameScreen.start();
    }
    
    return 0;
}
