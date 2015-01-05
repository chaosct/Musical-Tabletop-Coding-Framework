#ifndef OSC_POLYGON_HPP_INCLUDED
#define OSC_POLYGON_HPP_INCLUDED

#include "EventClient.hpp"
#include "osc_common_draw.hpp"
#include "GenericManager.hpp"
#include "GraphicDispatcher.hpp"
#include "Polygon.hpp"
#include "InputGestureDirectObjects.hpp"
#include "InputGestureDirectFingers.hpp"
#include "ofPath.h" // For generating rounded rectangles

class OSCPolygonObject : public EventClient, public OSCCommonDrawObject
{
    public:
    tableGraphics::Polygon* polygon;
    bool drawpolygon;
    bool drawstroke;
    void AddPoint(ofPoint point)
    {
        polygon->addVertex(point);
    }
    void Clear()
    {
        polygon->clear();
    }
    void SetTexture(std::string texture)
    {
        if(texture.compare("none")== 0)
        {
            polygon->SetTexture("");
        }
        else
        {
            polygon->SetTexture(texture);
        }
    }

    OSCPolygonObject() : drawpolygon(true),
                         drawstroke(false)
    {
        GenericManager::get<GraphicDispatcher>()->createGraphic(polygon);
        polygon->setCollide(true);
        registerEvents();
    }

    ~OSCPolygonObject() {
        GenericManager::get<GraphicDispatcher>()->removeGraphic(polygon);
    }

    void registerEvents(){
        registerEvent(InputGestureDirectObjects::newObject,&OSCPolygonObject::newObject, this, polygon);
        registerEvent(InputGestureDirectObjects::enterObject,&OSCPolygonObject::newObject, this, polygon);
        registerEvent(InputGestureDirectObjects::removeObject,&OSCPolygonObject::removeObject, this, polygon);
        registerEvent(InputGestureDirectObjects::exitObject,&OSCPolygonObject::removeObject, this, polygon);
        registerEvent(InputGestureDirectObjects::updateObject,&OSCPolygonObject::updateObject, this, polygon);
        registerEvent(InputGestureDirectFingers::newCursor, &OSCPolygonObject::newCursor, this, polygon);
        registerEvent(InputGestureDirectFingers::enterCursor, &OSCPolygonObject::newCursor, this, polygon);
        registerEvent(InputGestureDirectFingers::removeCursor, &OSCPolygonObject::removeCursor, this, polygon);
        registerEvent(InputGestureDirectFingers::exitCursor, &OSCPolygonObject::removeCursor, this, polygon);
        registerEvent(InputGestureDirectFingers::updateCursor, &OSCPolygonObject::updateCursor, this, polygon);
    }

    void unregisterEvents(){
        unregisterEvent(InputGestureDirectObjects::newObject);
        unregisterEvent(InputGestureDirectObjects::enterObject);
        unregisterEvent(InputGestureDirectObjects::removeObject);
        unregisterEvent(InputGestureDirectObjects::exitObject);
        unregisterEvent(InputGestureDirectObjects::updateObject);
        unregisterEvent(InputGestureDirectFingers::newCursor);
        unregisterEvent(InputGestureDirectFingers::enterCursor);
        unregisterEvent(InputGestureDirectFingers::removeCursor);
        unregisterEvent(InputGestureDirectFingers::exitCursor);
        unregisterEvent(InputGestureDirectFingers::updateCursor);
    }

    void newCursor(InputGestureDirectFingers::newCursorArgs & a)
    {
        DirectFinger *df = a.finger;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/addfinger");
        OscPacker(msg1) << (int)id << (int)df->s_id;
        OSCDispatcher::Instance().sender.sendMessage(msg1);

        ofxOscMessage msg;
        msg.setAddress("/figure/finger");
        OscPacker(msg) << (int)id <<(int)df->s_id << (float) df->x << (float) df->y;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }

