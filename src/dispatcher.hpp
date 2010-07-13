#ifndef DISPATCHER_HPP_INCLUDED
#define DISPATCHER_HPP_INCLUDED


#include "Singleton.hpp"
#include "ofxOsc.h"
#include "Graphic.hpp"
#include "GlobalConfig.hpp"
#include <map>


class OSCCMD
{
    public:
    std::string addr;
    OSCCMD(const std::string & addr);
    virtual void run(ofxOscMessage & m)=0;
};

class OSCDispatcher: public Singleton<OSCDispatcher,Graphic>
{
    ofxOscReceiver receiver;
    std::map<std::string,OSCCMD*> commands;
    public:

    ofxOscSender sender;
    OSCDispatcher();
    void update();
    void addCommand(OSCCMD * cmd);
};

class OscOptionalUnpacker
{
    ofxOscMessage & msg;
    int n;
    public:
    OscOptionalUnpacker(ofxOscMessage & m):msg(m),n(0){}
    OscOptionalUnpacker & operator >> (int & i)
    {
        if(n < msg.getNumArgs())
        {
            i = msg.getArgAsInt32( n++ );
        }
        return *this;
    }
    OscOptionalUnpacker & operator >> (float & i)
    {
        if(n < msg.getNumArgs())
        {
            i = msg.getArgAsFloat( n++ );
        }
        return *this;
    }
    OscOptionalUnpacker & operator >> (double & i)
    {
        if(n < msg.getNumArgs())
        {
            i = msg.getArgAsFloat( n++ );
        }
        return *this;
    }
    OscOptionalUnpacker & operator >> (std::string & i)
    {
        if(n < msg.getNumArgs())
        {
            i = msg.getArgAsString( n++ );
        }
        return *this;
    }
};

class OscPacker
{
    ofxOscMessage & msg;
    public:
    OscPacker(ofxOscMessage & m):msg(m){}
    OscPacker & operator << (int i)
    {
        msg.addIntArg(i);
        return *this;
    }
    OscPacker & operator << (unsigned int i)
    {
        msg.addIntArg(i);
        return *this;
    }
    OscPacker & operator << (float i)
    {
        msg.addFloatArg(i);
        return *this;
    }
    OscPacker & operator << (const std::string & i)
    {
        msg.addStringArg(i);
        return *this;
    }
};

#endif // DISPATCHER_HPP_INCLUDED
