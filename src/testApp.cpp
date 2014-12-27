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

#include "testApp.h"

#include "oscReporter.hpp"
#include "osc_general.hpp"
#include "osc_fingers.hpp"
#include "osc_polygon.hpp"
#include "osc_text.hpp"
#include "osc_line.hpp"

testApp::testApp(): tableapp(TableApp("MTCF"))
{
    
}

//--------------------------------------------------------------
void testApp::setup(){
    tableapp.setup();
    //new CalibratorObject(1);

    //Global
    new OscGeneral();



    OscPolygonDraw::Instance();
    OscTextDraw::Instance();
    OscLineDraw::Instance();
}

//--------------------------------------------------------------
void testApp::update(){

    objects.update();
    fingers.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    tableapp.draw();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
