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
        enum Event{ EVENT_IGNORE, EVENT_INPUT, EVENT_VERIFY};

        Handler();
        Handler( Handler::Event event);
    
        void setEvent(Handler::Event event);

        Handler::Event getEvent() const;
        
    private:
        Event event_;
};

#endif /* Handler_hpp */
