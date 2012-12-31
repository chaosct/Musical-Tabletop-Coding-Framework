#include "osc_general.hpp"
#include "osc_polygon.hpp"
#include "osc_text.hpp"

void OscGlobalReset::run(ofxOscMessage & m)
{
    OscOptionalUnpacker msg(m);
    std::string cmd;
    msg >> cmd;
    if (cmd == "reset")
    {
        OscTextDraw::Instance().o.reset();
        OSCFigureDraw::Instance().o.reset();
    }
    else
    {
        std::cout << "/mtcf: command " << cmd << " not found" << std::endl;
    }

}
