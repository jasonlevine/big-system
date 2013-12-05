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
#include "ofxPostProcessing.h"

#pragma once

class scene {
    public:
    virtual void setup(audioAnalytics * _aa, openNIManager * _oni){}
    virtual void update(){}
    virtual void draw(int x, int y, int width, int height, bool drawToScreen){}
//    void setupGUI(){}
//    void guiEvent(ofxUIEventArgs &e){}
    virtual ofTexture & getTexRef(int width, int height){}
    virtual void toggleGUI(){}
    
    audioAnalytics * aa;
    openNIManager * oni;
    
    ofxPostProcessing post;
//    ofxUICanvas *gui;
};
