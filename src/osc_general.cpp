#include "osc_general.hpp"
#include "osc_polygon.hpp"
#include "osc_text.hpp"
#include "oscReporter.hpp"

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
        OscTextDraw::Instance().o.reset();
        OSCFigureDraw::Instance().o.reset();
        OscWaveDraw::Instance().reset();
    }
    else if (cmd == "background")
    {
        msg >> color1 >> color2 >> color3;
        if (color1 < 0) color1 = 0;
        if (color2 < 0) color2 = 0;
        if (color3 < 0) color3 = 0;
        if (color1 > 255) color1 = 255;
        if (color2 > 255) color2 = 255;
        if (color3 > 255) color3 = 255;
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

void OscGeneral::draw()
{
    ofSetColor(color1,color2,color3);
    ofRect(0.0f,0.0f,1.0f,1.0f);
}