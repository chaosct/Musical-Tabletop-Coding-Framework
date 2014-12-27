#include "objectGraphics.hpp"
#include "GenericManager.hpp"
#include "GraphicDispatcher.hpp"
#include "Polygon.hpp"
#include "Line.hpp"

ObjectGraphs::ObjectGraphs() : radius(ofxGlobalConfig::getRef("FIGURE:BULLETRADIUS", 0.07f)),
                            sliderSize(ofxGlobalConfig::getRef("FIGURE:BULLETWIDTH", 0.01f)),
                            angleSize(ofxGlobalConfig::getRef("FIGURE:BARWIDTH", 0.01f)){
    GenericManager::get<GraphicDispatcher>()->createGraphic(angle.back);
    GenericManager::get<GraphicDispatcher>()->createGraphic(angle.front);
    GenericManager::get<GraphicDispatcher>()->createGraphic(slider.line);
    GenericManager::get<GraphicDispatcher>()->createGraphic(slider.circle);
    GenericManager::get<GraphicDispatcher>()->setLayer(NOT_LAYER, angle.back);
    GenericManager::get<GraphicDispatcher>()->setLayer(NOT_LAYER, angle.front);
    GenericManager::get<GraphicDispatcher>()->setLayer(NOT_LAYER, slider.line);
    GenericManager::get<GraphicDispatcher>()->setLayer(NOT_LAYER, slider.circle);

    slider.line->setEndpoints(ofVec2f(-radius, 0), ofVec2f(radius, 0));
    slider.line->setResolution(60);
    slider.line->setArc(100);
    slider.circle->circle(sliderSize, 30);

    genArc(angle.back, 0, PI, radius - angleSize/2, radius + angleSize/2);
    angle.back->setColor(ofColor(55, 55, 55));
    angle.front->setColor(ofColor(255, 255, 255));
    genArc(angle.front, 0, PI, radius - angleSize/2, radius + angleSize/2);
}

ObjectGraphs::~ObjectGraphs(){
    GenericManager::get<GraphicDispatcher>()->removeGraphic(slider.circle);
    GenericManager::get<GraphicDispatcher>()->removeGraphic(slider.line);
    GenericManager::get<GraphicDispatcher>()->removeGraphic(angle.back);
    GenericManager::get<GraphicDispatcher>()->removeGraphic(angle.front);
}

void ObjectGraphs::setPosition(const ofVec3f& pos){
    position.set(pos);
    update();
}

ofVec2f ObjectGraphs::getPosition(){
    return position;
}

void ObjectGraphs::setVisible(Type t, bool visible){
    switch(t){
    case SLIDER:
        slider.circle->setVisible(visible);
        slider.line->setVisible(visible);
        break;
    case ANGLE:
        angle.back->setVisible(visible);
        angle.front->setVisible(visible);
        break;
    default:
        break;
    }
}

void ObjectGraphs::setValue(Type t, float val){
    val = val < 0 ? 0 : (val > 1 ? 1 : val); // [0-1] clamp
    switch(t){
    case SLIDER:
        slider.value = val;
        break;
    case ANGLE:
        angle.value = val;
        break;
    default:
        break;
    }
    update();
}

float ObjectGraphs::getValue(Type t){
    switch(t){
    case SLIDER:
        return slider.value;
        break;
    case ANGLE:
        return angle.value;
        break;
    default:
        return 0.0f;
        break;
    }
}

void ObjectGraphs::setColor(Type t, const ofColor& color){
    switch(t){
    case SLIDER:
        slider.circle->setColor(color);
        break;
    case ANGLE:
        angle.front->setColor(color);
        break;
    default:
        break;
    }
}

void ObjectGraphs::update(){
    // Global position
    ofMatrix4x4 m;
    float rotate = atan2(position.y - 0.5f, position.x - 0.5f) * 180/PI;
    m.rotate(rotate, 0, 0, 1);
    m.translate(position);
    slider.line->setMatrix(m);
    angle.back->setMatrix(m);
    angle.front->setMatrix(m);

    // Slider value
    m.makeIdentityMatrix();
    m.translate(-radius, 0, 0);
    rotate += slider.value * 180;
    m.rotate(rotate, 0, 0, 1);
    m.translate(position);
    slider.circle->setMatrix(m);

    // Angle value
    genArc(angle.front, 0, angle.value * PI, radius - angleSize/2, radius + angleSize/2);
}

void ObjectGraphs::genArc(tableGraphics::Polygon* polygon,
                            float angle1, float angle2,
                            float radius1, float radius2){
    polygon->clear();
    const int resolution = 30;
    float incr = (angle2 - angle1) / resolution;
    for(float angle = angle1; angle < angle2; angle += incr){
        polygon->addVertex(ofPoint(cos(angle)*radius1, sin(angle)*radius1));
    }
    polygon->addVertex(ofPoint(cos(angle2)*radius1, sin(angle2)*radius1));
    for(float angle = angle2; angle > angle1; angle -= incr){
        polygon->addVertex(ofPoint(cos(angle)*radius2, sin(angle)*radius2));
    }
    polygon->addVertex(ofPoint(cos(angle1)*radius2, sin(angle1)*radius2));
}

