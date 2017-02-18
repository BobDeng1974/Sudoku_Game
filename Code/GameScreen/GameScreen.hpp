//
//  GameScreen.hpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 18/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef GameScreen_hpp
#define GameScreen_hpp

#include <iostream>
#include <sstream>
#include <fstream>


#include "Window.hpp"
#include "Sudoku.hpp"
#include "RectButton.hpp"

class GameScreen
{
    public:
        GameScreen();
        GameScreen(int windowWidth, int windowHeight);

        // Initialize Screen
        bool init();

        // Start Screen Functions
        void start();
    
    
    
    
    protected:
        // Load Screen Buttons
        virtual bool loadButtons();
    
    
    private:
        // Screen Size
        int windowWidth_;
        int windowHeight_;
    
        // Window variable
        Window* window_=nullptr;
    
        // Screen Renderer
        SDL_Renderer* renderer_ = nullptr;
    
        //TTF font
        TTF_Font* font_;
    
        // List of Buttons
        std::vector<Button*> listButtons;
    
        //Sudoku game
        Sudoku* sudoku_ = nullptr;
    
        // Bool indicates if initiated correctly
        bool hasInitiated=false;
};

#endif /* GameScreen_hpp */
