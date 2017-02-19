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
                if(isPlaying){
                    // Forward events to components and store answers (Handlers) onto queue
                    handlerQueue_.push(sudoku_->handleEvent(&e));
                    for( Button* button: listButtons) handlerQueue_.push(button->handleEvent(&e));
                }
                else{
                    for( Button* button: difficultyPickerButtons) handlerQueue_.push(button->handleEvent(&e));
                }
            }
        }
        // Process queue of handlers
        update = processHandlers();
        
        if( update){
            render();
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

    pickingText = new Texture(renderer_,font_);
    pickingText->loadFromRenderedText("Choose the difficulty:", textColor);
    
    sudoku_ = new Sudoku( renderer_, font_, windowWidth_,windowHeight_);

   /* if(!sudoku_->buildFromFile( filename_)){
        std::cerr << "Failed to load Puzzle\n";
        success = false;
    }
    
    if(!sudoku_->solveSudoku()){
        std::cerr << "Failed to solve puzzle\n";
        success = false;
    }*/

    if( success) hasInitiated=true;

    return success;
}


// Load all buttons
bool GameScreen::loadButtons(){
    
    RectButton* newButton;
    
    // Verify Button
    newButton = new RectButton(renderer_ , font_, 520, 100, buttonWidth_, buttonHeight_);
    newButton->setText("Verify");
    newButton->setCallbackEvent(Handler::EVENT_VERIFY);
    listButtons.push_back(newButton);
    
    // Reset Button
    newButton = new RectButton(renderer_ , font_, 520, 200, buttonWidth_, buttonHeight_);
    newButton->setText("Reset");
    newButton->setCallbackEvent(Handler::EVENT_RESET);
    listButtons.push_back(newButton);
    
    // Reset Button
    newButton = new RectButton(renderer_ , font_, 520, 200, buttonWidth_, buttonHeight_);
    newButton->setText("Reset");
    newButton->setCallbackEvent(Handler::EVENT_RESET);
    listButtons.push_back(newButton);

    
    
    int diff_x = (windowWidth_*3/4)/2 - (buttonWidth_);
    int diff_y = ( windowHeight_ - (buttonHeight_*3/2)*6 )/2;
    // Easy Button
    newButton = new SelectorButton(renderer_ , font_,  diff_x, diff_y + (buttonHeight_*3/2), buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_EASY);
    newButton->setText("Easy");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons.push_back(newButton);
    
    // Medium Button
    newButton = new SelectorButton(renderer_ , font_, diff_x, diff_y + (buttonHeight_*3/2)*2, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_MEDIUM);
    newButton->setText("Medium");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons.push_back(newButton);
    
    // Hard Button
    newButton = new SelectorButton(renderer_ , font_,  diff_x, diff_y + (buttonHeight_*3/2)*3, buttonWidth_*2, buttonHeight_ , Difficulty::DIFFICULTY_HARD);
    newButton->setText("Hard");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons.push_back(newButton);
    
    // Very Hard Button
    newButton = new SelectorButton(renderer_ , font_,  diff_x, diff_y + (buttonHeight_*3/2)*4, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_VERYHEARD);
    newButton->setText("Very Hard");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons.push_back(newButton);
    
    // Custom Button
    newButton = new SelectorButton(renderer_ , font_,  diff_x, diff_y + (buttonHeight_*3/2)*5, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_CUSTOM);
    newButton->setText("Custom");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons.push_back(newButton);

    return true;
}


// Process all handlers
bool GameScreen::processHandlers()
{
    bool success = false;
    
    while( !handlerQueue_.empty()){
        Handler handler = handlerQueue_.front();
        handlerQueue_.pop();
    
        switch (handler.getEvent())
        {
            case Handler::EVENT_VERIFY:
                isVerify = !isVerify;
                success = true;
                break;
                
            case Handler::EVENT_INPUT:
                success = true;
                break;
                
            case Handler::EVENT_RESET:
                sudoku_->reset();
                sudoku_->buildFromFile(filename_);
                for( Button* button: listButtons) button->reset();
                break;
                
            case Handler::EVENT_PICKER:
                if( !isPlaying){
                    success = loadPickedPuzzle( handler.getIntExtra());
                }
                break;
                
            case Handler::EVENT_IGNORE:
                break;

        }
    }
    return success;
}


void GameScreen::render()
{
    // Clear window
    window_->clearScreen();
    
    // Render Elements
    if( isPlaying)
        sudoku_->render(isVerify);  // Render sudoku if playing
    else{
        pickingText->render((windowWidth_*3/4)/2 - (pickingText->getWidth()/2), ( windowHeight_ - (buttonHeight_*3/2)*6 )/2);
        for( Button* button: difficultyPickerButtons) button->render(); // else render the picking buttons
    }
    
    // Render options buttons
    for( Button* button: listButtons) button->render();
    
    // Update Screen
    window_->updateScreen();
}

bool GameScreen::loadPickedPuzzle( int difficulty)
{
    // Return boolean
    bool success = false;
    
    // TODO Refactor into function
    // TODO Implement Custom
    
    
    switch (difficulty) {
        case DIFFICULTY_EASY:
            sudoku_->reset();
            sudoku_->buildFromFile("Puzzles/Easy/sudoku1.txt");
            sudoku_->solveSudoku();
            isPlaying = true;
            success = true;
            break;
            
        case DIFFICULTY_MEDIUM:
            sudoku_->reset();
            sudoku_->buildFromFile("Puzzles/Medium/Medium1.txt");
            sudoku_->solveSudoku();
            isPlaying = true;
            success = true;
            break;
            
        case DIFFICULTY_HARD:
            sudoku_->reset();
            sudoku_->buildFromFile("Puzzles/Hard/hard1.txt");
            sudoku_->solveSudoku();
            isPlaying = true;
            success = true;
            break;
            
        case DIFFICULTY_VERYHEARD:
            sudoku_->reset();
            sudoku_->buildFromFile("Puzzles/Hard/hard2.txt");
            sudoku_->solveSudoku();
            isPlaying = true;
            success = true;
            break;
            
        case DIFFICULTY_CUSTOM:
            break;
    }
    
    return success;
}




