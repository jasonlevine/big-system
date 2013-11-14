//
//  hexagonScene.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//

#pragma once

#include "scene.h"

class hexagonScene : public scene {
public:
    void setup(audioAnalytics * _aa, openNIManager * _oni);
    void update();
    void draw(int width, int height);
    
    vector<ofImage> waveSeq;
    int numImgs;
    int curFrame;
};