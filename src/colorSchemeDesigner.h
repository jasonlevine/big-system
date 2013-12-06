//
//  colorSchemeDesigner.h
//  colorSchemeDesigner
//
//  Created by Jason Levine on 2013-12-05.
//
//

#pragma once
#include "ofMain.h"
#include "ofxUI.h"


class colorSchemeDesigner {
public:
    void setup();
    void draw();
    void drawHueScheme(int scheme, int position);
    void guiEvent(ofxUIEventArgs &e);
    
    void addColorRef(ofFloatColor *col) { assignColors.push_back(col); }
    void assignRandom(bool unique);
    
    ofxUICanvas * gui;
    
    ofFloatColor primary;
    
    float hue, saturation, brightness;
    float angle, distance;
    
    float width, xOffset;
    
    int mode;
    
    vector<vector<ofFloatColor> > colorScheme;
    vector<ofFloatColor*> assignColors;
};
