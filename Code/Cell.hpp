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
        Cell();
        Cell(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int size, int row, int col);

        // Set cell values
        void setValues(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int size, int row, int col);
    
        // Render Cell
        void render( bool modeVerify=false, int value = 0);
    
        // Load text texture
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

        // Mode Setters
        void setFocused(bool state);
        void setBlocked(bool isBlocked);
    
        // Input Handlers
        bool handleFocusEvent( SDL_Event* e);
        void handleInputEvent( SDL_Event* e);
    
        // Resets Cell
        void reset();
    
        // Deallocator
        void free();
    
        //Game Logic
        // When only 1 possibiliy, set value for it
        int fixValue();
    
        // Setters
        void setCoordinates(int row, int col);
        void setValue(int value);
        bool setValuePossibility(int value, bool valid);  // set a bit to valid
        void setValuePossibility(std::bitset<9> possibleValues); // set the comple bitset
    
        // Getters
        Coordinates getCoordinates() const;
        int getValue() const;
        bool getValuePossibility(int value) const;
        std::size_t getNumberPossibilities() const; // get number of negatives on bitset
        std::bitset<9> getPossibleValues() const;


    
    private:
        // SDL Variables
        SDL_Renderer* renderer_=nullptr;
        TTF_Font* font_=nullptr;
        SDL_Texture* mTexture=nullptr;
    
        // GUI position and size
        SDL_Point anchorPoint_;
        int size_;
    
        // Board coordinates
        Coordinates coords_;

        // State Variables
        bool isFocused_ = false;
        bool isBlocked_ = false;
    
        // Game Logic Variabless
        std::bitset<9> possibleValues_;
        int value_ = 0;
};

#endif /* Cell_hpp */
