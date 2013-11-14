//
//  birdBlooms.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-10.
//
//
#include "ofMain.h"
#include "ofxSvg.h"

struct birdData {
    ofVec2f pos;
    float size;
    int alpha;
    bool fadedAway;
};


class birdBlooms {
    public:
    
    void loadSVG(string filename);
    void addBird(ofVec2f pos);
    void update();
    void draw();
    
    ofxSVG bird;
    vector<birdData> birds;
    
};