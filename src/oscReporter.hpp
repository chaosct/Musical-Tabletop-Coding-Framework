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

#ifndef OSCREPORTER_HPP_INCLUDED
#define OSCREPORTER_HPP_INCLUDED

#include "dispatcher.hpp"
#include <set>
#include <map>
#include "simpleAllObjects.hpp"

class OscWaveDraw: public OSCCMD, public Graphic, public Singleton<OscWaveDraw>
{
    class Wave
    {
        float vector[200];
        public:
        DirectPoint p1,p2;
        Wave():p1(0,0.5),p2(1,0.5)
        {
            //vector = new float[2*100];
            for(int i = 0; i < 100 ; i++)
            {
                vector[2*i]=i;
            }

        }
        ~Wave()
        {
            //delete[] vector;
        }
        void update(OscOptionalUnpacker & msg)
        {
            // int costats = 10;
            for (int i = 0; i < 100 ; i++)
            {
                int v;
                msg >> v;
                float f = (2.0 * v / 999.0) -1;
//                if(i < costats)
//                {
//                    float factor = (float)i/costats;
//                    f = f * factor;
//                }
//                else if (i >= (100-costats))
//                {
//                    float factor = (float)(100-i)/costats;
//                    f = f * factor*factor*factor;
//                }
                vector[2*i+1]= f;
            }
        }
        void draw()
        {
            glPushMatrix();
            //orientar i posar a lloc
            glTranslatef(p1.getX(),p1.getY(),0);
            glRotatef(p1.getAngle(p2)*180.0/PI,0,0,1);
            float dist = p1.getDistance(p2);
            glScalef(dist,dist,1);

            glScalef(1,0.3,1);
            glScalef(0.01,0.5,1);
            ofSetColor(255,255,255);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(2,GL_FLOAT,0,vector);
            glDrawArrays(GL_LINE_STRIP,0,100);
            glDisableClientState(GL_VERTEX_ARRAY);
            glPopMatrix();
        }
    };
    std::map< int, Wave> waves;

    public:
    OscWaveDraw():OSCCMD("/wave")
    {

    }
    void run(ofxOscMessage & m)
    {
        OscOptionalUnpacker msg(m);
        std::string cmd;
        int id;
        msg >> id >> cmd;
        if (cmd == "wav")
        {
            waves[id].update(msg);
        }
        else if(cmd == "rm")
        {
            waves.erase(id);
        }
        else if(cmd == "p1")
        {
            float x,y;
            msg >> x >> y;
            waves[id].p1.set(x,y);
        }
        else if(cmd == "p2")
        {
            float x,y;
            msg >> x >> y;
            waves[id].p2.set(x,y);
        }
    }
    void draw()
    {
       for (std::map< int, Wave>::iterator it = waves.begin(); it != waves.end(); ++it)
       {
           it->second.draw();
       }
    }
    void reset()
    {
        waves.clear();
    }
};

class object_data{
    public:
        bool can_cursors, can_angle;
        float xpos,ypos,angle,f_value,a_value,angle_increment;
        object_data():can_cursors(false), can_angle(false),xpos(0),ypos(0),angle(0),f_value(0),a_value(0),angle_increment(0){}
};

