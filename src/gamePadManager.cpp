//
//  gamePadManager.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-06.
//
//

#include "gamePadManager.h"


void gamePadManager::setup(vector<scene*> &_scenes) {
    scenes = _scenes;
    
    ofxGamepadHandler::get()->enableHotplug();
    
    //CHECK IF THERE EVEN IS A GAMEPAD CONNECTED
    if(ofxGamepadHandler::get()->getNumPads()>0){
        ofxGamepad* pad = ofxGamepadHandler::get()->getGamepad(0);
        ofAddListener(pad->onAxisChanged, this, &gamePadManager::axisChanged);
        ofAddListener(pad->onButtonPressed, this, &gamePadManager::buttonPressed);
        ofAddListener(pad->onButtonReleased, this, &gamePadManager::buttonReleased);
    }
}



//--------------------------------------------------------------

void gamePadManager::axisChanged(ofxGamepadAxisEvent& e)
{
	cout << "AXIS " << e.axis << " VALUE " << ofToString(e.value) << endl;
    squigglerScene * squiggle = static_cast<squigglerScene*>(scenes[0]);
    switch (e.axis) {
        case 2:
            squiggle->orientZ = ofMap(e.value, -1.0, 1.0, -90, 90);
            squiggle->cam.setOrientation(ofVec3f(squiggle->orientX, squiggle->orientY, squiggle->orientZ));
            break;
            
        case 3:
            squiggle->orientY = ofMap(e.value, -1.0, 1.0, -90, 90);
            squiggle->cam.setOrientation(ofVec3f(squiggle->orientX, squiggle->orientY, squiggle->orientZ));
            break;
            
        case 1:
            squiggle->fadeAmt = ofMap(e.value, 0.0, 1.0, 100, 0);
            break;
    }
}

void gamePadManager::buttonPressed(ofxGamepadButtonEvent& e)
{
	cout << "BUTTON " << e.button << " PRESSED" << endl;
}

void gamePadManager::buttonReleased(ofxGamepadButtonEvent& e)
{
	cout << "BUTTON " << e.button << " RELEASED" << endl;
}
