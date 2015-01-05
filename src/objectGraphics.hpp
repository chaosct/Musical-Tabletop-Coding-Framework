#ifndef _OBJECTGRAPHICS_H_
#define _OBJECTGRAPHICS_H_

#include "ofxGlobalConfig.hpp"

namespace tableGraphics{
    class Line;
    class Polygon;
}

class ObjectGraphs{
    public:
        ObjectGraphs();
        ~ObjectGraphs();

        void setPosition(const ofVec3f&);
        ofVec2f getPosition();

        enum Type{ SLIDER, ANGLE };
        typedef Type Type;
        void setVisible(Type, bool);
        void setValue(Type, float);
        float getValue(Type);
        void setColor(Type, const ofColor&);

    private:
        ofVec2f position;

        struct Slider{
            tableGraphics::Line* line;
            tableGraphics::Polygon* circle;
            float value;
            Slider() : value(0){}
        } slider;

        struct Angle{
            tableGraphics::Polygon* back;
            tableGraphics::Polygon* front;
            float value;
            Angle() : value(0){}
        } angle;

        float radius;
        float sliderSize;
        float angleSize;

        void update();
        void genArc(tableGraphics::Polygon*, float a1, float a2, float r1, float r2);
};

#endif // _OBJECTGRAPHICS_H_