class OscObjectReporter: public OSCCMD, public Graphic
{
    std::map<int,object_data> fiducials;
    std::set< int > fiducialstoupdate;
    SimpleAllObjects objects;
    float control_radius, bar_width, bullet_width,bullet_radius;
    public:
    OscObjectReporter():
        OSCCMD("/objects"),
        control_radius(ofxGlobalConfig::getRef("FIGURE:CONTROLLRADIUS",0.065f)),
        bar_width(ofxGlobalConfig::getRef("FIGURE:BARWIDTH",0.012f)),
        bullet_width(ofxGlobalConfig::getRef("FIGURE:BULLETWIDTH",0.01f)),
        bullet_radius(ofxGlobalConfig::getRef("FIGURE:BULLETRADIUS",0.07f))
    {
        this->registerEvent(InputGestureDirectObjects::I().newObject,&OscObjectReporter::newObject);
        this->registerEvent(InputGestureDirectObjects::I().removeObject,&OscObjectReporter::removeObject);
        this->registerEvent(InputGestureDirectObjects::I().updateObject,&OscObjectReporter::updateObject);
        this->registerEvent(InputGestureDirectFingers::I().newCursor, &OscObjectReporter::newCursor);
        //this->registerEvent(InputGestureDirectFingers::I().removeCursor, &OscObjectReporter::removeCursor);
        this->registerEvent(InputGestureDirectFingers::I().updateCursor, &OscObjectReporter::updateCursor);

    }
    void run(ofxOscMessage & m)
    {
        int fid;
        std::string cmd;
        OscOptionalUnpacker msg(m);


        // >> fid >> canangle >> cancursor >> deleteme;
        msg >> fid >> cmd;
        if(cmd == "fvalue")
        {
            float value;
            msg >> value;
            //std::cout << value << std::endl;
            fiducials[fid].f_value = value;
        }
        else if(cmd == "config")
        {
            int deleteme = 0;
            int canangle = 0;
            int cancursor = 0;
            msg >> canangle >> cancursor >> deleteme;
            if(deleteme)
            {
                fiducials.erase(fid);
                return;
            }
            fiducials[fid].can_angle   = (canangle != 0);
            fiducials[fid].can_cursors = (cancursor != 0);
        }
        if(objects.isOnTable(fid))
        {
            reporton(fid);
        }
        else
        {
            reportoff(fid);
        }
    }
    void update()
    {
        std::set< int >::iterator it;
        for(it = fiducialstoupdate.begin();it != fiducialstoupdate.end(); ++it)
        {
            const int & f = *it;
            if(objects.isOnTable(f))report(objects[f]);
        }
        fiducialstoupdate.clear();
    }


    void reporton (int fid)
    {
        //report on
        ofxOscMessage msg;
        msg.setAddress("/object/on");
        OscPacker(msg) << fid;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void reportoff (int fid)
    {
        //report on
        ofxOscMessage msg;
        msg.setAddress("/object/off");
        OscPacker(msg) << fid;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }

    void report (DirectObject * obj)
    {
        ofxOscMessage msg;
        msg.setAddress("/object");
        OscPacker(msg) << (int)obj->f_id << obj->getX() << obj->getY() << obj->angle << obj->xspeed << obj->yspeed << obj->rspeed << obj->maccel << obj->raccel;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }

    void newObject(InputGestureDirectObjects::newObjectArgs & a)
    {
        DirectObject * obj = a.object;
        if(fiducials.find(obj->f_id)!= fiducials.end()){
            reporton(obj->f_id);
            fiducials[obj->f_id].angle = obj->angle;
        }
    }

    void updateObject(InputGestureDirectObjects::updateObjectArgs & a)
    {
        DirectObject * obj = a.object;
        if(fiducials.find(obj->f_id)!= fiducials.end()){

            fiducialstoupdate.insert(obj->f_id);

            //update data
            fiducials[obj->f_id].xpos = obj->getX();
            fiducials[obj->f_id].ypos = obj->getY();

            float increment = obj->angle - fiducials[obj->f_id].angle;
            if(increment > M_PI) increment = (obj->angle-2*PI)-fiducials[obj->f_id].angle;
            else if(increment < -M_PI)increment = (obj->angle+2*PI)-fiducials[obj->f_id].angle;
            fiducials[obj->f_id].angle_increment += increment;

            //std::cout << obj->angle << "  " << fiducials[obj->f_id].angle << "   " << fiducials[obj->f_id].angle_increment << endl;

            //update angle parameter
            fiducials[obj->f_id].a_value += increment/(M_PI*2);
            if(fiducials[obj->f_id].a_value >= 1) fiducials[obj->f_id].a_value = 1;
            else if(fiducials[obj->f_id].a_value<0)fiducials[obj->f_id].a_value = 0;

            ofxOscMessage msg;
            msg.setAddress("/object/angle_report");
            OscPacker(msg) << (int)obj->f_id << (float)fiducials[obj->f_id].a_value;
            OSCDispatcher::Instance().sender.sendMessage(msg);

            fiducials[obj->f_id].angle = obj->angle;
        }
    }

    void removeObject(InputGestureDirectObjects::removeObjectArgs & a)
    {
        DirectObject * obj = a.object;
        if(fiducials.find(obj->f_id)!= fiducials.end()){
            reportoff(obj->f_id);
        }
    }

    float computeAngle(DirectFinger * finger, DirectPoint &figure){
        DirectPoint center;
        center.set(0.5f,0.5f);
        float angle = (figure.getAngle(finger)-figure.getAngle(center));
        if (angle < 0) angle += PI*2;
        angle = angle /PI;
        if(angle >= 1.5f) angle = 0;
        else if(angle > 1) angle = 1;
        return angle;
    }

    void newCursor(InputGestureDirectFingers::newCursorArgs & a)
    {
        DirectFinger * finger = a.finger;
        for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it)
        {
            if(objects.isOnTable((*it).first) && (*it).second.can_cursors){
                DirectPoint figure;
                figure.set((*it).second.xpos,(*it).second.ypos);
                float dist = figure.getDistance(finger);
                if(dist <= 0.075){
                    (*it).second.f_value = computeAngle(finger,figure);
                    ofxOscMessage msg;
                    msg.setAddress("/object/finger_report");
                    OscPacker(msg) << (int)(*it).first << (float)fiducials[(*it).first].f_value;
                    OSCDispatcher::Instance().sender.sendMessage(msg);
                }
            }
        }
    }

