//
//  bloom.cpp
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "bloom.h"

bloom::bloom(const ofVec2f _pos, const ofColor col, const int circRes){
    pos = _pos;
    vel = 1;
    decay = 0.95;
    size = 3;
    alpha = 1.0f;
    bloomColor = col;
    isDead = false;
    circleRes = circRes;
}


void bloom::update() {
    if (pos.z > headZ) {
        isDead = true;
    }
    else {
        size+=vel;
        vel*=decay;
        alpha-=0.01;
        bloomColor.a = alpha * 255;
    }
}

void bloom::draw() {
    ofPushMatrix();
    
    ofTranslate(pos);
    ofRotateZ(zAngle);
    ofRotateY(yAngle);
    
    ofSetColor(bloomColor);
    ofSetCircleResolution(circleRes);
    ofNoFill();
    ofCircle(0, 0, size);

    ofColor insideCol = bloomColor;
    insideCol.a = ofClamp(bloomColor.a / 2, 0, 255);
    ofSetColor(insideCol);
    ofFill();
    ofCircle(0, 0, size);
    
    ofPopMatrix();
    
    ofSetCircleResolution(22);
}
