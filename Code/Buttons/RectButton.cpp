//
//  RectangleButton.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 17/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "RectButton.hpp"

RectButton::RectButton(SDL_Renderer* renderer, TTF_Font* font) : Button(renderer, font){}

RectButton::RectButton(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int width, int height): Button(renderer, font,x,y)
{
    this->width_ = width;
    this->height_ = height;
}


Handler RectButton::handleEvent(SDL_Event *e)
{
    // return Handler
    Handler handler(Handler::EVENT_IGNORE);
    
    //If mouse event happened
    if( e->type == SDL_MOUSEBUTTONDOWN)
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );
        
        //Check if mouse is in button
        bool inside = true;
        
        //Mouse is left of the button
        if( x < anchorPoint_.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > anchorPoint_.x + width_ )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < anchorPoint_.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > anchorPoint_.y + height_ )
        {
            inside = false;
        }
        
        if(inside){
            isClicked_ = !isClicked_;
            handler.setEvent(event_);
        }
    }
    return handler;
}

void RectButton::reset()
{
    isClicked_=false;
}

void RectButton::render()
{
    SDL_Rect fillRect = { anchorPoint_.x, anchorPoint_.y, width_, height_ };
    if( isClicked_)SDL_SetRenderDrawColor( renderer_, 0xCC, 0xCC, 0xCC, 0xFF );
    else SDL_SetRenderDrawColor( renderer_, 0xE5, 0xE5, 0xE5, 0xFF );
    SDL_RenderFillRect( renderer_, &fillRect );
    
    //Render green outlined quad
    SDL_Rect outlineRect = { anchorPoint_.x, anchorPoint_.y, width_, height_ };
    SDL_SetRenderDrawColor( renderer_, 0x00, 0x00, 0x00, 0xFF );
    SDL_RenderDrawRect( renderer_, &outlineRect );

    if(texture_!=nullptr)
        texture_->render( anchorPoint_.x + (width_ - texture_->getWidth())/2, anchorPoint_.y + (height_ - texture_->getHeight())/2);

}

// Sets Button event to be set on clicked
void RectButton::setCallbackEvent(Handler::Event event){ this->event_ = event; }
