//
//  squigglerScene.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-02.
//
//

#pragma once

#include "scene.h"
#include "squiggler.h"

//#include "ofxPostProcessing.h"



class squigglerScene : public scene {
public:
    void setup(audioAnalytics * _aa, openNIManager * _oni);
    void update();
    void draw(int x, int y, int width, int height, bool drawToScreen);
    
    void setupSquigglers(vector<int> &tracks);
    
    ofTexture & getTexRef(int width, int height);
    
    void setupGUI();
    void guiEvent(ofxUIEventArgs &e);
    void toggleGUI();
    
    ///vars
    
    vector<squiggler*> squigglers;
//    squiggler voxSquiggler, bgVoxSquiggler, kickSquiggler, bassSquiggler;
    
    ofFbo fbo;
    
    ofXml voxStats;
    
    
    ofxPostProcessing post;
    vector<RenderPass::Ptr> renderPasses;
    
    ofCamera cam;
    //UI variables
    ofxUICanvas *gui;
    
    int fadeAmt;
    bool useCam;
    float posX, posY, posZ;
    float lookatX, lookatY, lookatZ;
    float orientX, orientY, orientZ;
    bool usePost;
    bool flip;
    bool kaleidoscope, dof, bloom, highlight, godrays;
    
    ofxXmlSettings presets;


};