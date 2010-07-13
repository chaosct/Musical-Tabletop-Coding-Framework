#include "dispatcher.hpp"

OSCCMD::OSCCMD(const std::string & Addr):addr(Addr)
{
    OSCDispatcher::Instance().addCommand(this);
}


OSCDispatcher::OSCDispatcher()
{
    receiver.setup(GlobalConfig::getRef("OSCDISPATCHER::RECEIVER::PORT",1234));
    sender.setup(GlobalConfig::getRef<std::string>("OSCDISPATCHER::SENDER::ADDR","localhost"),GlobalConfig::getRef("OSCDISPATCHER::SENDER::PORT",1235));
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
    }
}

void OSCDispatcher::addCommand(OSCCMD * cmd)
{
    commands[cmd->addr]=cmd;
}
