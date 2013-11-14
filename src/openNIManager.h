//
//  openNIManager.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//

#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"

class openNIManager {
public:
    void setup();
    void update();
    void draw();

    void userEvent(ofxOpenNIUserEvent & event);
	ofxOpenNI openNIDevice;
};

    
    
