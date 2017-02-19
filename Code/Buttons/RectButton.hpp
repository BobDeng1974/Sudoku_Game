//
//  RectButton.hpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 17/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef RectButton_hpp
#define RectButton_hpp

#include <iostream>

#include "Button.hpp"
#include "Handler.hpp"

class RectButton : public Button
{
    public:
        RectButton(SDL_Renderer* renderer, TTF_Font* font);
        RectButton(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height);
    
        // handle Events
        Handler handleEvent(SDL_Event *e);
    
        // render button
        void render();
    
    protected:
        int width_;
        int height_;
    
        bool isClicked_ = false;
  
};


#endif /* RectangleButton_hpp */
