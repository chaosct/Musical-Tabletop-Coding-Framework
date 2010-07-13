#ifndef OSCREPORTER_HPP_INCLUDED
#define OSCREPORTER_HPP_INCLUDED

#include "dispatcher.hpp"
#include <set>
#include "simpleAllObjects.hpp"

class OscObjectReporter: public OSCCMD, public OnTable < tuio::CanDirectObjects <Graphic> >
{
    std::set<int> fids;
    SimpleAllObjects objects;
    public:
    OscObjectReporter():OSCCMD("/objects")
    {

    }
    void run(ofxOscMessage & m)
    {
        int fid;
        OscOptionalUnpacker(m) >> fid;
        fids.insert(fid);
    }
    void update()
    {
        for (std::set<int>::iterator it = fids.begin(); it != fids.end(); ++it)
        {
            int f = *it;
            if(objects.isOnTable(f))
            {
               report(objects[f]);
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
    }
    void removeObject(tuio::DirectObject * obj)
    {
        ofxOscMessage msg;
        msg.setAddress("/object/off");
        OscPacker(msg) << (int)obj->f_id;
        OSCDispatcher::Instance().sender.sendMessage(msg);
    }
};

#endif // OSCREPORTER_HPP_INCLUDED
