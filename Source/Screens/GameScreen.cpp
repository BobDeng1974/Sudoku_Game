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

// Screen main function
void GameScreen::start()
{
    // Initialize elements
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
                    // Is selecting difficulty
                    for( Button* button: difficultyPickerButtons_) handlerQueue_.push(button->handleEvent(&e));
                }
            }
        }
        // Process queue of handlers and see if required update window
        update = processHandlers();
        if( update){
            render();
        }
    }
    saveFile_->writeFile();
}


// Initializes all Screen components (window,buttons,sudoku)
bool GameScreen::init()
{
    bool success=true;
    
    // Initiate window (sdl, window, renderer, png, ttf)
    window_ = new Window(windowWidth_, windowHeight_, "Sudoku");
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
    // Creates different sized fonts
    font_->createSizedFont(24);
    font_->createSizedFont(20);
    font_->createSizedFont(16);
    font_->createSizedFont(12);

    // Loads save file
    saveFile_ = new SaveFile("Saved/saved.bin");
    if(!saveFile_->readFile()) success = false;
    
    if( success){
        //Initiate Buttons
        if(!loadButtons()){
            std::cerr << "Failed to load Buttons\n";
            success = false;
        }
        // Create text
        pickingText_ = new Texture(renderer_,font_->getFont(24));
        pickingText_->loadFromRenderedText("Choose the difficulty:", textColor_);
        
        // Create sudoku
        sudoku_ = new Sudoku( renderer_, font_->getFont(28), windowWidth_,windowHeight_);
        hasInitiated=true;
    }

    return success;
}


