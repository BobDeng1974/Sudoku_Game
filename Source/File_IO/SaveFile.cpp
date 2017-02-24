//
//  SaveFile.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 23/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "SaveFile.hpp"

SaveFile::SaveFile( std::string filename)
{
    this->filename_ = filename;
}

bool SaveFile::readFile()
{
    //return bool
    bool success = true;
    
    //Open file for reading in binary
    SDL_RWops* file = SDL_RWFromFile( filename_.c_str(), "r+b" );
    
    //File does not exist
    if( file == nullptr ){
        //Create file for writing
        file = SDL_RWFromFile( filename_.c_str(), "w+b" );
        if( file != nullptr ){            
            //Initialize data
            for( int i = 0; i < DATA_SIZE; ++i ){
                data_[ i ] = 0;
                SDL_RWwrite( file, &data_[ i ], sizeof(Sint32), 1 );
            }
            //Close file handler
            SDL_RWclose( file );
        }
        else{
            success = false;
        }
    }
    else{
        //Load data
        for( int i = 0; i < DATA_SIZE; ++i )
        {
            SDL_RWread( file, &data_[ i ], sizeof(Sint32), 1 );
        }
        //Close file handler
        SDL_RWclose( file );
    }
    return success;
}

bool SaveFile::writeFile()
{
    bool success = true;
    
    //Open data for writing
    SDL_RWops* file = SDL_RWFromFile( filename_.c_str(), "w+b" );
    if( file != nullptr ){
        //Save data
        for( int i = 0; i < DATA_SIZE; ++i ){
            SDL_RWwrite( file, &data_[ i ], sizeof(Sint32), 1 );
        }
        //Close file handler
        SDL_RWclose( file );
    }
    else{
        success = false;
    }
    return success;
}


void SaveFile::incrementIndex(int index)
{
    if( index < DATA_SIZE) data_[index]++;
}

Sint32 SaveFile::getIndexValue(int index) const
{
    if( index < DATA_SIZE) return data_[index];
    else return 0;
}

