//
//  SaveFile.hpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 23/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef SaveFile_hpp
#define SaveFile_hpp

#include <iostream>

#include <SDL2/SDL.h>

#define DATA_SIZE 5

class SaveFile
{
public:
    SaveFile( std::string filename );
    
    void incrementIndex(int index);     // Increments the value at index
    Sint32 getIndexValue(int index) const; // Returns the value at index
    
    // Read file and fill data
    virtual bool readFile();
    
    // Write to file
    virtual bool writeFile();
    
private:
    // save file name
    std::string filename_;
    
    // data container
    Sint32 data_[DATA_SIZE ];
};



#endif /* SaveFile_hpp */
