//
//  RectangleButton.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 17/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "RectButton.hpp"

RectButton::RectButton(SDL_Renderer* renderer) : Button(renderer){}

RectButton::RectButton(SDL_Renderer* renderer, int x, int y, int width, int height): Button(renderer,x,y)
{
    this->width_ = width;
    this->height_ = height;
}


void RectButton::handleEvent(SDL_Event *e)
{
    std::cout << "hi\n";
}

void RectButton::render()
{
    if( texture_!=nullptr){
    
    }
}
