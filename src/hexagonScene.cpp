//
//  hexagonScene.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//


#include "hexagonScene.h"

//--------------------------------------------------------------
void hexagonScene::setup(audioAnalytics * _aa, openNIManager * _oni) {
    
    ofSetCircleResolution(6);
    numImgs = 778;
    for (int i = 0; i < numImgs; i++){
        string iterator = ofToString(i);
        string seqNum;
        if ( i < 10 ) seqNum = "00" + iterator;
        else if ( i > 9 && i < 100 ) seqNum = "0" + iterator;
        else seqNum = iterator;
        string filename = "waves/waves" + seqNum + ".jpg";
        
        ofImage frame;
        frame.loadImage(filename);
        waveSeq.push_back(frame);
    }
    
    curFrame = 0;
}

//--------------------------------------------------------------
void hexagonScene::update() {
    if (ofGetFrameNum() % 2 == 0) {
        curFrame++;
        curFrame%=numImgs;
    }
}

//--------------------------------------------------------------
void hexagonScene::draw(int width, int height){
    
    ofPixels pix = waveSeq[curFrame].getPixelsRef();
    
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 30; j++) {
            int x = (float)i / 40 * waveSeq[curFrame].getWidth();
            int y = (float)j / 30 * waveSeq[curFrame].getHeight();
            ofSetColor(pix.getColor(x,y));
            float w = width / 40;
            float h = height / 30;
            
            if ( (i % 2) == 0) ofCircle(i*w,j*h,w/1.5);
            else ofCircle(i*w,(j+0.5)*h,w/1.5);
        }
    }
}