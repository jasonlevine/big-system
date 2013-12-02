//
//  projectionManager.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-02.
//
//

#pragma once

#include "openNIManager.h"
#include "scene.h"

class projectionManager {
public:
    void setup(openNIManager &_oni, vector<scene*> &_scenes);
    void update();
    void draw(int wallScene, int bodyScene);
    
    openNIManager * oni;
    vector<scene*> scenes;
    
    ofShader maskingShader;
    
    int bodyScene, wallScene;
    
    ofFbo maskFbo;
    ofFbo fbo;
    
};
