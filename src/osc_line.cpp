#include "osc_line.hpp"
#include "OscTools.hpp"
#include "GenericManager.hpp"
#include "GraphicDispatcher.hpp"
#include "Line.hpp"

OSCLineObject::OSCLineObject() : amplitude(0.1f) {
    GenericManager::get<GraphicDispatcher>()->createGraphic(line);
    line->setResolution(512);
}

OSCLineObject::~OSCLineObject(){
    GenericManager::get<GraphicDispatcher>()->removeGraphic(line);
}

void OSCLineObject::cmd_color(int r, int g, int b, int a){
    line->setColor(ofColor(r, g, b, a));
}

void OSCLineObject::cmd_hidden(bool hide){
    line->setVisible(!hide);
}

void OSCLineObject::cmd_report_matrix(){
    line->setMatrix(total_matrix);
}

void OSCLineObject::cmd_bring_top(){
    GenericManager::get<GraphicDispatcher>()->bringTop(line);
}

void OSCLineObject::cmd_set_layer(int layer){
    GenericManager::get<GraphicDispatcher>()->setLayer(layer, line);
}

void OSCLineObject::cmd_load_shader(std::string path)
{
    bool success = line->loadShader(path);
    if(!success){
        ofLogError("OSCLineObject::cmd_load_shader()") << "Failed, check shader name: " << path;
    }
}

void OSCLineObject::run_extra(const std::string& cmd, OscOptionalUnpacker& msg){
    if (cmd == "wav"){
        std::vector<float> waveValues;
        while(!msg.Eos()){
            float value;
            msg >> value;
            waveValues.push_back(value);
        }
        line->updateWaveform(waveValues, amplitude);
    }else if(cmd == "ampl"){
        float a(0);
        msg >> a;
        amplitude = a;
    }else if(cmd == "length"){
        float l(0);
        msg >> l;
        float half = l/2;
        line->setEndpoints(ofVec2f(-half, 0), ofVec2f(half, 0));
    }else if(cmd == "p1"){
        float x(0), y(0);
        msg >> x >> y;
        p1.set(x,y);
        line->setEndpoints(p1, p2);
    }else if(cmd == "p2"){
        float x(0), y(0);
        msg >> x >> y;
        p2.set(x,y);
        line->setEndpoints(p1, p2);
    }else if(cmd == "arc"){
        float a(0);
        msg >> a;
        line->setArc(a);
    }
}

