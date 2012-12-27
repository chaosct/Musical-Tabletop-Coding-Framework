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

class OscBGChanger: public OSCCMD, public BackgroundGraphic
{
    int color1,color2,color3;
    public:
    OscBGChanger():OSCCMD("/background")
    {//0.2f,0.4f,0.6f,
        color1 = 51;
        color2 = 102;
        color3 = 153;
    }

    void run(ofxOscMessage & m)
    {
        OscOptionalUnpacker(m) >> color1 >> color2 >> color3;
        if (color1 < 0) color1 = 0;
        if (color2 < 0) color2 = 0;
        if (color3 < 0) color3 = 0;
        if (color1 > 255) color1 = 255;
        if (color2 > 255) color2 = 255;
        if (color3 > 255) color3 = 255;
    }

    void draw()
    {
        ofSetColor(color1,color2,color3);
        ofRect(0.0f,0.0f,1.0f,1.0f);
    }
};

class OscFingerColor: public OSCCMD
{
    public:
    OscFingerColor():OSCCMD("/fingercolor"){}

    void run(ofxOscMessage & m)
    {
        static int & R = ofxGlobalConfig::getRef("FEEDBACK:CURSOR:COLOR:R",255);
        static int & G = ofxGlobalConfig::getRef("FEEDBACK:CURSOR:COLOR:G",0);
        static int & B = ofxGlobalConfig::getRef("FEEDBACK:CURSOR:COLOR:B",0);

        OscOptionalUnpacker(m) >> R >> G >> B;
    }

};

class OscGlobalReset: public OSCCMD
{
public:
    OscGlobalReset():OSCCMD("/mtcf/reset"){}

    void run(ofxOscMessage & m)
    {
        std::cout << "TODO: Reset everything" << std::endl;
    }
};