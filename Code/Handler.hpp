//
//  Handler.hpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 19/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#ifndef Handler_hpp
#define Handler_hpp

#include <stdio.h>

class Handler
{
    public:
        enum Event{ EVENT_IGNORE, EVENT_INPUT, EVENT_VERIFY, EVENT_RESET, EVENT_NEWGAME, EVENT_PICKER, EVENT_HINT, EVENT_EASY, EVENT_CUSTOM};

        Handler();
        Handler( Handler::Event event);
    
        // Setters
        void setEvent(Handler::Event event);
        void setIntExtra(int extra);
    
        // Getters
        Handler::Event getEvent() const;
        int getIntExtra()const;
    
    private:
        Event event_;
    
        int intExtra_;
};

#endif /* Handler_hpp */
