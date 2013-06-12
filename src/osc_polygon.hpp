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
    int id;
    bool drawstroke;
    bool drawpolygon;
    ofColor poly_color;
    ofColor stroke_color;
    int linewidth;
    void AddPoint(ofPoint point)
    {
        polygon.AddVertex(point);
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
    OSCPolygonObject():FigureGraphic(&polygon),drawpolygon(true),drawstroke(false),linewidth(1),id(0)
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
        hasAlpha(true);
        color.set(255,255,255,255);
    }
    void newCursor(InputGestureDirectFingers::newCursorArgs & a)
    {
        if (isHidden()) return;
        DirectFinger *df = a.finger;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/addfinger");
        OscPacker(msg1) << (int)id << (int)df->s_id;
        OSCDispatcher::Instance().sender.sendMessage(msg1);

        ofxOscMessage msg;
        msg.setAddress("/figure/finger");
        OscPacker(msg) << (int)id <<(int)df->s_id << (float) df->getX() << (float) df->getY();
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void removeCursor(InputGestureDirectFingers::removeCursorArgs & a)
    {
        if (isHidden()) return;
        DirectFinger *df = a.finger;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/rmfinger");
        OscPacker(msg1) << (int)id << (int)df->s_id << (float) df->getX() << (float) df->getY();
        OSCDispatcher::Instance().sender.sendMessage(msg1);
    }
    void updateCursor(InputGestureDirectFingers::updateCursorArgs & a)
    {
        if (isHidden()) return;
        DirectFinger *df = a.finger;
        ofxOscMessage msg;
        msg.setAddress("/figure/finger");
        OscPacker(msg) << (int)id <<(int)df->s_id << (float) df->getX() << (float) df->getY();
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void newObject(InputGestureDirectObjects::newObjectArgs & a)
    {
        if (isHidden()) return;
        DirectObject *df = a.object;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/addobject");
        OscPacker(msg1) << (int)id << (int)df->f_id;
        OSCDispatcher::Instance().sender.sendMessage(msg1);

        ofxOscMessage msg;
        msg.setAddress("/figure/object");
        OscPacker(msg) << (int)id <<(int)df->f_id << (float) df->getX() << (float) df->getY();
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void removeObject(InputGestureDirectObjects::removeObjectArgs & a)
    {
        if (isHidden()) return;
        DirectObject *df = a.object;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/rmobject");
        OscPacker(msg1) << (int)id << (int)df->f_id;
        OSCDispatcher::Instance().sender.sendMessage(msg1);
    }
    void updateObject(InputGestureDirectObjects::updateObjectArgs & a)
    {
        if (isHidden()) return;
        DirectObject *df = a.object;
        ofxOscMessage msg;
        msg.setAddress("/figure/object");
        OscPacker(msg) << (int)id <<(int)df->f_id << (float) df->getX() << (float) df->getY();
        OSCDispatcher::Instance().sender.sendMessage(msg);
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
            ofPushStyle();
            ofSetLineWidth(linewidth);
            color = stroke_color;
            setFill(false);
            FigureGraphic::draw();
            ofPopStyle();
        }
        if(not (drawpolygon or drawstroke) )
        {
            bool ishidden = isHidden();
            isHidden(true);
            FigureGraphic::draw();
            isHidden(ishidden);
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
    void cmd_color(int r,int g,int b,int a)
    {
        poly_color.set(r,g,b,a);
    }
    void cmd_hidden(bool ishidden)
    {
         isHidden(ishidden);
    }
    void cmd_bring_top()
    {
        BringTop();
    }
    void cmd_set_layer(int _layer)
    {
        SetLayer(_layer);
    }
    void run_extra(const std::string & cmd, OscOptionalUnpacker & msg)
    {
        if(cmd == "addrectangle")
        {
            Clear();
            float x,y,h,w,r;
            x=y=h=w=r=0;
            msg >> x >> y >> w >> h >> r;

            if (!(r > w || r > h || r <= 0))
            {
                ofPolyline shape;
                float x2 = x + w;
                float y2 = y + h;
                shape.lineTo(x+r, y);
                shape.bezierTo(x,y, x,y+r, x,y+r);
                shape.lineTo(x, y2-r);
                shape.bezierTo(x,y2, x+r,y2, x+r,y2);
                shape.lineTo(x2-r, y2);
                shape.bezierTo(x2,y2, x2,y2-r, x2,y2-r);
                shape.lineTo(x2, y+r);
                shape.bezierTo(x2,y, x2-r,y, x2-r,y);
                shape.lineTo(x+r, y);
                vector<ofPoint> & vert = shape.getVertices();
                for (std::vector<ofPoint>::iterator it = vert.begin(); it != vert.end(); ++it)
                {
                    AddPoint(*it);
                }

            }
            else
            {
                AddPoint(ofPoint(x-w/2,y-h/2));
                AddPoint(ofPoint(x+w/2,y-h/2));
                AddPoint(ofPoint(x+w/2,y+h/2));
                AddPoint(ofPoint(x-w/2,y+h/2));
            }
        }
        else if(cmd == "addcircle")
        {
            Clear();
            float x,y,r;
            int nvertex;
            msg >> x >> y >> r >> nvertex;
            for (int i =0; i < nvertex; ++i)
            {
                float cx = x + r * cos(M_PI*2.0/nvertex*i);
                float cy = y + r * sin(M_PI*2.0/nvertex*i);
                AddPoint(ofPoint(cx,cy));
            }
            
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
            float w;
            msg >> w;
            linewidth = static_cast<int>(floor(w + 0.5f));
        }
        else if(cmd == "touchable")
        {
            int c;
            msg >> c;
            canCollide(c != 0);
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
