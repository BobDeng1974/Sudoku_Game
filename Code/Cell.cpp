//
//  Cell.cpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Cell.hpp"


Cell::Cell(){}

Cell::Cell(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int size, int row, int col)
{
    setValues(renderer, font, x, y, size, row, col);
}

void Cell::setValues(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int size, int row, int col)
{
    this->renderer_ = renderer;
    this->font_ = font;
    this->anchorPoint_.x = x;
    this->anchorPoint_.y = y;
    this->size_ = size;
    this->coords_.setRow(row);
    this->coords_.setCol(col);
}

void Cell::render(bool modeVerify, int value)
{
    SDL_Rect fillRect = { anchorPoint_.x, anchorPoint_.y, size_, size_ };
    
    // Set color according to mode
    if( isFocused_)
        SDL_SetRenderDrawColor( renderer_, 0xE0, 0xE0, 0xE0, 0xFF );  // Cell is focused ( light gray)
    
    else if ( isBlocked_)
        SDL_SetRenderDrawColor( renderer_, 0x99, 0x99, 0x99, 0xFF );  // Cell is from puzzle (dark gray)
    
    else if ( modeVerify){  // Mode verify
        if(value_ == value )
            SDL_SetRenderDrawColor( renderer_, 0x00, 0xE6, 0x99, 0xFF );    // cell is correct
        else if ( value_!=0)
            SDL_SetRenderDrawColor( renderer_, 0xFF, 0x00, 0x00, 0xFF );    // cell is wrong
        else
            SDL_SetRenderDrawColor( renderer_, 0xFF, 0xFF, 0xFF, 0xFF );    // cell is empty
    }
    else SDL_SetRenderDrawColor( renderer_, 0xFF, 0xFF, 0xFF, 0xFF );
    SDL_RenderFillRect( renderer_, &fillRect );
    
    if(value_!=0){
        SDL_Color textColor = { 0, 0, 0, 0xFF };
        loadFromRenderedText( std::to_string(value_) , textColor );
        //Set rendering space and render to screen
        SDL_Rect renderQuad = { anchorPoint_.x + size_/4, anchorPoint_.y, size_/2, size_ };
        SDL_RenderCopy(renderer_, mTexture, NULL, &renderQuad);
    }
    
}


bool Cell::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();
    
    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Blended( font_, textureText.c_str(), textColor );
    if( textSurface != NULL )
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( renderer_, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get image dimensions
            //mWidth = textSurface->w;
            //mHeight = textSurface->h;
        }
        
        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    else
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    
    
    //Return success
    return mTexture != NULL;
}

bool Cell::handleFocusEvent(SDL_Event* e, bool isCustom)
{
    if( isBlocked_ && !isCustom) return false;
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
        else if( x > anchorPoint_.x + size_ )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < anchorPoint_.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > anchorPoint_.y + size_ )
        {
            inside = false;
        }
        
        if(inside)isFocused_ = true;
    
        return inside;
    }
    return false;
}


void Cell::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
    }
  }

void Cell::reset()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
    }
    isFocused_=false;
    isBlocked_=false;
    value_ = 0;
    possibleValues_.reset();
}



void Cell::setFocused(bool state){ isFocused_ = state;}

void Cell::setBlocked(bool isBlocked){ isBlocked_ = isBlocked; }


// Game Logic Funtions

int Cell::fixValue()
{
    if( possibleValues_.count()!=8) return false;
    for(int i=0; i<9; i++){
        if(possibleValues_[i]==0){
            value_ = i+1;
            return value_;
        }
    }
    return -1;
}


//  Setters
void Cell::setFocus(bool isFocused){ this->isFocused_ = isFocused; }

void Cell::setCoordinates(int row, int col)
{
    coords_.setRow(row);
    coords_.setCol(col);
}

void Cell::setValue(int value){ value_=value; }

void Cell::setValuePossibility(std::bitset<9> possibleValues) { possibleValues_=possibleValues; }

bool Cell::setValuePossibility(int value, bool valid) {
    bool wasFalse = false;
    if(!possibleValues_[value-1]) wasFalse=true;
    possibleValues_[value-1]=valid;
    return wasFalse;
}

//  Getters
Coordinates Cell::getCoordinates() const { return coords_; }

int Cell::getValue() const { return value_; }

bool Cell::getValuePossibility(int value) const { return possibleValues_[value-1]; }

std::size_t Cell::getNumberPossibilities() const { return 9-possibleValues_.count(); }

std::bitset<9> Cell::getPossibleValues() const { return possibleValues_; }
