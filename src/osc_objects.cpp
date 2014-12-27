#include "osc_objects.hpp"

OscObjectReporter::OscObjectReporter() : OSCCMD("/objects"){
    this->registerEvent(InputGestureDirectObjects::newObject,&OscObjectReporter::newObject);
    this->registerEvent(InputGestureDirectObjects::removeObject,&OscObjectReporter::removeObject);
    this->registerEvent(InputGestureDirectObjects::updateObject,&OscObjectReporter::updateObject);
    this->registerEvent(InputGestureDirectFingers::newCursor, &OscObjectReporter::newCursor);
    //this->registerEvent(InputGestureDirectFingers::removeCursor, &OscObjectReporter::removeCursor);
    this->registerEvent(InputGestureDirectFingers::updateCursor, &OscObjectReporter::updateCursor);
}

void OscObjectReporter::run(ofxOscMessage & m){
    int fid;
    std::string cmd;
    OscOptionalUnpacker msg(m);
    msg >> fid >> cmd;
    if (cmd == "fvalue"){
        float value = 0;
        msg >> value;
        fiducials[fid].graphics.setValue(ObjectGraphs::SLIDER, value);
    }else if (cmd == "config"){
        int deleteme = 0;
        int canangle = 0;
        int cancursor = 0;
        msg >> canangle >> cancursor >> deleteme;
        if(deleteme){
            fiducials.erase(fid);
            return;
        }
        fiducials[fid].can_angle   = (canangle != 0);
        fiducials[fid].can_cursors = (cancursor != 0);
    }else if (cmd == "color"){
        ObjectGraphs::Type selector = ObjectGraphs::SLIDER;
        msg >> cmd;
        if (cmd == "slider"){
            selector = ObjectGraphs::SLIDER;
        }else if (cmd == "angle"){
            selector = ObjectGraphs::ANGLE;
        }
        int r, g, b, a;
        r = g = b = a = 0;
        msg >> r >> g >> b >> a;
        fiducials[fid].graphics.setColor(selector, ofColor(r, g, b, a));
    }
    if (objects.isOnTable(fid)){
        reporton(fid);
        fiducials[fid].graphics.setVisible(ObjectGraphs::ANGLE, fiducials[fid].can_angle);
        fiducials[fid].graphics.setVisible(ObjectGraphs::SLIDER, fiducials[fid].can_cursors);
    }else{
        reportoff(fid);
        fiducials[fid].graphics.setVisible(ObjectGraphs::ANGLE, false);
        fiducials[fid].graphics.setVisible(ObjectGraphs::SLIDER, false);
    }
}

void OscObjectReporter::update(){
    std::set< int >::iterator it;
    for(it = fiducialstoupdate.begin();it != fiducialstoupdate.end(); ++it){
        const int & f = *it;
        if(objects.isOnTable(f))report(objects[f]);
    }
    fiducialstoupdate.clear();
}

void OscObjectReporter::report(DirectObject * obj){
    ofxOscMessage msg;
    msg.setAddress("/object");
    OscPacker(msg) << (int)obj->f_id
        << obj->x
        << obj->y
        << obj->orientation
        << obj->xspeed
        << obj->yspeed
        << obj->rspeed
        << obj->maccel
        << obj->raccel;
    OSCDispatcher::Instance().sender.sendMessage(msg);
}

void OscObjectReporter::reporton(int fid){
    //report on
    ofxOscMessage msg;
    msg.setAddress("/object/on");
    OscPacker(msg) << fid;
    OSCDispatcher::Instance().sender.sendMessage(msg);
}

void OscObjectReporter::reportoff(int fid){
    //report on
    ofxOscMessage msg;
    msg.setAddress("/object/off");
    OscPacker(msg) << fid;
    OSCDispatcher::Instance().sender.sendMessage(msg);
}

void OscObjectReporter::newObject(InputGestureDirectObjects::newObjectArgs & a){
    DirectObject * obj = a.object;
    if(fiducials.find(obj->f_id) != fiducials.end()){
        reporton(obj->f_id);
        object_data& object = fiducials[obj->f_id];
        object.angle = obj->orientation;
        object.graphics.setPosition(ofVec3f(obj->x, obj->y, 0));
        object.graphics.setVisible(ObjectGraphs::SLIDER, object.can_cursors);
        object.graphics.setVisible(ObjectGraphs::ANGLE, object.can_angle);
    }
}

void OscObjectReporter::updateObject(InputGestureDirectObjects::updateObjectArgs & a){
    DirectObject * obj = a.object;
    if(fiducials.find(obj->f_id)!= fiducials.end()){

        fiducialstoupdate.insert(obj->f_id);

        // Update angle parameter
        float increment = obj->orientation - fiducials[obj->f_id].angle;
        if (increment > PI){
            increment -= TWO_PI;
        }else if (increment < -PI){
            increment += TWO_PI;
        }
        float angle = fiducials[obj->f_id].graphics.getValue(ObjectGraphs::ANGLE);
        angle += increment / TWO_PI;
        if(angle > 1){
            angle = 1.0f;
        }else if(angle < 0){
            angle = 0.0f;
        }
        fiducials[obj->f_id].graphics.setValue(ObjectGraphs::ANGLE, angle);

        // Update position
        fiducials[obj->f_id].graphics.setPosition(ofVec3f(obj->x, obj->y, 0));
        fiducials[obj->f_id].angle = obj->orientation;

        ofxOscMessage msg;
        msg.setAddress("/object/angle_report");
        OscPacker(msg) << (int)obj->f_id << angle;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
}

void OscObjectReporter::removeObject(InputGestureDirectObjects::removeObjectArgs & a){
    DirectObject * obj = a.object;
    if(fiducials.find(obj->f_id)!= fiducials.end()){
        reportoff(obj->f_id);
        fiducials[obj->f_id].graphics.setVisible(ObjectGraphs::SLIDER, false);
        fiducials[obj->f_id].graphics.setVisible(ObjectGraphs::ANGLE, false);
    }
}

void OscObjectReporter::newCursor(InputGestureDirectFingers::newCursorArgs & a){
    updateCursor(a);
}

void OscObjectReporter::updateCursor(InputGestureDirectFingers::updateCursorArgs & a){
    DirectFinger * finger = a.finger;
    for (std::map<int,object_data>::iterator it = fiducials.begin(); it != fiducials.end(); ++it){
        if(objects.isOnTable((*it).first) && (*it).second.can_cursors){
            ofVec2f figure(it->second.graphics.getPosition());
            float dist = figure.distance(*finger);
            if(dist <= 0.075){

                // Finger is close enough to object, update slider value.
                float value = computeAngle(finger, figure);
                it->second.graphics.setValue(ObjectGraphs::SLIDER, value);

                ofxOscMessage msg;
                msg.setAddress("/object/finger_report");
                OscPacker(msg) << it->first << value;
                OSCDispatcher::Instance().sender.sendMessage(msg);
            }
        }
    }
}

float OscObjectReporter::computeAngle(DirectFinger * finger, ofVec2f &figure){
    const ofVec2f center(0.5f, 0.5f);
    float angle = atan2(figure.y - finger->y, figure.x - finger->x) -
                  atan2(figure.y - center.y, figure.x - center.x);
    if (angle < 0){
        angle += TWO_PI;
    }
    angle /= PI;
    if(angle >= 1.5f){
        angle = 0;
    }else if(angle > 1){
        angle = 1;
    }
    return angle;
}

