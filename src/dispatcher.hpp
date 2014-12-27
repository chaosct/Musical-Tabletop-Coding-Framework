/*

    Musical Tabletop Coding Framework

    Copyright (c) 2010 Carles F. Julià <carles.fernandez@upf.edu>
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

#ifndef DISPATCHER_HPP_INCLUDED
#define DISPATCHER_HPP_INCLUDED


#include "Singleton.hpp"
#include "ofxOsc.h"
#include "ofxGlobalConfig.hpp"
#include <map>


class OSCCMD
{
    public:
    std::string addr;
    OSCCMD(const std::string & addr);
    virtual void run(ofxOscMessage & m)=0;
};

class OSCDispatcher: public Singleton<OSCDispatcher>
{
    ofxOscReceiver receiver;
    std::map<std::string,OSCCMD*> commands;
    std::string & remoteIp;
    int & remotePort;
    public:

    ofxOscSender sender;
    OSCDispatcher();
    void update();
    void addCommand(OSCCMD * cmd);
};


#endif // DISPATCHER_HPP_INCLUDED
