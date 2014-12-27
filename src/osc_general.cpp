#include "osc_general.hpp"
#include "osc_polygon.hpp"
#include "osc_text.hpp"
#include "osc_line.hpp"

OscGeneral::OscGeneral() : OSCCMD("/mtcf"),
                            textsize(ofxGlobalConfig::getRef("PROGRAM:TEXTSIZE", 1.0f)){
    buildBackground();
}

void OscGeneral::buildBackground(){
    background.unregisterEvents();
    ofxOscMessage parms;

    // Layer
    parms.addIntArg(0);
    background.run("layer", parms);
    parms.clear();

    // Color
    parms.addIntArg(51);
    parms.addIntArg(102);
    parms.addIntArg(153);
    background.run("color", parms);
    parms.clear();

    // Geometry
    parms.addFloatArg(0.5f);
    parms.addFloatArg(0.5f);
    parms.addFloatArg(1.0f);
    parms.addFloatArg(1.0f);
    background.run("addrectangle", parms);
}

void OscGeneral::run(ofxOscMessage & m)
{
    static int & R = ofxGlobalConfig::getRef("FEEDBACK:CURSOR:COLOR:R",255);
    static int & G = ofxGlobalConfig::getRef("FEEDBACK:CURSOR:COLOR:G",0);
    static int & B = ofxGlobalConfig::getRef("FEEDBACK:CURSOR:COLOR:B",0);

    OscOptionalUnpacker msg(m);
    std::string cmd;
    msg >> cmd;
    if (cmd == "reset")
    {
        OscPolygonDraw::Instance().o.reset();
        OscTextDraw::Instance().o.reset();
        OscLineDraw::Instance().o.reset();
    }
    else if (cmd == "background")
    {
        msg >> cmd;
        background.run(cmd, msg);
    }
    else if (cmd == "fingercolor")
    {
        msg >> R >> G >> B;
    }
    else if (cmd == "textsize")
    {
        msg >> textsize;
    }
    else
    {
        std::cout << "/mtcf: command " << cmd << " not found" << std::endl;
    }

}

