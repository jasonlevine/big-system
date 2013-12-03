//
//  scene.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//
#include "audioAnalytics.h"
#include "openNIManager.h"
#include "smoother.h"
#include "decayer.h"

#pragma once

class scene {
    public:
    virtual void setup(audioAnalytics * _aa, openNIManager * _oni){}
    virtual void update(){}
    virtual void draw(int width, int height){}
//    void setupGUI(){}
//    void guiEvent(ofxUIEventArgs &e){}
    virtual void toggleGUI(){}
    
    audioAnalytics * aa;
    openNIManager * oni;
//    ofxUICanvas *gui;
};
