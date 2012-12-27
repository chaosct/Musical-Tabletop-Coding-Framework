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


class OSCCommonDrawObject
{
public:
    virtual void cmd_color(int r,int g,int b){}
    virtual void cmd_position(float x,float y,float angle){}
    virtual void cmd_hidden(bool ishidden){}
    virtual void run_extra(const std::string & cmd, OscOptionalUnpacker & msg){}
    void run(const std::string & cmd, OscOptionalUnpacker & msg)
    {
        if(cmd == "color")
        {
            int r,g,b;
            msg >> r >> g >> b;
            cmd_color(r,g,b);
        }
        else if(cmd == "transform")
        {
            std::cout << "TODO: transform not implemented" << std::endl;
        }
        else if(cmd == "hidden")
        {
            int  ishidden;
            msg >> ishidden;
            cmd_hidden(ishidden != 0);
        }
        else if(cmd == "layer")
        {
            std::cout << "TODO: layer not implemented" << std::endl;
        }
        else if(cmd == "bring_top")
        {
            std::cout << "TODO: bring_top not implemented" << std::endl;
        }
        else if(cmd == "position")
        {
            float x,y,angle;
            msg >> x >> y >> angle;
            cmd_position(x,y,angle);
        }
        else if(cmd == "matrix")
        {
            std::cout << "TODO: matrix not implemented" << std::endl;
        }
        else
        {
            run_extra(cmd,msg);
        }

    }
};


template<typename T>
class OSCCommonDraw: public OSCCMD
{
public:
    OSCCommonDraw(const std::string & addr):OSCCMD(addr){}

    std::map<int, T*> objects;

    void run(ofxOscMessage & m)
    {
        OscOptionalUnpacker msg(m);
        std::string cmd;
        int id;
        msg >> id >> cmd;
        std::cout << cmd << std::endl;
        if(objects.find(id) == objects.end())
        {
            objects[id] = new T();
            objects[id]->id = id;
        }
        if (cmd == "rm")
        {
            delete objects[id];
            objects.erase(id);

        }
        else
        {
            objects[id]->run(cmd,msg);
        }
    }

};
