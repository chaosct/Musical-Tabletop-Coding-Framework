#ifndef OSC_POLYGON_HPP_INCLUDED
#define OSC_POLYGON_HPP_INCLUDED

#include "osc_common_draw.hpp"
#include "Polygon.h"
#include "FigureGraphic.hpp"



class poly :public FigureGraphic
{
    public:
    Figures::Polygon polygon;
    int cursor;
    int object;
    int id;
    void AddPoint(ofPoint point)
    {
        polygon.AddVertex(point);
        cursor = -1;
        object = -1;
    }
    void Clear()
    {
        polygon = Figures::Polygon();
    }
    bool collide(ofPoint point)
    {
        return polygon.Collide(point);
    }
    void SetTexture(std::string texture)
    {
        if(texture.compare("none")== 0)
        {
            polygon.SetTexture("");
        }
        else
            polygon.SetTexture(texture);
    }
    poly():FigureGraphic(&polygon)
    {
        this->registerMyEvent(InputGestureDirectObjects::I().newObject,&poly::newObject);
        this->registerMyEvent(InputGestureDirectObjects::I().enterObject,&poly::newObject);
        this->registerMyEvent(InputGestureDirectObjects::I().removeObject,&poly::removeObject);
        this->registerMyEvent(InputGestureDirectObjects::I().exitObject,&poly::removeObject);
        this->registerMyEvent(InputGestureDirectObjects::I().updateObject,&poly::updateObject);
        this->registerMyEvent(InputGestureDirectFingers::I().newCursor, &poly::newCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().enterCursor, &poly::newCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().removeCursor, &poly::removeCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().exitCursor, &poly::removeCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().updateCursor, &poly::updateCursor);
    }
    void newCursor(InputGestureDirectFingers::newCursorArgs & a)
    {
        DirectFinger *df = a.finger;
        if (cursor == -1)
        {
            cursor= df->s_id;
            ofxOscMessage msg1;
            msg1.setAddress("/figure/addfinger");
            OscPacker(msg1) << (int)id << (int)df->s_id;
            OSCDispatcher::Instance().sender.sendMessage(msg1);

            ofxOscMessage msg;
            msg.setAddress("/figure/finger");
            OscPacker(msg) << (int)id <<(int)df->s_id << (float) df->getX() << (float) df->getY();
            OSCDispatcher::Instance().sender.sendMessage(msg);
        }
    }
    void removeCursor(InputGestureDirectFingers::removeCursorArgs & a)
    {
        DirectFinger *df = a.finger;

            if(cursor == df->s_id)
            {
                //std::cout << "remove" << std::endl;
                cursor = -1;
                ofxOscMessage msg1;
                msg1.setAddress("/figure/rmfinger");
                OscPacker(msg1) << (int)id << (int)df->s_id;
                OSCDispatcher::Instance().sender.sendMessage(msg1);
            }
    }
    void updateCursor(InputGestureDirectFingers::updateCursorArgs & a)
    {
        DirectFinger *df = a.finger;
       // updatelist.insert(f);

            if(cursor == df->s_id)
            {
                ofxOscMessage msg;
                msg.setAddress("/figure/finger");
                OscPacker(msg) << (int)id <<(int)df->s_id << (float) df->getX() << (float) df->getY();
                OSCDispatcher::Instance().sender.sendMessage(msg);
            }
    }
    void newObject(InputGestureDirectObjects::newObjectArgs & a)
    {
        DirectObject *df = a.object;

           if(object == -1)
           {

                   object= df->f_id;
                   ofxOscMessage msg1;
                   msg1.setAddress("/figure/addobject");
                   OscPacker(msg1) << (int)id << (int)df->f_id;
                   OSCDispatcher::Instance().sender.sendMessage(msg1);

                   ofxOscMessage msg;
                   msg.setAddress("/figure/object");
                   OscPacker(msg) << (int)id <<(int)df->f_id << (float) df->getX() << (float) df->getY();
                   OSCDispatcher::Instance().sender.sendMessage(msg);

           }
    }
    void removeObject(InputGestureDirectObjects::removeObjectArgs & a)
    {
        DirectObject *df = a.object;

            if(object == df->f_id)
            {
                //std::cout << "remove" << std::endl;
                object = -1;
                ofxOscMessage msg1;
                msg1.setAddress("/figure/rmobject");
                OscPacker(msg1) << (int)id << (int)df->f_id;
                OSCDispatcher::Instance().sender.sendMessage(msg1);
            }
    }
    void updateObject(InputGestureDirectObjects::updateObjectArgs & a)
    {
        DirectObject *df = a.object;
       // updatelist.insert(f);

            if(object == df->f_id)
            {
                ofxOscMessage msg;
                msg.setAddress("/figure/object");
                OscPacker(msg) << (int)id <<(int)df->f_id << (float) df->getX() << (float) df->getY();
                OSCDispatcher::Instance().sender.sendMessage(msg);
            }
    }
};

class OSCPolygonObject: public OSCCommonDrawObject, public poly
{
public:
    //OSCCommonDrawObject():poly(){}
    void cmd_color(int r,int g,int b)
    {
        color.set(r,g,b);
    }
    void cmd_position(float x,float y,float angle)
    {
        transformation.setTranslation(x,y,0);
        transformation.setRotate(ofQuaternion(angle,ofVec3f(0,0,1)));
    }
    void cmd_hidden(bool ishidden)
    {
         std::cout << "TODO: hidden not implemented" << std::endl;
    }
    void run_extra(const std::string & cmd, OscOptionalUnpacker & msg)
    {
        if(cmd == "addrectangle")
        {
            std::cout << "TODO: addrectangle not implemented" << std::endl;
        }
        else if(cmd == "addcircle")
        {
            std::cout << "TODO: addcircle not implemented" << std::endl;
        }
        else if(cmd == "addvertex")
        {
            float fx, fy;
            while (!msg.Eos())
            {
                msg >> fx >> fy;
                AddPoint(ofPoint(fx,fy));
            }
        }
        else if(cmd == "clearvertex")
        {
            Clear();
        }
        else if(cmd == "texture")
        {
            std::string tex_name;
            msg >> tex_name;
            SetTexture(tex_name);
        }
        else if(cmd == "drawpolygon")
        {
            std::cout << "TODO: drawpolygon not implemented" << std::endl;
        }
        else if(cmd == "drawstroke")
        {
            std::cout << "TODO: drawstroke not implemented" << std::endl;
        }
        else if(cmd == "strokecolor")
        {
            std::cout << "TODO: strokecolor not implemented" << std::endl;
        }
        else if(cmd == "strokewidth")
        {
            std::cout << "TODO: strokewidth not implemented" << std::endl;
        }
        else if(cmd == "touchable")
        {
            std::cout << "TODO: touchable not implemented" << std::endl;
        }

    }
};

class OSCFigureDraw
{
public:
    OSCCommonDraw<OSCPolygonObject> o;
    OSCFigureDraw():o("/figure"){}
};


#endif // OSC_POLYGON_HPP_INCLUDED
