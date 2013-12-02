//
//  svgScene.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//

#include "svgScene.h"


//--------------------------------------------------------------
void svgScene::setup(audioAnalytics * _aa, openNIManager * _oni) {
    aa = _aa;
    oni = _oni;
    
    bird.load("bird.svg");
    waves.load("sea-waves.svg");
    addBird = false;
    
    wavesInput.setDecay(0.1);
    wavesInput2.setNumPValues(25);
}

//--------------------------------------------------------------
void svgScene::update() {
    //birds
    if (aa->amp[3] > 0.3 && !addBird) {
        birds.addBird(ofVec2f(ofRandom(50, 750), ofRandom(0, 300)));
        addBird = true;
    }
    
    if (aa->amp[3] < 0.2 && addBird) addBird = false;
    
    birds.update();
    
    //waves
//    wavesInput.addValue(aa->amp[2]);
//    noiseT+=wavesInput.getValue() / 10;
    noiseT+=aa->amp[2]/10;
//    wavesInput2.addValue(aa->amp[2]);
}

//--------------------------------------------------------------
void svgScene::draw(int width, int height){
    //waves
    ofSetDepthTest(false);
    
    ofSetHexColor(0xEDF484);
    ofRect(0,0,width, width);

    for (int i = 0; i < waves.getNumPath(); i++){
        ofPushMatrix();
        ofTranslate(width/2, height - height/3);
        ofScale(1 / waves.getWidth() * width, 1 / waves.getHeight() * height);
        ofRotateZ(ofSignedNoise(noiseT+i));
        
        ofPushMatrix();
        ofTranslate(-waves.getWidth()/2, -waves.getHeight()/2);
        waves.getPathAt(i).draw();
        ofPopMatrix();
        
        ofPopMatrix();
    }
    
    
    
    
    
    //birds
    
    ofEnableAlphaBlending();
    for (int i = 0; i < birds.birds.size(); i++){
        ofPushMatrix();
        ofTranslate(birds.birds[i].pos);
        ofTranslate(bird.getWidth()/2, bird.getHeight()/2);
        ofScale(1 / bird.getWidth() * birds.birds[i].size,
                1 / bird.getHeight() * birds.birds[i].size);
        
        for (int j = 0; j < bird.getNumPath(); j++) {
            ofPath path = bird.getPathAt(j);
            int alpha = 255 - birds.birds[i].size / 200 * 255;
            path.setColor(ofColor(0, 0, 0, alpha));
            path.draw();
        }
        
        
        ofTranslate(-bird.getWidth()/2, -bird.getHeight()/2);
        ofPopMatrix();
    }
    
    ofSetDepthTest(true);
    ofDisableAlphaBlending();
}

