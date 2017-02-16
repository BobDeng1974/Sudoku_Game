//
//  Cell.hpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Cell_hpp
#define Cell_hpp

#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>

#include <iostream>
#include <string>

#include "Coordinates.hpp"

class Cell
{
    public:
        enum State { STATE_SOLVED, STATE_ROW, STATE_COL, STATE_ROWCOL, STATE_NOK};

        Cell();
        Cell(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int size, int row, int col);

        void setValues(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int size, int row, int col);
    
        void render();
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

        void setFocused(bool state);
        void setBlocked(bool isBlocked);
    
        bool handleFocusEvent( SDL_Event* e);
        void handleInputEvent( SDL_Event* e);
    
        void free();
    
        //Game Logic
        int fixValue();
    
        void setRemovedQueue();
        void setQueued( State mode);
        void setCoordinates(int row, int col);
        void setValue(int value);
        bool setValuePossibility(int value, bool valid);
        void setValuePossibility(std::bitset<9> possibleValues);
    
        Coordinates getCoordinates() const;
        int getValue() const;
        bool getValuePossibility(int value) const;
        std::size_t getNumberPossibilities() const;
        std::bitset<9> getPossibleValues() const;
        bool isQueued() const;
        State getMode() const;
    

    
    private:
        SDL_Renderer* renderer_=nullptr;
        TTF_Font* font_=nullptr;
        SDL_Texture* mTexture=nullptr;
    
        // GUI position and size
        int x_;
        int y_;
        int size_;
    
        // Board coordinates
        int row_;
        int col_;
    
    
        bool isFocused_ = false;
        bool isBlocked_ = false;
    
        // Game Logic Variabless
        Coordinates coords_;
        std::bitset<9> possibleValues_;
        State state_;
        bool isQueued_=false;
        int value_ = 0;

};

#endif /* Cell_hpp */