    void updateCursor(InputGestureDirectFingers::updateCursorArgs & a)
    {
        DirectFinger * finger = a.finger;
        for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it)
        {
            if(objects.isOnTable((*it).first) && (*it).second.can_cursors){
                DirectPoint figure;
                figure.set((*it).second.xpos,(*it).second.ypos);
                float dist = figure.getDistance(finger);
                if(dist <= 0.075){
                    (*it).second.f_value = computeAngle(finger,figure);
                    ofxOscMessage msg;
                    msg.setAddress("/object/finger_report");
                    OscPacker(msg) << (int)(*it).first << (float)fiducials[(*it).first].f_value;
                    OSCDispatcher::Instance().sender.sendMessage(msg);
                }
            }
        }
    }

    void draw(){
        for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it)
        {
            int f = (*it).first;
            if(objects.isOnTable(f)){
               if((*it).second.can_angle)
                {
                    ofPushMatrix();
                    DirectPoint center;
                    center.set(0.5f,0.5f);
                    DirectPoint target;
                    target.set((*it).second.xpos,(*it).second.ypos);
                    float fig_angle = center.getAngle(target);
                    ofTranslate((*it).second.xpos,(*it).second.ypos);ofRotate(fig_angle*180/PI);
                    //empty
                    ofSetColor(55,55,55);
                    glBegin(GL_TRIANGLE_STRIP);
                    double step = PI/40.0;
                    for (double i = 0; i <= PI; i+=step)
                    {
                        glVertex2f(control_radius*cos(i),control_radius*sin(i));
                        glVertex2f((control_radius+bar_width)*cos(i),(control_radius+bar_width)*sin(i));
                    }
                    glEnd();

                    float angle = PI*(*it).second.a_value;
                    if(angle != 0){
                        //value
                        ofSetColor(255,255,255);
                        glBegin(GL_TRIANGLE_STRIP);
                        step = angle/40.0;
                        for (double i = 0; i <= angle; i+=step)
                        {
                            glVertex2f(control_radius*cos(i),control_radius*sin(i));
                            glVertex2f((control_radius+bar_width)*cos(i),(control_radius+bar_width)*sin(i));
                        }
                        glEnd();
                    }
                    ofPopMatrix();
                }
                if((*it).second.can_cursors){
                    ofPushMatrix();
                    DirectPoint center;
                    center.set(0.5f,0.5f);
                    DirectPoint target;
                    target.set((*it).second.xpos,(*it).second.ypos);
                    float fig_angle = center.getAngle(target);
                    ofTranslate((*it).second.xpos,(*it).second.ypos);
                    ofRotate((fig_angle+PI)*180/PI);
                    //empty
                    ofSetColor(155,255,155);
                    ofSetLineWidth(1);
                    glBegin(GL_LINE_STRIP);
                    double step = PI/40.0;
                    for (double i = 0; i <= PI; i+=step)
                    {
                        glVertex2f(bullet_radius*cos(i),bullet_radius*sin(i));
                    }
                    glEnd();

                    ofRotate((*it).second.f_value*180);
                    glTranslatef(bullet_radius,0.0f,0.0f);
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(0,0);
                    step = 2*PI/40.0;
                    for (double i = 0; i <= 2*PI; i+=step)
                    {
                        glVertex2f(bullet_width*cos(i),bullet_width*sin(i));
                    }
                    glVertex2f(bullet_width*cos(0),bullet_width*sin(0));
                    glEnd();
                    ofPopMatrix();
                }
            }
        }
    }
};

#endif // OSCREPORTER_HPP_INCLUDED
