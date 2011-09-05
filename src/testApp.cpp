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
#include "CursorFeedback.hpp"
#include "FigureFeedback.hpp"
#include "TapFeedback.hpp"

#include "oscReporter.hpp"

//--------------------------------------------------------------
void testApp::setup(){
    tableapp.setup();
    new CursorFeedback();
    new FigureFeedback();
    new TapFeedback();
    //new CalibratorObject(1);

    new OscObjectReporter();
    new OscDistanceReporter();
    new OscWaveDraw();
    new OscBGChanger();
    new OscFingerReporter();
    new OscFingerColor();
}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    tableapp.draw();
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}
