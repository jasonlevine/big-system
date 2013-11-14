//
//  bloom.h
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
#include "ofMain.h"

class bloom {
public:
    bloom(){};
    bloom(const ofVec2f _pos, const ofColor col, const int circRes);
    void update();
    void draw();
    bool isDead;
    
protected:
    ofVec3f pos;
    float vel;
    float decay;
    float size;
    float alpha;
    int circleRes;

    ofColor bloomColor;

};


