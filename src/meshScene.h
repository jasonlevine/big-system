//
//  meshScene.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-02.
//
//

#pragma once

#include "ofMain.h"
#include "scene.h"
#include "ofxUI.h"
#include "ofxPostProcessing.h"

class meshScene : public scene {
    
    void setup(audioAnalytics * _aa, openNIManager * _oni);
    void update();
    void draw(int width, int height);
    
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
    void toggleGUI();
    
    ofxPostProcessing post;
    vector<RenderPass::Ptr> renderPasses;
    
    ofCamera cam;
    
    //waves
    vector<vector<float> > waveHistory;
    int track, track2;
    float bassAccum, time;
    
    //UI variables
    ofxUICanvas *gui;
    
    float noiseScale;
    float meshRotateX;
    float camX, camY, camZ;
    float lookatX, lookatY, lookatZ;

    
    //postproc vars
    float waveStrength, noiseStrength;
    float fboBlend;
    
    bool drawPost;

    float hScale, colScale;
};
