//
//  birdBlooms.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-10.
//
//

#include "birdBlooms.h"

void birdBlooms::loadSVG(string filename) {
    bird.load("bird.svg");
}

void birdBlooms::addBird(ofVec2f pos){
    birdData newBird;
    newBird.pos = pos;
    newBird.size = 10;
    newBird.alpha = 255;
    newBird.fadedAway = false;
    birds.push_back(newBird);
}

void birdBlooms::update(){
    for (int i = 0; i < birds.size(); i++){
        birds[i].size++;
        //birds[i].alpha--;
        float sizeNorm = birds[i].size / 200;
        birds[i].alpha = 255 - sizeNorm * sizeNorm * 255;

        if(birds[i].size > 200) birds[i].fadedAway = true;
    }
    
    for (int i = 0; i < birds.size(); i++){
        if (birds[i].fadedAway) birds.erase(birds.begin() + i);
    }
}
void birdBlooms::draw(){
    bird.draw();
    
    for (int i = 0; i < birds.size(); i++){
        ofPushMatrix();
        ofTranslate(birds[i].pos);
        ofTranslate(bird.getWidth()/2, bird.getHeight()/2);
        ofScale(1 / bird.getWidth() * birds[i].size,
                1 / bird.getHeight() * birds[i].size);
        bird.draw();
        ofTranslate(-bird.getWidth()/2, -bird.getHeight()/2);
        ofPopMatrix();
    }
}
