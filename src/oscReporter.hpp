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
        OscOptionalUnpacker(m) >> fid1 >> fid2;
        dists.insert(std::make_pair(fid1,fid2));
        //report initial state
        if(objects.isOnTable(fid1) and objects.isOnTable(fid1))
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