    void removeCursor(InputGestureDirectFingers::removeCursorArgs & a)
    {
        DirectFinger *df = a.finger;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/rmfinger");
        OscPacker(msg1) << (int)id << (int)df->s_id << (float) df->x << (float) df->y;
        OSCDispatcher::Instance().sender.sendMessage(msg1);
    }
    void updateCursor(InputGestureDirectFingers::updateCursorArgs & a)
    {
        DirectFinger *df = a.finger;
        ofxOscMessage msg;
        msg.setAddress("/figure/finger");
        OscPacker(msg) << (int)id <<(int)df->s_id << (float) df->x << (float) df->y;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void newObject(InputGestureDirectObjects::newObjectArgs & a)
    {
        DirectObject *df = a.object;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/addobject");
        OscPacker(msg1) << (int)id << (int)df->f_id;
        OSCDispatcher::Instance().sender.sendMessage(msg1);

        ofxOscMessage msg;
        msg.setAddress("/figure/object");
        OscPacker(msg) << (int)id <<(int)df->f_id << (float) df->x << (float) df->y;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void removeObject(InputGestureDirectObjects::removeObjectArgs & a)
    {
        DirectObject *df = a.object;
        ofxOscMessage msg1;
        msg1.setAddress("/figure/rmobject");
        OscPacker(msg1) << (int)id << (int)df->f_id;
        OSCDispatcher::Instance().sender.sendMessage(msg1);
    }
    void updateObject(InputGestureDirectObjects::updateObjectArgs & a)
    {
        DirectObject *df = a.object;
        ofxOscMessage msg;
        msg.setAddress("/figure/object");
        OscPacker(msg) << (int)id <<(int)df->f_id << (float) df->x << (float) df->y;
        OSCDispatcher::Instance().sender.sendMessage(msg);
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
        polygon->setMatrix(total_matrix);
    }
    void cmd_color(int r,int g,int b,int a)
    {
        polygon->setColor(ofColor(r, g, b, a));
    }
    void cmd_hidden(bool isHidden)
    {
        if (isHidden){
            polygon->setVisible(true);
        }else{
            polygon->setVisible(false);
        }
    }
    void cmd_bring_top()
    {
        GenericManager::get<GraphicDispatcher>()->bringTop(polygon);
    }
    void cmd_set_layer(int _layer)
    {
        GenericManager::get<GraphicDispatcher>()->setLayer(_layer, polygon);
    }
    void cmd_load_shader(std::string path)
    {
        bool success = polygon->loadShader(path);
        if(!success){
            ofLogWarning("OSCPolygonObject::cmd_load_shader()") << "Failed, check shader name: " << path;
        }
    }
    void run_extra(const std::string & cmd, OscOptionalUnpacker & msg)
    {
        if(cmd == "rectangle")
        {
            float w, h;
            w = h = 0.0f;
            msg >> w >> h;
            polygon->rectangle(w, h);
        }
        else if(cmd == "circle")
        {
            int res = 64;
            float radius, star;
            radius = star = 0.0f;
            msg >> radius >> res >> star;
            polygon->circle(radius, res, star);
        }
        else if(cmd == "addrectangle")
        {
            Clear();
            float x,y,h,w,r;
            x=y=h=w=r=0;
            msg >> x >> y >> w >> h >> r;

            ofPath path;
            path.rectRounded(x - w/2, y - h/2, w, h, r);
            path.setCircleResolution(60);
            vector<ofPoint>& vertices = path.getOutline()[0].getVertices();
            for (std::vector<ofPoint>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
                AddPoint(*it);
            }
        }
        else if(cmd == "addcircle")
        {
            Clear();
            float x,y,r;
            int nvertex = 30;
            msg >> x >> y >> r >> nvertex;
            for (int i =0; i < nvertex; ++i)
            {
                float cx = x + r * cos(PI*2.0/nvertex*i);
                float cy = y + r * sin(PI*2.0/nvertex*i);
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
            int draw;
            msg >> draw;
            polygon->setDraw(tableGraphics::Polygon::FILL, static_cast<bool>(draw));
        }
        else if(cmd == "drawstroke")
        {
            int draw;
            msg >> draw;
            polygon->setDraw(tableGraphics::Polygon::STROKE, static_cast<bool>(draw));
        }
        else if(cmd == "strokecolor")
        {
            int r, g, b, a;
            msg >> r >> g >> b;
            if (msg.Eos()) {
                a = 255;
            } else {
                msg >> a;
            }
            polygon->setStrokeColor(ofColor(r, g, b, a));
        }
        else if(cmd == "strokewidth")
        {
            float w;
            msg >> w;
            polygon->setStrokeWidth(w);
        }
        else if(cmd == "touchable")
        {
            int c;
            msg >> c;
            polygon->setCollide(c);
        }
        else
        {
            std::cout << "Polygon: command " << cmd << " not found" << std::endl;
        }

    }
};

class OscPolygonDraw: public Singleton<OscPolygonDraw>
{
public:
    OSCCommonDraw<OSCPolygonObject> o;
    OscPolygonDraw():o("/figure"){}
};


#endif // OSC_POLYGON_HPP_INCLUDED
