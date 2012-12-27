/*

    Musical Tabletop Coding Framework

    Copyright (c) 2012 Carles F. Julià <carles.fernandez@upf.edu>
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

#pragma once
#include "dispatcher.hpp"
#include "OscTools.hpp"
#include <set>
#include <map>
#include "InputGestureDirectObjects.hpp"

class OscDistanceReporter: public OSCCMD, public Graphic
{
    typedef std::set< std::pair<int,int> > distsType;
    distsType dists;
    std::set< std::pair<int,int> > diststoupdate;
    struct properties
    {
        bool visible;
        properties():visible(true){}
    };
    std::map< std::pair<int,int>, properties > listproperties;
    SimpleAllObjects objects;
    public:
    OscDistanceReporter():OSCCMD("/distance")
    {
        this->registerEvent(InputGestureDirectObjects::I().newObject,&OscDistanceReporter::newObject);
        this->registerEvent(InputGestureDirectObjects::I().removeObject,&OscDistanceReporter::removeObject);
        this->registerEvent(InputGestureDirectObjects::I().updateObject,&OscDistanceReporter::updateObject);
    }
    void run(ofxOscMessage & m)
    {
        int fid1,fid2,draw;
        std::string command;
        OscOptionalUnpacker(m) >> fid1 >> fid2 >> command >> draw;
        if (command == "rm")
        {
            dists.erase(std::make_pair(fid1,fid2));
            return;
        }
        if(command == "draw")
        {
            listproperties[std::make_pair(fid1,fid2)].visible = draw;
        }
        ///else
        dists.insert(std::make_pair(fid1,fid2));
        ///report initial state
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
        for(distsType::iterator it = dists.begin(); it != dists.end(); ++it)
        {
            const std::pair<int,int> & p = *it;
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
                if (listproperties[p].visible)
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

    void newObject(InputGestureDirectObjects::newObjectArgs & a)
    {
        DirectObject * obj = a.object;
        for(distsType::iterator it = dists.begin(); it != dists.end(); ++it)
        {
            const std::pair<int,int> & p = *it;
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
    void removeObject(InputGestureDirectObjects::removeObjectArgs & a)
    {
        DirectObject * obj = a.object;
        for(distsType::iterator it = dists.begin(); it != dists.end(); ++it)
        {
            const std::pair<int,int> & p = *it;
            if(   (p.first == obj->f_id and objects.isOnTable(p.second))
               or (p.second == obj->f_id and objects.isOnTable(p.first))
               )
            {
                reportoff(p.first,p.second);
            }
        }
    }
    void updateObject(InputGestureDirectObjects::updateObjectArgs & a)
    {
        DirectObject * obj = a.object;
        //std::pair<int,int> p;
        //BOOST_FOREACH( p , dists)
        for(distsType::iterator it = dists.begin(); it != dists.end(); ++it)
        {
            const std::pair<int,int> & p = *it;
            if(   (p.first == obj->f_id and objects.isOnTable(p.second))
               or (p.second == obj->f_id and objects.isOnTable(p.first))
               )
            {
                diststoupdate.insert(std::make_pair(p.first,p.second));
            }
        }
    }
};