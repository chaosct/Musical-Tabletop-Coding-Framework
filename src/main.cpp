#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"
#include "ofxGlobalConfig.hpp"


int main( )
{
    //OSX ONLY
    //uncomment this for the final bundled version, it will contain de data folder into the osx bundle
    //ofSetDataPathRoot("../Resources/data/");
    //END OSX ONLY
    ofAppGlutWindow window;
    int width = ofxGlobalConfig::getRef("PROGRAM:WIDTH",1024);
    int height = ofxGlobalConfig::getRef("PROGRAM:HEIGHT",768);
    if(ofxGlobalConfig::getRef("PROGRAM:FULLSCREEN",0))
        ofSetupOpenGL(&window, width ,height, OF_GAME_MODE);
    else
        ofSetupOpenGL(&window, width ,height, OF_WINDOW);
	ofRunApp( new testApp());
}