// Load all buttons
bool GameScreen::loadButtons(){
    
    RectButton* newButton;
    std::string buttonName;
    
    // Load Difficulty Selection Buttons
    int diff_x = (windowWidth_*3/4)/2 - (buttonWidth_);
    int diff_y = ( windowHeight_ - (buttonHeight_*3/2)*6 )/2;
    int value;

    // Easy Button
    value = (saveFile_->getIndexValue(0)>4)? 4: saveFile_->getIndexValue(0);
    buttonName = "Easy (";
    buttonName.append( std::to_string(value));
    buttonName.append("/4)");
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2), buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_EASY);
    newButton->setText(buttonName);
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Medium Button
    value = (saveFile_->getIndexValue(1)>4)? 4: saveFile_->getIndexValue(1);
    buttonName = "Medium (";
    buttonName.append( std::to_string(value));
    buttonName.append("/4)");
    newButton = new SelectorButton(renderer_ , font_->getFont(24), diff_x, diff_y + (buttonHeight_*3/2)*2, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_MEDIUM);
    newButton->setText(buttonName);
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Hard Button
    value = (saveFile_->getIndexValue(2)>4)? 4: saveFile_->getIndexValue(2);
    buttonName = "Hard (";
    buttonName.append( std::to_string(value));
    buttonName.append("/4)");
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2)*3, buttonWidth_*2, buttonHeight_ , Difficulty::DIFFICULTY_HARD);
    newButton->setText(buttonName);
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Very Hard Button
    value = (saveFile_->getIndexValue(3)>4)? 4: saveFile_->getIndexValue(3);
    buttonName = "Very Hard (";
    buttonName.append( std::to_string(value));
    buttonName.append("/4)");
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2)*4, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_VERYHARD);
    newButton->setText(buttonName);
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);
    
    // Custom Button
    buttonName = "Custom (";
    buttonName.append( std::to_string(saveFile_->getIndexValue(4)));
    buttonName.append(")");
    newButton = new SelectorButton(renderer_ , font_->getFont(24),  diff_x, diff_y + (buttonHeight_*3/2)*5, buttonWidth_*2, buttonHeight_, Difficulty::DIFFICULTY_CUSTOM);
    newButton->setText(buttonName);
    newButton->setCallbackEvent(Handler::EVENT_PICKER);
    difficultyPickerButtons_.push_back(newButton);

    
    int buttonx = (3*windowWidth_)/4 + ( ( windowWidth_/4)- buttonWidth_)/2;
    int buttony = (windowHeight_/6)/2 - buttonHeight_/2;
    // Verify Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, buttony, buttonWidth_, buttonHeight_);
    newButton->setText("Verify");
    newButton->setCallbackEvent(Handler::EVENT_VERIFY);
    newButton->setToggle(true);
    listButtons.push_back(newButton);
    
    // Reset Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, (3*buttony) + (buttonHeight_), buttonWidth_, buttonHeight_);
    newButton->setText("Reset");
    newButton->setCallbackEvent(Handler::EVENT_RESET);
    listButtons.push_back(newButton);
    
    // New Game Button
    newButton = new RectButton(renderer_ , font_->getFont(12), buttonx, (5*buttony) + (2*buttonHeight_), buttonWidth_, buttonHeight_);
    newButton->setText("New Game");
    newButton->setCallbackEvent(Handler::EVENT_NEWGAME);
    listButtons.push_back(newButton);
    
    // Hint Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, (7*buttony + 3*buttonHeight_) , buttonWidth_, buttonHeight_);
    newButton->setText("Hint");
    newButton->setCallbackEvent(Handler::EVENT_HINT);
    listButtons.push_back(newButton);
    
    // Easy Mode Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, (9*buttony + 4*buttonHeight_), buttonWidth_, buttonHeight_);
    newButton->setText("Easy");
    newButton->setCallbackEvent(Handler::EVENT_EASY);
    newButton->setToggle(true);
    listButtons.push_back(newButton);
    
    // Done Mode Button
    newButton = new RectButton(renderer_ , font_->getFont(16), buttonx, (11*buttony + 5*buttonHeight_), buttonWidth_, buttonHeight_);
    newButton->setText("Done");
    newButton->setCallbackEvent(Handler::EVENT_CUSTOM);
    newButton->setVisibility(false);
    newButton->setEnabled(false);
    doneButton = newButton;
    listButtons.push_back(newButton);
    
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
            case Handler::EVENT_VERIFY: //set toggle verify mode
                isVerify_ = !isVerify_;
                success = true;
                break;
                
            case Handler::EVENT_INPUT: // Set for update screen and verify is finished
                if( sudoku_->getNoEmptyBlock()==0 && sudoku_->isAllCorrect() && !isFinished){
                    sudoku_->blockBoard();
                    isFinished=true;
                    saveFile_->incrementIndex(currentDifficulty);
                    updateButtonText(currentDifficulty);
                }
                success = true;
                break;
                
            case Handler::EVENT_RESET: // resets board
                sudoku_->reset(false);
                //sudoku_->buildFromFile(filename_);
                for( Button* button: listButtons) button->reset();
                hintNo = MAX_HINTS;
                isFinished=false;
                break;
                
            case Handler::EVENT_NEWGAME: // opens difficulty selection menu
                sudoku_->reset(true);
                isPlaying_ = false;
                listButtons[0]->setEnabled(true);
                listButtons[3]->setEnabled(true);
                listButtons[4]->setEnabled(true);
                break;
                
            case Handler::EVENT_PICKER: // User has picked difficulty
                if( !isPlaying_){
                    success = handleDifficulty( handler.getIntExtra());
                    isFinished=false;
                }
                break;
                
            case Handler::EVENT_HINT: // User asked for hiny
                if(hintNo > 0){
                    sudoku_->showAndBlockCell();
                    success=true;
                    hintNo--;
                    if(sudoku_->getNoEmptyBlock()==0 && sudoku_->isAllCorrect()){
                        sudoku_->blockBoard();
                        isFinished=true;
                        saveFile_->incrementIndex(currentDifficulty);
                        updateButtonText(currentDifficulty);
                    }
                }
                break;
                
            case Handler::EVENT_EASY: // Toggle easymode
                if( !sudoku_->isCustomMode()) sudoku_->setEasyMode( !sudoku_->isEasyMode());
                success = true;
                break;
                
            case Handler::EVENT_CUSTOM: // User has finished creating board
                
                if(!sudoku_->solveSudoku()){
                    std::cerr << "impossible Sudoku configuration!\n";
                    sudoku_->reset( true);
                }
                else{
                    sudoku_->setEasyMode(false);
                    sudoku_->setCustomMode(false);
                    isPlaying_=true;
                    success = true;
                    sudoku_->setInitialStateBoard();
                    doneButton->setEnabled(false);
                    doneButton->setVisibility(false);
                }
                listButtons[0]->setEnabled(true);
                listButtons[3]->setEnabled(true);
                listButtons[4]->setEnabled(true);
                break;
                
            case Handler::EVENT_RENDER:
                success = true;
                break;
                
            case Handler::EVENT_IGNORE:
                break;
        }
    }
    return success;
}

