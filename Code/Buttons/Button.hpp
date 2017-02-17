//
//  Button.hpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 16/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Button_hpp
#define Button_hpp

#include <SDL2/SDL.h>

#include "Texture.hpp"

class Button
{
    public:
        // Constructors with renderer required
        Button(SDL_Renderer* renderer);
        Button(SDL_Renderer* renderer, int x, int y);
    
        // set 2D position of button
        void setPosition( int x, int y);
    
        // handle event on button
        virtual void handleEvent( SDL_Event *e) = 0;
    
        virtual void render() = 0;
    
    protected:
        // button location
        SDL_Point anchorPoint_;
    
        // Window Renderer
        SDL_Renderer* renderer_;
    
        // Button Texture
        Texture* texture_=nullptr;
};

#endif /* Button_hpp */
