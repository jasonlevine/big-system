//
//  renderManager.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//

#pragma once

#include "ofMain.h"
#include "openNIManager.h"
#include "scene.h"


class renderManager {
    public:
    void setup(openNIManager &_oni, vector<scene*> &_scenes);
    void update();
    void draw(int wallScene, int bodyScene);
    void drawStage(int wallScene);
    void drawPerformer(int bodyScene);
    
    openNIManager * oni;
    vector<scene*>  scenes;
    
    ofVec3f center;
    ofEasyCam cam;
    
    ofImage floorTex, wallTex;
    
    ofFbo backProjo, bodyProjo;

    int width, height;
    
    ofShader shader;
    
};
