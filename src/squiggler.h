//
//  squiggler.h
//  just_you
//
//  Created by Jason Levine on 2013-12-01.
//
//

#pragma once 

#include "ofMain.h"
#include "ofxUI.h"

class squiggler {
public:
    void setup(float guiX, int _track, float _maxAmp, float _maxPitch);
    void update(float pitch, float amp, bool useCam);
    void draw();
    void guiEvent(ofxUIEventArgs &e);
    
    ofxUICanvas *gui;
    
    vector<ofVec3f> trail;
    ofPolyline polyline, leftLine, rightLine;
    
    int track;
    float maxAmp;
    float maxPitch;
    
    int trailLength;
    bool curve;
    int smoothing;
    float baseThickness;
    bool taper;
    bool calligraphy;
    ofFloatColor *colorStart, *colorEnd, *colorLine;
    ofFloatColor base;
    bool redFade, greenFade, blueFade, alphaFade;
    float zFade;
    float melt;
};