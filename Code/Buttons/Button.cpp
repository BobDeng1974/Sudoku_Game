//
//  Button.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 16/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Button.hpp"

Button::Button(SDL_Renderer* renderer)
{
    this->renderer_ = renderer;
}

Button::Button(SDL_Renderer* renderer, int x, int y)
{
    this->renderer_ = renderer;
    setPosition(x, y);
    texture_ = new Texture(renderer_);
}

void Button::setPosition(int x, int y)
{
    this->anchorPoint_.x=x;
    this->anchorPoint_.y=y;
}