// Render screen
void GameScreen::render()
{
    // Clear window
    window_->clearScreen();
    
    // Render Elements
    if( isPlaying_)
        sudoku_->render(isVerify_);  // Render sudoku if playing
    else{
        pickingText_->render((windowWidth_*3/4)/2 - (pickingText_->getWidth()/2), ( windowHeight_ - (buttonHeight_*3/2)*6 )/2);
        for( Button* button: difficultyPickerButtons_) button->render();
        for( Button* button: listButtons) button->render();
    }
    
    // Render options buttons
    for( Button* button: listButtons) button->render();
    
    // Update Screen
    window_->updateScreen();
}

// Difficulty selected
bool GameScreen::handleDifficulty( int difficulty)
{
    // Return boolean
    bool success = false;
    
    switch (difficulty) {
        case DIFFICULTY_EASY:
            loadPuzzle( DIFFICULTY_EASY, "Puzzles/Easy/easy");
            currentDifficulty = DIFFICULTY_EASY;
            success = true;
            break;
            
        case DIFFICULTY_MEDIUM:
            loadPuzzle(DIFFICULTY_MEDIUM, "Puzzles/Medium/medium");
            currentDifficulty = DIFFICULTY_MEDIUM;
            success = true;
            break;
            
        case DIFFICULTY_HARD:
            loadPuzzle(DIFFICULTY_HARD, "Puzzles/Hard/hard");
            currentDifficulty = DIFFICULTY_HARD;
            success = true;
            break;
            
        case DIFFICULTY_VERYHARD:
            loadPuzzle(DIFFICULTY_VERYHARD, "Puzzles/VeryHard/veryhard");
            currentDifficulty = DIFFICULTY_VERYHARD;
            success = true;
            break;
            
        case DIFFICULTY_CUSTOM:
            doneButton->setEnabled(true);
            doneButton->setVisibility(true);
            sudoku_->setCustomMode(true);
            sudoku_->setEasyMode(true);
            isPlaying_ = true;
            currentDifficulty = DIFFICULTY_CUSTOM;
            success = true;
            listButtons[0]->setEnabled(false);
            listButtons[3]->setEnabled(false);
            listButtons[4]->setEnabled(false);
            break;
    }
    
    return success;
}

// Loads puzzle from path dependant on difficulty
void GameScreen::loadPuzzle(GameScreen::Difficulty difficulty, std::string path)
{
    // selects one of the 4 available puzzles
    int index = (saveFile_->getIndexValue(difficulty)%4 )+1 ;
    // Loads board from file
    std::string filename = path;
    filename.append( std::to_string(index));
    filename.append(".txt");
    sudoku_->buildFromFile(filename);
    sudoku_->solveSudoku();
    isPlaying_ = true;
}

// Defines difficulty selector button text
void GameScreen::updateButtonText(GameScreen::Difficulty difficulty)
{
    std::string buttonName;
    int value = saveFile_->getIndexValue( difficulty);
    if (value >4) value = 4;
    switch (difficulty) {
        case DIFFICULTY_EASY:
            buttonName = "Easy (";
            buttonName.append( std::to_string(value));
            buttonName.append("/4)");
            break;
            
        case DIFFICULTY_MEDIUM:
            buttonName = "Medium (";
            buttonName.append( std::to_string(value));
            buttonName.append("/4)");
            break;
            
        case DIFFICULTY_HARD:
            buttonName = "Hard (";
            buttonName.append( std::to_string(value));
            buttonName.append("/4)");
            break;
            
        case DIFFICULTY_VERYHARD:
            buttonName = "Very Hard (";
            buttonName.append( std::to_string(value));
            buttonName.append("/4)");
            break;
            
        case DIFFICULTY_CUSTOM:
            buttonName = "Custom (";
            buttonName.append( std::to_string(saveFile_->getIndexValue(4)));
            buttonName.append(")");
            break;
    }
    difficultyPickerButtons_[difficulty]->setText(buttonName);
}


