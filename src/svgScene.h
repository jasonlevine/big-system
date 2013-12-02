//
//  svgScene.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//

#pragma once

#include "scene.h"
#include "ofxSvg.h"
#include "birdBlooms.h"

class svgScene : public scene {
public:
    void setup(audioAnalytics * _aa, openNIManager * _oni);
    void update();
    void draw(int width, int height);
    
    ofxSVG bird, waves;
    
    birdBlooms birds;
    bool addBird;
    
    decayer wavesInput;
    smoother wavesInput2;
    float noiseT;
};

    

