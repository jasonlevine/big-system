//
//  bloomController.h
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//
#include "bloom.h"

class bloomController {
public:
    bloomController(){};
    void update(float headZ);
    void draw();
    void addBloom(const ofVec3f, const ofColor col, float zAngle, float yAngle);
    void setRes(const int res) { circleRes = res; } ;
    
protected:
    vector<bloom> blooms;
    int circleRes;
};

