//
//  Window.cpp
//  Sudoku
//
//  Created by Ricardo Martins on 15/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Window.hpp"

Window::Window(int windowWidth, int windowHeight, const char* windowTitle, int bgR, int bgG, int bgB){
    this->screenWidth_ = windowWidth;
    this->screenHeight_ = windowHeight;
    this->bgR_ = bgR;
    this->bgG_ = bgG;
    this->bgB_ = bgB;
    
    bool success;
    
    if(SDL_Init(SDL_INIT_VIDEO) <0){
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        success = false;
    }
    else{
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        window_ = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth_, screenHeight_, SDL_WINDOW_SHOWN);
        if(window_==nullptr){
            std::cerr <<  "Window could not be created! SDL_Error: "<< SDL_GetError() << std::endl;
            success = false;
        }
        else{
            // Create renderer
            renderer_ = SDL_CreateRenderer( window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( renderer_ == nullptr){
                std::cerr << "Renderer coundn't be created! SDL Error: " << SDL_GetError() << std::endl;
                success = false;
            }
            else{
                //Initialize renderer color
                SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
}

Window::~Window()
{
    //Destroy window
    SDL_DestroyRenderer( renderer_ );
    SDL_DestroyWindow( window_ );
    window_ = NULL;
    renderer_ = NULL;
    
    //Quit SDL subsystems
    if( isPNGActive_)IMG_Quit();
    SDL_Quit();
}


void Window::clearScreen()
{
    //Clear screen
    SDL_SetRenderDrawColor( renderer_, bgR_, bgG_, bgB_, 0xFF );
    SDL_RenderClear( renderer_ );
}

void Window::updateScreen()
{
    //Update screen
    SDL_RenderPresent( renderer_ );
}

bool Window::initPNG()
{
    if (isPNGActive_)return true;
    
    int imgFlags = IMG_INIT_PNG;
    if ( !( IMG_Init(imgFlags) & imgFlags)){
        std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    isPNGActive_=true;
    return true;
}

bool Window::initTTF()
{
    if( isTTFActive_) return true;
    
    //Initialize SDL_ttf
    if( TTF_Init() == -1 )
    {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }
    isTTFActive_=true;
    return true;
}

bool Window::loadFont(std::string path)
{
    //Loading success flag
    bool success = true;
    
    //Open the font
    font_ = TTF_OpenFont( path.c_str(), 28 );
    if( font_ == NULL )
    {
        std::cerr << "Failed to load lazy font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        success = false;
    }
    return success;
}


SDL_Renderer* Window::getRenderer() const { return renderer_; }
TTF_Font* Window::getFont() const { return font_; }


