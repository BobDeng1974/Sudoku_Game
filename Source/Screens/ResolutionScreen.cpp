//
//  ResolutionScreen.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 21/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "ResolutionScreen.hpp"

ResolutionScreen::ResolutionScreen(int windowWidth, int windowHeight)
{
    this->windowWidth_ = windowWidth;
    this->windowHeight_ = windowHeight;
}

ResolutionScreen::~ResolutionScreen()
{
    //Free global font
    delete font_;
    
    // Free window  (deletes renderer)
    delete window_;
    renderer_=nullptr;
    window_=nullptr;
}
// Screen's main function
bool ResolutionScreen::start()
{
    bool success = false;
    // Initialize components
    if( !hasInitiated) return success;
    render();
    
    // User wants to Quit
    bool quit = false;
    
    // Required to update window
    bool update = true;
    
    // Event Handler
    SDL_Event e;
    
    while( !quit){
        while( SDL_PollEvent( &e) != 0 ){
            if( e.type == SDL_QUIT){
                quit = true;
            }
            else{
                for( Button* button: listButtons) handlerQueue_.push(button->handleEvent(&e));
            }
        }
        // Process queue of handlers
        update = processHandlers();
        if( update){
            quit = true;
            success = true;
        }
    }
    return success;
}


// Initializes all Screen components (window,buttons,sudoku)
bool ResolutionScreen::init()
{
    bool success=true;
    
    // Initiate window (sdl, window, renderer, png, ttf)
    window_ = new Window(windowWidth_, windowHeight_, "My fun project!");
    if( !window_->init()){
        std::cerr << "Failed to initiate window\n";
        success = false;
    }
    
    // Get Screen Variables
    renderer_ = window_->getRenderer();
    
    // Create font class
    font_ = new Font("Images/font2.ttf");
    if( !font_->createSizedFont(28)){
        std::cerr << " Failed to load font\n";
        success = false;
    }
    font_->createSizedFont(24);
    font_->createSizedFont(20);
    font_->createSizedFont(16);
    font_->createSizedFont(12);
    
    
    if( success){
        //Initiate Buttons
        if(!loadButtons()){
            std::cerr << "Failed to load Buttons\n";
            success = false;
        }else{
            hasInitiated=true;
        }
    }
    
    return success;
}

// Load all buttons
bool ResolutionScreen::loadButtons(){
    
    RectButton* newButton;
    int buttonWidth = (windowWidth_/4);
    int buttonHeight = (windowHeight_/2);
    
    int margin = windowWidth_/16;
    
    // Verify Button
    newButton = new SelectorButton(renderer_ , font_->getFont(20), margin, buttonHeight/2, buttonWidth, buttonHeight,1);
    newButton->setText("640x480");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    listButtons.push_back(newButton);
    
    // Reset Button
    newButton = new SelectorButton(renderer_ , font_->getFont(20), ((2*margin) + buttonWidth), buttonHeight/2, buttonWidth, buttonHeight ,2 );
    newButton->setText("800x600");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    listButtons.push_back(newButton);
    
    // New Game Button
    newButton = new SelectorButton(renderer_ , font_->getFont(20), ((3*margin) + (buttonWidth*2)), buttonHeight/2, buttonWidth, buttonHeight, 3);
    newButton->setText("960x720");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    listButtons.push_back(newButton);
    
    return true;
}


// Process all handlers
bool ResolutionScreen::processHandlers()
{
    bool success = false;
    
    while( !handlerQueue_.empty()){
        Handler handler = handlerQueue_.front();
        handlerQueue_.pop();
        
        switch (handler.getEvent())
        {
            case Handler::EVENT_PICKER: // Save user's selection
                switch(handler.getIntExtra()){
                    case 1:
                        chosenWidth_=640;
                        chosenHeight_ = 480;
                        break;
                    case 2:
                        chosenWidth_=800;
                        chosenHeight_ = 600;
                        break;
                    case 3:
                        chosenWidth_=960;
                        chosenHeight_ = 720;
                        break;
                }
                success = true;
                break;
                
            default:
                break;
        }
    }
    return success;
}

// Render screen
void ResolutionScreen::render()
{
    // Clear window
    window_->clearScreen();
    
    // Render buttons
    for( Button* button: listButtons) button->render(); // else render the picking buttons
    
    // Update Screen
    window_->updateScreen();
}

// Getter for user selection
void ResolutionScreen::getResolution(int &width, int &height) const
{
    width = chosenWidth_;
    height = chosenHeight_;
}

