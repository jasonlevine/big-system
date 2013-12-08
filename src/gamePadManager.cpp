//
//  gamePadManager.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-06.
//
//

#include "gamePadManager.h"


void gamePadManager::setup(vector<scene*> &_scenes, colorSchemeDesigner *_colorScheme, projectionManager *_pm) {
    
    scenes = _scenes;
    colorScheme = _colorScheme;
    pm = _pm;
    
    ofxGamepadHandler::get()->enableHotplug();
    
    //CHECK IF THERE EVEN IS A GAMEPAD CONNECTED
    if(ofxGamepadHandler::get()->getNumPads()>0){
        ofxGamepad* pad = ofxGamepadHandler::get()->getGamepad(0);
        ofAddListener(pad->onAxisChanged, this, &gamePadManager::axisChanged);
        ofAddListener(pad->onButtonPressed, this, &gamePadManager::buttonPressed);
        ofAddListener(pad->onButtonReleased, this, &gamePadManager::buttonReleased);
    }
    
    smooth1.setNumPValues(10);
    smooth2.setNumPValues(10);
    smooth3.setNumPValues(10);
    smooth4.setNumPValues(10);
    
    decay1.setDecay(0.95);
    decay2.setDecay(0.95);
}



//--------------------------------------------------------------

void gamePadManager::axisChanged(ofxGamepadAxisEvent& e)
{
	cout << "AXIS " << e.axis << " VALUE " << ofToString(e.value) << endl;
    squigglerScene * squiggle0 = static_cast<squigglerScene*>(scenes[0]);
    squigglerScene * squiggle1 = static_cast<squigglerScene*>(scenes[1]);
    
    meshScene * mesh = static_cast<meshScene*>(scenes[2]);
    
    switch (e.axis) {
        case 0:
            decay1.addValue(ofMap(e.value, -1.0, 1.0, 100, 0));
            squiggle0->fadeAmt = decay1.getValue();
            squiggle1->fadeAmt = decay1.getValue();
            break;
            
        case 2:
            smooth1.addValue(ofMap(e.value, -1.0, 1.0, -90, 90));
            squiggle0->orientZ = smooth1.getMean();
            squiggle0->cam.setOrientation(ofVec3f(squiggle0->orientX, squiggle0->orientY, squiggle0->orientZ));
            
            squiggle1->orientZ = smooth1.getMean();
            squiggle1->cam.setOrientation(ofVec3f(squiggle1->orientX, squiggle1->orientY, squiggle1->orientZ));
            break;
            
        case 3:
            smooth2.addValue(ofMap(e.value, -1.0, 1.0, -90, 90));
            squiggle0->orientY = smooth2.getMean();
            squiggle0->cam.setOrientation(ofVec3f(squiggle0->orientX, squiggle0->orientY, squiggle0->orientZ));
            
            squiggle1->orientY = smooth2.getMean();            squiggle1->cam.setOrientation(ofVec3f(squiggle1->orientX, squiggle1->orientY, squiggle1->orientZ));
            break;
            
        case 4:
            smooth3.addValue(e.value / 100);
            colorScheme->hue += smooth3.getMean();
            if (colorScheme->hue > 1.0) colorScheme->hue -= 1.0;
            if (colorScheme->hue < 0.0) colorScheme->hue += 1.0;
            colorScheme->updateColorScheme();
            break;
            
        case 5:
            smooth4.addValue(e.value / 50);
            colorScheme->hue += smooth4.getMean();
            if (colorScheme->distance > 0.25) colorScheme->distance = 0.25;
            if (colorScheme->distance < 0.0) colorScheme->distance = 0.0;
            colorScheme->updateColorScheme();
            break;
            
        case 1:
            decay2.addValue(ofMap(e.value, -1.0, 1.0, 0.0, 10.0));
            mesh->colScale = decay2.getValue();
            break;
            
            
        
            
            
        
    }
}

void gamePadManager::buttonPressed(ofxGamepadButtonEvent& e)
{
	squigglerScene * squiggle0 = static_cast<squigglerScene*>(scenes[0]);
    squigglerScene * squiggle1 = static_cast<squigglerScene*>(scenes[1]);
    
    meshScene * mesh = static_cast<meshScene*>(scenes[2]);
    cout << "BUTTON " << e.button << " PRESSED" << endl;
    
    
    switch (e.button) {
        case 8:
            // pm->screens[0] >> "monitor"
            pm->screens[0].draw_scene_index += 1;
            pm->screens[0].draw_scene_index %= NUM_SCENES;
            break;
            
        case 9:
            // pm->screens[1] >> "proj_01"
            pm->screens[1].draw_scene_index += 1;
            pm->screens[1].draw_scene_index %= NUM_SCENES;
            break;
    }
}

void gamePadManager::buttonReleased(ofxGamepadButtonEvent& e)
{
	cout << "BUTTON " << e.button << " RELEASED" << endl;
}
