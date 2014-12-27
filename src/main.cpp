#include "ofMain.h"
#include "testApp.h"
#include "ofxGlobalConfig.hpp"


int main( )
{
    //OSX ONLY
    //uncomment this for the final bundled version, it will contain de data folder into the osx bundle
    //ofSetDataPathRoot("../Resources/data/");
    //END OSX ONLY

    int width = ofxGlobalConfig::getRef("PROGRAM:WIDTH", 1024);
    int height = ofxGlobalConfig::getRef("PROGRAM:HEIGHT", 768);
    if(ofxGlobalConfig::getRef("PROGRAM:FULLSCREEN", 0)){
        ofSetupOpenGL(width, height, OF_GAME_MODE);
    }else{
        ofSetupOpenGL(width, height, OF_WINDOW);
    }
    ofRunApp(new testApp());
}
