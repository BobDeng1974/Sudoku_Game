//
//  GameScreen.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 18/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "GameScreen.hpp"

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
                if(isPlaying_){
                    // Forward events to components and store answers (Handlers) onto queue
                    handlerQueue_.push(sudoku_->handleEvent(&e));
                    for( Button* button: listButtons) handlerQueue_.push(button->handleEvent(&e));
                }
                else{
                    for( Button* button: difficultyPickerButtons_) handlerQueue_.push(button->handleEvent(&e));
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
        }
        
        pickingText_ = new Texture(renderer_,font_->getFont(24));
        pickingText_->loadFromRenderedText("Choose the difficulty:", textColor_);
        
        sudoku_ = new Sudoku( renderer_, font_->getFont(28), windowWidth_,windowHeight_);
        hasInitiated=true;
    }

    return success;
}


// Load all buttons
bool GameScreen::loadButtons(){
    
    RectButton* newButton;
    
    int buttonx = (3*windowWidth_)/4 + ( ( windowWidth_/4)- buttonWidth_)/2;
    int buttony = (windowHeight_/5)/5;
    // Verify Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, buttony, buttonWidth_, buttonHeight_);
    newButton->setText("Verify");
    newButton->setCallbackEvent(Handler::EVENT_VERIFY);
    listButtons.push_back(newButton);
    
    // Reset Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, (2*buttony) + buttonHeight_, buttonWidth_, buttonHeight_);
    newButton->setText("Reset");
    newButton->setCallbackEvent(Handler::EVENT_RESET);
    listButtons.push_back(newButton);
    
    // New Game Button
    newButton = new RectButton(renderer_ , font_->getFont(12), buttonx, (3*buttony) + (2*buttonHeight_), buttonWidth_, buttonHeight_);
    newButton->setText("New Game");
    newButton->setCallbackEvent(Handler::EVENT_NEWGAME);
    listButtons.push_back(newButton);
    
    // Hint Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, (4*buttony) + (3*buttonHeight_), buttonWidth_, buttonHeight_);
    newButton->setText("Hint");
    newButton->setCallbackEvent(Handler::EVENT_HINT);
    listButtons.push_back(newButton);

    // Easy Mode Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, (4*buttony) + (3*buttonHeight_), buttonWidth_, buttonHeight_);
    newButton->setText("Easy");
    newButton->setCallbackEvent(Handler::EVENT_EASY);
    listButtons.push_back(newButton);
    
    // Load Difficulty Selection Buttons
    int diff_x = (windowWidth_*3/4)/2 - (buttonWidth_);
    int diff_y = ( windowHeight_ - (buttonHeight_*3/2)*6 )/2;
    // Easy Button
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2), buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_EASY);
    newButton->setText("Easy");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Medium Button
    newButton = new SelectorButton(renderer_ , font_->getFont(24), diff_x, diff_y + (buttonHeight_*3/2)*2, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_MEDIUM);
    newButton->setText("Medium");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Hard Button
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2)*3, buttonWidth_*2, buttonHeight_ , Difficulty::DIFFICULTY_HARD);
    newButton->setText("Hard");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Very Hard Button
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2)*4, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_VERYHEARD);
    newButton->setText("Very Hard");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Custom Button
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2)*5, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_CUSTOM);
    newButton->setText("Custom");
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);

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
                isVerify_ = !isVerify_;
                success = true;
                break;
                
            case Handler::EVENT_INPUT:
                if( sudoku_->getNoEmptyBlock()==0 && sudoku_->isAllCorrect()){
                    sudoku_->blockBoard();
                    isFinished=true;
                }
                success = true;
                break;
                
            case Handler::EVENT_RESET:
                sudoku_->reset();
                sudoku_->buildFromFile(filename_);
                for( Button* button: listButtons) button->reset();
                hintNo = MAX_HINTS;
                isFinished=false;
                break;
                
            case Handler::EVENT_NEWGAME:
                isPlaying_ = false;
                break;
                
            case Handler::EVENT_PICKER:
                if( !isPlaying_){
                    success = loadPickedPuzzle( handler.getIntExtra());
                    isFinished=false;
                }
                break;
                
            case Handler::EVENT_HINT:
                if(hintNo > 0){
                    sudoku_->showAndBlockCell();
                    success=true;
                    hintNo--;
                }
                break;
                
            case Handler::EVENT_EASY:
                
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
    if( isPlaying_)
        sudoku_->render(isVerify_);  // Render sudoku if playing
    else{
        pickingText_->render((windowWidth_*3/4)/2 - (pickingText_->getWidth()/2), ( windowHeight_ - (buttonHeight_*3/2)*6 )/2);
        for( Button* button: difficultyPickerButtons_) button->render(); // else render the picking buttons
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
            filename_ ="Puzzles/Easy/sudoku1.txt";
            sudoku_->reset();
            sudoku_->buildFromFile(filename_);
            sudoku_->solveSudoku();
            isPlaying_ = true;
            success = true;
            break;
            
        case DIFFICULTY_MEDIUM:
            filename_= "Puzzles/Medium/Medium1.txt";
            sudoku_->reset();
            sudoku_->buildFromFile(filename_);
            sudoku_->solveSudoku();
            isPlaying_ = true;
            success = true;
            break;
            
        case DIFFICULTY_HARD:
            filename_ = "Puzzles/Hard/hard1.txt";
            sudoku_->reset();
            sudoku_->buildFromFile(filename_);
            sudoku_->solveSudoku();
            isPlaying_ = true;
            success = true;
            break;
            
        case DIFFICULTY_VERYHEARD:
            filename_ = "Puzzles/Hard/hard2.txt";
            sudoku_->reset();
            sudoku_->buildFromFile(filename_);
            sudoku_->solveSudoku();
            isPlaying_ = true;
            success = true;
            break;
            
        case DIFFICULTY_CUSTOM:
            break;
    }
    
    return success;
}




