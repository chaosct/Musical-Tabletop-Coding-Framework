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

#include "dispatcher.hpp"

OSCCMD::OSCCMD(const std::string & Addr):addr(Addr)
{
    OSCDispatcher::Instance().addCommand(this);
}


OSCDispatcher::OSCDispatcher():remoteIp(GlobalConfig::getRef<std::string>("OSCDISPATCHER:SENDER:ADDR","localhost")),remotePort(GlobalConfig::getRef("OSCDISPATCHER:SENDER:PORT",1235))
{
    receiver.setup(GlobalConfig::getRef("OSCDISPATCHER:RECEIVER:PORT",1234));
    sender.setup(remoteIp,remotePort);
    ofxOscMessage msg;
    msg.setAddress("/init");
    sender.sendMessage(msg);
}

void OSCDispatcher::update()
{
    while (receiver.hasWaitingMessages())
    {
        ofxOscMessage message;
        receiver.getNextMessage(&message);
        std::string addr = message.getAddress();
        if(commands.find(addr) != commands.end())
            commands[addr]->run(message);


        //we update the IP addr of the PD client
        if (message.getRemoteIp() != remoteIp)
            sender.setup(remoteIp,remotePort);
    }
}

void OSCDispatcher::addCommand(OSCCMD * cmd)
{
    commands[cmd->addr]=cmd;
}
