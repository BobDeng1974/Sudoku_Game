//
//  ResolutionScreen.hpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 21/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef ResolutionScreen_hpp
#define ResolutionScreen_hpp

#include <vector>
#include <queue>

#include "Window.hpp"
#include "Font.hpp"
#include "SelectorButton.hpp"

class ResolutionScreen
{
    public:
        ResolutionScreen(int windowWidth, int windowHeight);
        ~ResolutionScreen();
        // Initialize Screen
        bool init();
        
        // Start Screen Functions
        bool start();
    
        // Get chosen resolution
        void getResolution(int &width, int &height) const;

    protected:
        // Load Screen Buttons
        virtual bool loadButtons();
        
        //Process Handlers
        virtual bool processHandlers();
        
        // Render screen
        virtual void render();
    
    private:
        // Screen sizes
        int windowWidth_;
        int windowHeight_;
    
        // Window variable
        Window* window_=nullptr;
        
        // Screen Renderer
        SDL_Renderer* renderer_ = nullptr;
        
        //TTF font
        Font* font_ = nullptr;
        
        // List of Buttons
        std::vector<Button*> listButtons;
    
        // List of Handlers (event tracker)
        std::queue<Handler> handlerQueue_;

        // Bool indicates if initiated correctly
        bool hasInitiated=false;
    
        // Bool indicates if user has chosen resolution
        int hasChosen = 0;
        int chosenWidth_;
        int chosenHeight_;
};


#endif /* ResolutionScreen_hpp */
