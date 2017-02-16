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

#include <stdio.h>

class Button
{
    public:
        Button( int x, int y, int width, int heigth);
        ~Button();
    
        void setPosition( int x, int y, int width, int heigth);
    
        virtual void handleEvent( SDL_Event *e) = 0;
    
    private:
        int x_;
        int y_;
        int width_;
        int height_;
};

#endif /* Button_hpp */
