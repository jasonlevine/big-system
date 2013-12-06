//
//  gamePadManager.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-06.
//
//

#pragma once
#include "ofxGamepadHandler.h"
#include "squigglerScene.h"

class gamePadManager {
public:

    void setup(vector<scene*> &_scenes);
    
    void axisChanged(ofxGamepadAxisEvent &e);
    void buttonPressed(ofxGamepadButtonEvent &e);
    void buttonReleased(ofxGamepadButtonEvent &e);
    
    vector<scene*> scenes;
};
