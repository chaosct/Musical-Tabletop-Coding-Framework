/*

    Musical Tabletop Coding Framework

    Copyright (c) 2012 Carles F. Julià <carles.fernandez@upf.edu>
    Copyright (c) 2010 Daniel Gallardo Grassot <daniel.gallardo@upf.edu>

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation
    files (the "Software"), to deal in the Software without
    restriction, including without limitation the rights to use,
    copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following
    conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.

*/

#pragma once
#include "dispatcher.hpp"
#include "OscTools.hpp"
#include <set>
#include "InputGestureDirectFingers.hpp"

class OscFingerReporter : public Graphic
{

    std::set< DirectFinger * > updatelist;

    public:
    void newCursor(InputGestureDirectFingers::newCursorArgs & a)
    {
        DirectFinger *df = a.finger;
        ofxOscMessage msg;
        msg.setAddress("/finger");
        OscPacker(msg) << std::string("in") << (int)df->s_id << (float) df->x << (float) df->y;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void removeCursor(InputGestureDirectFingers::removeCursorArgs & a)
    {
        DirectFinger *df = a.finger;
        ofxOscMessage msg;
        msg.setAddress("/finger");
        OscPacker(msg) << std::string("out") << (int)df->s_id << (float) df->x << (float) df->y;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void updateCursor(InputGestureDirectFingers::updateCursorArgs & a)
    {
        DirectFinger *f = a.finger;
        updatelist.insert(f);
    }

    void update()
    {
        for(std::set< DirectFinger * >::iterator it = updatelist.begin(); it != updatelist.end(); ++it) 
        {
            DirectFinger *df = *it;
            ofxOscMessage msg;
            msg.setAddress("/finger");
            OscPacker(msg) << std::string("move") << (int)df->s_id << (float) df->x << (float) df->y;
            OSCDispatcher::Instance().sender.sendMessage(msg);
        }
        updatelist.clear();
    }
    
    OscFingerReporter()
    {
        this->registerEvent(InputGestureDirectFingers::I().newCursor, &OscFingerReporter::newCursor);
        this->registerEvent(InputGestureDirectFingers::I().removeCursor, &OscFingerReporter::removeCursor);
        this->registerEvent(InputGestureDirectFingers::I().updateCursor, &OscFingerReporter::updateCursor);
        
    }

};