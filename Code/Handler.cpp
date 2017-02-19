//
//  Handler.cpp
//  Sudoku_Game
//
//  Created by Ricardo Martins on 19/02/2017.
//  Copyright © 2017 Ricardo Martins. All rights reserved.
//

#include "Handler.hpp"

Handler::Handler(){}

Handler::Handler( Handler::Event event)
{
    setEvent(event);
}

void Handler::setEvent(Handler::Event event){ this->event_ = event; }

Handler::Event Handler::getEvent() const { return event_;}
