#ifndef OSC_POLYGON_HPP_INCLUDED
#define OSC_POLYGON_HPP_INCLUDED

#include "osc_common_draw.hpp"
#include "Polygon.h"
#include "FigureGraphic.hpp"
#include "InputGestureDirectObjects.hpp"
#include "InputGestureDirectFingers.hpp"


class OSCPolygonObject :public FigureGraphic, public OSCCommonDrawObject
{
    public:
    Figures::Polygon polygon;
    int cursor;
    int object;
    int id;
    bool drawstroke;
    bool drawpolygon;
    ofColor poly_color;
    ofColor stroke_color;
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
    OSCPolygonObject():FigureGraphic(&polygon),drawpolygon(true),drawstroke(false)
    {
        this->registerMyEvent(InputGestureDirectObjects::I().newObject,&OSCPolygonObject::newObject);
        this->registerMyEvent(InputGestureDirectObjects::I().enterObject,&OSCPolygonObject::newObject);
        this->registerMyEvent(InputGestureDirectObjects::I().removeObject,&OSCPolygonObject::removeObject);
        this->registerMyEvent(InputGestureDirectObjects::I().exitObject,&OSCPolygonObject::removeObject);
        this->registerMyEvent(InputGestureDirectObjects::I().updateObject,&OSCPolygonObject::updateObject);
        this->registerMyEvent(InputGestureDirectFingers::I().newCursor, &OSCPolygonObject::newCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().enterCursor, &OSCPolygonObject::newCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().removeCursor, &OSCPolygonObject::removeCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().exitCursor, &OSCPolygonObject::removeCursor);
        this->registerMyEvent(InputGestureDirectFingers::I().updateCursor, &OSCPolygonObject::updateCursor);
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

    void draw()
    {
        ofPushMatrix();
        ofMultMatrix(total_matrix);
        if(drawpolygon){
            setFill(true);
            color = poly_color;
            FigureGraphic::draw();
        }
        if(drawstroke)
        {
            color = stroke_color;
            setFill(false);
            FigureGraphic::draw();
        }
        ofPopMatrix();
    }
    void cmd_report_matrix()
    {
        ofxOscMessage msg;
        msg.setAddress("/figure/matrix");
        OscPacker(msg) << (int)id <<
            (float)total_matrix(0,0)<<(float)total_matrix(0,1)<<(float)total_matrix(0,2)<<(float)total_matrix(0,3)<<
            (float)total_matrix(1,0)<<(float)total_matrix(1,1)<<(float)total_matrix(1,2)<<(float)total_matrix(1,3)<<
            (float)total_matrix(2,0)<<(float)total_matrix(2,1)<<(float)total_matrix(2,2)<<(float)total_matrix(2,3)<<
            (float)total_matrix(3,0)<<(float)total_matrix(3,1)<<(float)total_matrix(3,2)<<(float)total_matrix(3,3);
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void cmd_color(int r,int g,int b)
    {
        poly_color.set(r,g,b);
    }
    void cmd_hidden(bool ishidden)
    {
         isHidden(ishidden);
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
            int yesno;
            msg >> yesno;
            drawpolygon = (yesno==1);

        }
        else if(cmd == "drawstroke")
        {
            int yesno;
            msg >> yesno;
            drawstroke = (yesno==1);
        }
        else if(cmd == "strokecolor")
        {
            int r,g,b;
            msg >> r >> g >> b;
            stroke_color.set(r,g,b);
        }
        else if(cmd == "strokewidth")
        {
            std::cout << "TODO: strokewidth not implemented" << std::endl;
        }
        else if(cmd == "touchable")
        {
            std::cout << "TODO: touchable not implemented" << std::endl;
        }
        else
        {
            std::cout << "Polygon: command " << cmd << " not found" << std::endl;
        }

    }
};

class OSCFigureDraw: public Singleton<OSCFigureDraw>
{
public:
    OSCCommonDraw<OSCPolygonObject> o;
    OSCFigureDraw():o("/figure"){}
};


#endif // OSC_POLYGON_HPP_INCLUDED
