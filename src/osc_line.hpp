#ifndef _OSC_LINE_HPP_
#define _OSC_LINE_HPP_

#include "osc_common_draw.hpp"
#include "dispatcher.hpp"
#include "Singleton.hpp"

namespace tableGraphics{
    class Line;
}

class OSCLineObject : public OSCCommonDrawObject{
    public:
        OSCLineObject();
        ~OSCLineObject();

        void cmd_color(int, int, int, int);
        void cmd_hidden(bool);
        void cmd_report_matrix();
        void cmd_bring_top();
        void cmd_set_layer(int);
        void cmd_load_shader(std::string);
        void run_extra(const std::string& cmd, OscOptionalUnpacker& msg);

    private:
        tableGraphics::Line* line;
        ofVec2f p1, p2;
        float amplitude;
};

class OscLineDraw : public Singleton<OscLineDraw>{
    public:
        OscLineDraw() : o("/line"){}
        OSCCommonDraw<OSCLineObject> o;
};

#endif // _OSC_LINE_HPP_

