//
//  bloomController.cpp
//  HMXprototype3
//
//  Created by Jason Levine on 12-06-18.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "bloomController.h"

void bloomController::update(float headZ) {
    for (vector<bloom>::iterator bb = blooms.begin(); bb != blooms.end(); ) {
        if (bb->isDead) {
            bb = blooms.erase(bb);
        }
        else {
            bb->update(headZ);
            ++bb;
        }
    }
}

void bloomController::draw() {
    for (vector<bloom>::iterator bb = blooms.begin(); bb != blooms.end(); ++bb) {
        bb->draw();
    }
}

void bloomController::addBloom(const ofVec3f pos, const ofColor col, float zAngle, float yAngle){
    bloom temp(pos, col, circleRes, zAngle, yAngle);
    blooms.push_back(temp);
}
