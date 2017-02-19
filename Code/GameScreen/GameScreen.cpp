//
//  GameScreen.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 18/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "GameScreen.hpp"

GameScreen::GameScreen(){}

GameScreen::GameScreen(int windowWidth, int windowHeight)
{
    this->windowWidth_ = windowWidth;
    this->windowHeight_ = windowHeight;
}

void GameScreen::start()
{
    if( !hasInitiated) return;
    
    // User wants to Quit
    bool quit = false;
    
    // Required to update window
    bool update = false;
    
    // Event Handler
    SDL_Event e;
    
    while( !quit){
        while( SDL_PollEvent( &e) != 0 ){
            if( e.type == SDL_QUIT){
                quit = true;
            }
            else{
                // Forward events to components and store answers (Handlers) onto queue
                handlerQueue_.push(sudoku_->handleEvent(&e));
                for( Button* button: listButtons) handlerQueue_.push(button->handleEvent(&e));
            }
        }
        // Process queue of handlers
        update = processHandlers();
        
        if( update){
            // Clear window
            window_->clearScreen();
            
            // Render sudoky and Buttons
            sudoku_->render();
            for( Button* button: listButtons) button->render();
            
            // Update Screen
            window_->updateScreen();
        }
    }

}


// Initializes all Screen components (window,buttons,sudoku)
bool GameScreen::init()
{
    bool success=true;
    
    // Initiate window (sdl, window, renderer, png, ttf)
    window_ = new Window(windowWidth_, windowHeight_, "My fun project!");
    if( !window_->init()){
        std::cerr << "Failed to initiate window\n";
        success = false;
    }
    
    // Load font
    if( !window_->loadFont("Images/font2.ttf")){
        std::cerr << "Failed to load window\n";
        success = false;
    }
    // Get Screen Variables
    renderer_ = window_->getRenderer();
    font_ = window_->getFont();
    
    //Initiate Buttons
    if(!loadButtons()){
        std::cerr << "Failed to load Buttons\n";
        success = false;
    }

    sudoku_ = new Sudoku( renderer_, font_, windowWidth_,windowHeight_);

    if(!sudoku_->buildFromFile( std::string("Puzzles/Hard/hard3.txt"))){
        std::cerr << "Failed to load Puzzle\n";
        success = false;
    }
    
    if(!sudoku_->solveSudoku()){
        std::cerr << "Failed to solve puzzle\n";
        success = false;
    }

    if( success) hasInitiated=true;

    return success;
}


// Load all buttons
bool GameScreen::loadButtons(){
    
    RectButton* newButton;
    
    newButton = new RectButton(renderer_ , font_, 520, 100, 100,50);
    newButton->setText("Verify");
    listButtons.push_back(newButton);
    
    return true;
}


// Process all handlers
bool GameScreen::processHandlers()
{
    bool success = true;
    
    while( !handlerQueue_.empty()){
        Handler handler = handlerQueue_.front();
        handlerQueue_.pop();
    
        switch (handler.getEvent())
        {
            case Handler::EVENT_VERIFY:
                break;
                
            case Handler::EVENT_INPUT:
                break;
                
            case Handler::EVENT_IGNORE:
                break;

        }
    }
    
    return success;
}








