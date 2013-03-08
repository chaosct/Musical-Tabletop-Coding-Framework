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
#include "ofVectorMath.h"

class OSCCommonDrawObject
{
public:
    virtual void cmd_color(int r,int g,int b,int a){}
    virtual void cmd_hidden(bool ishidden){}
    virtual void run_extra(const std::string & cmd, OscOptionalUnpacker & msg){}
    virtual void cmd_report_matrix(){}
    virtual void cmd_bring_top(){}
    virtual void cmd_set_layer(int _layer){}

    ofMatrix4x4 transform_matrix;
    ofMatrix4x4 translate_matrix;
    ofMatrix4x4 total_matrix;

    void recalculate_total_matrix()
    {
        total_matrix = translate_matrix * transform_matrix;
        cmd_report_matrix();
    }

    void run(const std::string & cmd, OscOptionalUnpacker & msg)
    {
        if(cmd == "color")
        {
            int r,g,b,a;
            msg >> r >> g >> b >> a;
            cmd_color(r,g,b,a);
        }
        else if(cmd == "transform")
        {
            transform_matrix.makeIdentityMatrix();
            while(!msg.Eos())
            {
                std::string command;
                msg >> command;
                if (command == "t")
                {
                    float dx,dy;
                    msg >> dx >> dy;
                    transform_matrix.glTranslate(dx,dy,0.0f);
                }
                else if (command == "r")
                {
                    float alpha;
                    msg >> alpha;
                    transform_matrix.glRotate(alpha,.0f,.0f,1.0f);
                }
                else if (command == "s")
                {
                    float sx,sy;
                    msg >> sx >> sy;
                    transform_matrix.glScale(sx,sy,1.0f);
                }
                else if (command == "S")
                {
                    float sxy;
                    msg >> sxy;
                    transform_matrix.glScale(sxy,sxy,1.0f);
                }
                else
                {
                    std::cout << "transform: command " << command << " not found" << std::endl;
                }
            }
            recalculate_total_matrix();
        }
        else if(cmd == "hidden")
        {
            int  ishidden;
            msg >> ishidden;
            cmd_hidden(ishidden != 0);
        }
        else if(cmd == "layer")
        {
            int _layer;
            msg >> _layer;
            cmd_set_layer(_layer);
        }
        else if(cmd == "bring_top")
        {
            cmd_bring_top();
        }
        else if(cmd == "position")
        {
            float x,y,angle;
            msg >> x >> y >> angle;
            translate_matrix.setTranslation(x,y,0);
            translate_matrix.setRotate(ofQuaternion(angle,ofVec3f(0,0,1)));
            recalculate_total_matrix();
        }
        else if(cmd == "matrix")
        {
            float m00,m01,m02,m03,
            m10,m11,m12,m13,
            m20,m21,m22,m23,
            m30,m31,m32,m33;
            msg >> m00>>m01>>m02>>m03>>
            m10>>m11>>m12>>m13>>
            m20>>m21>>m22>>m23>>
            m30>>m31>>m32>>m33;
            transform_matrix.set(m00,m01,m02,m03,
            m10,m11,m12,m13,
            m20,m21,m22,m23,
            m30,m31,m32,m33);
            translate_matrix.makeIdentityMatrix();
            recalculate_total_matrix();
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
    void reset()
    {
        for (typename std::map<int, T*>::iterator it = objects.begin();
              it != objects.end(); ++it)
              {
                  delete (it->second);
              }
        objects.clear();
    }
    void run(ofxOscMessage & m)
    {
        OscOptionalUnpacker msg(m);
        std::string cmd;
        int id;
        msg >> id >> cmd;
        //std::cout << cmd << std::endl;
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
