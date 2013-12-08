//
//  gamePadManager.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-06.
//
//

#pragma once
#include "ofxGamepadHandler.h"
#include "projectionManager.h"
#include "squigglerScene.h"
#include "meshScene.h"
#include "colorSchemeDesigner.h"
#include "smoother.h"
#include "decayer.h"

class gamePadManager {
public:

    void setup(vector<scene*> &_scenes, colorSchemeDesigner *_colorScheme, projectionManager *_pm);
    void update();
    
    void axisChanged(ofxGamepadAxisEvent &e);
    void buttonPressed(ofxGamepadButtonEvent &e);
    void buttonReleased(ofxGamepadButtonEvent &e);
    
    vector<scene*> scenes;
    colorSchemeDesigner *colorScheme;
    projectionManager *pm;
    
    smoother smooth1, smooth2, smooth3, smooth4;
    decayer decay1, decay2;
};
