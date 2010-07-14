#ifndef OSCREPORTER_HPP_INCLUDED
#define OSCREPORTER_HPP_INCLUDED

#include "dispatcher.hpp"
#include <set>
#include "simpleAllObjects.hpp"


class object_data{
    public:
        bool can_cursors, can_angle;
        float xpos,ypos,angle,f_value,a_value,angle_increment;
        object_data():can_cursors(false), can_angle(false),xpos(0),ypos(0),angle(0),f_value(0),a_value(0),angle_increment(0){}
};

class OscObjectReporter: public OSCCMD, public OnTable < tuio::CanDirectFingers < tuio::CanDirectObjects <Graphic> > >
{
    std::map<int,object_data> fiducials;
    SimpleAllObjects objects;
    public:
    OscObjectReporter():OSCCMD("/objects")
    {

    }
    void run(ofxOscMessage & m)
    {
        int fid,canangle,cancursor;
        OscOptionalUnpacker(m) >> fid >> canangle >> cancursor;
        fiducials[fid]=object_data();
        //fids.insert(fid);
        if(canangle == 3){ fiducials[fid].can_angle = true; std::cout << "o" << std::endl;}
        else if(cancursor == 3) fiducials[fid].can_cursors = true;
    }
    void update()
    {
        for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it)
        {
            int f = (*it).first;
            if(objects.isOnTable(f))
            {
//               report(objects[f]);
            }
        }
    }

    void report (tuio::DirectObject * obj)
    {
        ofxOscMessage msg;
        msg.setAddress("/object");
        OscPacker(msg) << (int)obj->f_id << obj->getX() << obj->getY() << obj->angle << obj->xspeed << obj->yspeed << obj->rspeed << obj->maccel << obj->raccel;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }

    void newObject(tuio::DirectObject * obj)
    {
        ofxOscMessage msg;
        msg.setAddress("/object/on");
        OscPacker(msg) << (int)obj->f_id;
        OSCDispatcher::Instance().sender.sendMessage(msg);
        if(fiducials.find(obj->f_id)!= fiducials.end()){
            fiducials[obj->f_id].angle = obj->angle;
        }
    }

    void updateObject(tuio::DirectObject * obj){
        if(fiducials.find(obj->f_id)!= fiducials.end()){
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
            msg.setAddress("/object/angle_repport");
            OscPacker(msg) << (int)obj->f_id << (float)fiducials[obj->f_id].a_value;
            OSCDispatcher::Instance().sender.sendMessage(msg);

            fiducials[obj->f_id].angle = obj->angle;
        }
    }

    void removeObject(tuio::DirectObject * obj)
    {
        ofxOscMessage msg;
        msg.setAddress("/object/off");
        OscPacker(msg) << (int)obj->f_id;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }

    void newCursor(tuio::DirectFinger * finger ){
    }

    void removeCursor(tuio::DirectFinger * finger ){
    }

    void updateCursor(tuio::DirectFinger * finger ){
    }

    void draw(){
        for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it)
        {
            int f = (*it).first;
            if(objects.isOnTable(f) && (*it).second.can_angle)
            {
               //report(objects[f]);
               float angle = PI*(*it).second.a_value;
               if(angle != 0){
                ofPushMatrix();
                DirectPoint center;
                center.set(0.5f,0.5f);
                DirectPoint target;
                target.set((*it).second.xpos,(*it).second.ypos);
                float fig_angle = center.getAngle(target);
                ofTranslate((*it).second.xpos,(*it).second.ypos);ofRotate(fig_angle*180/PI);
                ofSetColor(255,255,255);
                ofSetLineWidth(8);
                glBegin(GL_TRIANGLE_STRIP);
                double step = angle/20.0;
                for (double i = 0; i <= angle; i+=step)
                {
                    glVertex2f(0.05f*cos(i),0.05f*sin(i));
                    glVertex2f(0.056f*cos(i),0.056f*sin(i));
                }
                glEnd();
                ofPopMatrix();
               }
            }
        }
    }
};

#endif // OSCREPORTER_HPP_INCLUDED
