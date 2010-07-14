#ifndef OSCREPORTER_HPP_INCLUDED
#define OSCREPORTER_HPP_INCLUDED

#include "dispatcher.hpp"
#include <set>
#include <map>
#include "simpleAllObjects.hpp"
#include <boost/foreach.hpp>

class OscDistanceReporter: public OSCCMD, public OnTable < tuio::CanDirectObjects <Graphic> >
{
    std::set< std::pair<int,int> > dists;
    std::set< std::pair<int,int> > diststoupdate;
    SimpleAllObjects objects;
    public:
    OscDistanceReporter():OSCCMD("/distance")
    {

    }
    void run(ofxOscMessage & m)
    {
        int fid1,fid2;
        int destroy = 0;
        OscOptionalUnpacker(m) >> fid1 >> fid2 >> destroy;
        if (destroy)
        {
            dists.erase(std::make_pair(fid1,fid2));
            return;
        }
        dists.insert(std::make_pair(fid1,fid2));
        //report initial state
        if(objects.isOnTable(fid1) and objects.isOnTable(fid2))
        {
            reporton(fid1,fid2);
            report(fid1,fid2);
        }
        else
        {
            reportoff(fid1,fid2);
        }
    }
    void draw()
    {
        std::pair<int,int> p;
        BOOST_FOREACH( p , dists)
        {
            if( objects.isOnTable(p.second) and objects.isOnTable(p.first))
            {
                if(diststoupdate.find(p) == diststoupdate.end() )
                {
                    ofSetColor(255,255,255);
                }
                else
                {
                    ofSetColor(255,0,0);
                    report(p.first,p.second);
                }

                ofLine(objects[p.first]->getX(),objects[p.first]->getY(),objects[p.second]->getX(),objects[p.second]->getY());
            }
        }
        diststoupdate.clear();
    }

    void reporton (int fid1, int fid2)
    {
        //report on
        ofxOscMessage msg;
        msg.setAddress("/distance/on");
        OscPacker(msg) << fid1 << fid2;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void reportoff (int fid1, int fid2)
    {
        //report on
        ofxOscMessage msg;
        msg.setAddress("/distance/off");
        OscPacker(msg) << fid1 << fid2;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
    void report (int fid1, int fid2)
    {
        //report dist, angle
        ofxOscMessage msg;
        msg.setAddress("/distance");
        OscPacker(msg) <<  fid1 << fid2 << objects[fid1]->getDistance(objects[fid2]) << objects[fid1]->getAngle(objects[fid2]);
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }

    void newObject(tuio::DirectObject * obj)
    {
        std::pair<int,int> p;
        BOOST_FOREACH( p , dists)
        {
            if(   (p.first == obj->f_id and objects.isOnTable(p.second))
               or (p.second == obj->f_id and objects.isOnTable(p.first))
               )
            {
                reporton(p.first,p.second);
                diststoupdate.insert(std::make_pair(p.first,p.second));
                //report(p.first,p.second);
            }
        }
    }
    void removeObject(tuio::DirectObject * obj)
    {
        std::pair<int,int> p;
        BOOST_FOREACH(p , dists)
        {
            if(   (p.first == obj->f_id and objects.isOnTable(p.second))
               or (p.second == obj->f_id and objects.isOnTable(p.first))
               )
            {
                reportoff(p.first,p.second);
            }
        }
    }
    void updateObject(tuio::DirectObject * obj)
    {
        std::pair<int,int> p;
        BOOST_FOREACH( p , dists)
        {
            if(   (p.first == obj->f_id and objects.isOnTable(p.second))
               or (p.second == obj->f_id and objects.isOnTable(p.first))
               )
            {
                diststoupdate.insert(std::make_pair(p.first,p.second));
            }
        }
    }
};


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
        int fid;
        int canangle = 0;
        int cancursor = 0;
        OscOptionalUnpacker(m) >> fid >> canangle >> cancursor;
        fiducials[fid].can_angle   = (canangle != 0);
        fiducials[fid].can_cursors = (cancursor != 0);
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
            msg.setAddress("/object/angle_report");
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
        for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it)
        {
            if(objects.isOnTable((*it).first) && (*it).second.can_cursors){
                DirectPoint figure;
                figure.set((*it).second.xpos,(*it).second.ypos);
                float dist = figure.getDistance(finger);
                if(dist <= 0.075){
                    DirectPoint center;
                    center.set(0.5f,0.5f);
                    float angle = (figure.getAngle(finger)-figure.getAngle(center))/PI;
                    if (angle >1) angle = 1;
                    else if(angle <0) angle = 0;
                    (*it).second.f_value = angle;
                    ofxOscMessage msg;
                    msg.setAddress("/object/finger_report");
                    OscPacker(msg) << (int)(*it).first << (float)fiducials[(*it).first].f_value;
                    OSCDispatcher::Instance().sender.sendMessage(msg);
                }
            }
        }
    }

    void updateCursor(tuio::DirectFinger * finger ){
        for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it)
        {
            if(objects.isOnTable((*it).first) && (*it).second.can_cursors){
                DirectPoint figure;
                figure.set((*it).second.xpos,(*it).second.ypos);
                float dist = figure.getDistance(finger);
                if(dist <= 0.075){
                    DirectPoint center;
                    center.set(0.5f,0.5f);
                    float fig_angle = figure.getAngle(center);
                    float angle = (figure.getAngle(finger)-figure.getAngle(center))/PI;
                    if (angle >1) angle = 1;
                    else if(angle <0) angle = 0;
                    (*it).second.f_value = angle;
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
                        glVertex2f(0.05*cos(i),0.05*sin(i));
                        glVertex2f(0.056*cos(i),0.056*sin(i));
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
                            glVertex2f(0.05*cos(i),0.05*sin(i));
                            glVertex2f(0.056*cos(i),0.056*sin(i));
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
                        glVertex2f(0.053*cos(i),0.053*sin(i));
                    }
                    glEnd();

                    ofRotate((*it).second.f_value*180);
                    glTranslatef(0.053f,0.0f,0.0f);
                    glBegin(GL_TRIANGLE_FAN);
                    glVertex2f(0,0);
                    step = 2*PI/40.0;
                    for (double i = 0; i <= 2*PI; i+=step)
                    {
                        glVertex2f(0.0053*cos(i),0.0053*sin(i));
                    }
                    glVertex2f(0.0053*cos(0),0.0053*sin(0));
                    glEnd();
                    ofPopMatrix();
                }
            }
        }
    }
};

#endif // OSCREPORTER_HPP_INCLUDED
