//
//  Button.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 16/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Button.hpp"

Button::Button(SDL_Renderer* renderer, TTF_Font* font)
{
    this->renderer_ = renderer;
    this->font_ = font;
    texture_ = new Texture(renderer_, font);
}

Button::Button(SDL_Renderer* renderer, TTF_Font* font, int x, int y)
{
    this->renderer_ = renderer;
    this->font_ = font;
    setPosition(x, y);
    texture_ = new Texture(renderer_, font);
}

void Button::setPosition(int x, int y)
{
    this->anchorPoint_.x=x;
    this->anchorPoint_.y=y;
}

void Button::setTextColor(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
{
    textColor = {red, green, blue, alpha};
}


bool Button::setText(std::string text)
{
    if( texture_!=nullptr){
        texture_->loadFromRenderedText(text, textColor );
    }
    return true;
}

// Setters
void Button::setEnabled(bool enabled) { isEnabled_ = enabled; }
void Button::setVisibility(bool visibility) { isVisible_ = visibility; }

// Getters
bool Button::getEnabled() const { return isEnabled_; }
bool Button::getVisibility() const { return isVisible_; }
