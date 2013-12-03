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
    aa = _aa;
    oni = _oni;
    
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
    
    imgW = waveSeq[0].width;
    imgH = waveSeq[0].height;
    
    curFrame = 0;
    
    ampSmooth.setNumPValues(15);
}

//--------------------------------------------------------------
void hexagonScene::update() {
    if (ofGetFrameNum() % 2 == 0) {
        curFrame+=1;
        if (curFrame > (numImgs-1)) curFrame -= numImgs;
    }
//    cout << "aa->pitch[1] = " << aa->pitch[1] << endl;
    ampSmooth.addValue(aa->amp[0]);
//    curFrame = ofMap(pitchSmooth.getMean(), 0, aa->maxPitch[1], 0, numImgs - 1);
//    curFrame += aa->amp[0];
//    if (curFrame > numImgs) curFrame -= numImgs;
//    cout << "curFrame = " << curFrame << endl;
}

//--------------------------------------------------------------
void hexagonScene::draw(int width, int height){
    int frame1 = curFrame;
//    int frame2 = (frame1 + 1) % numImgs;
//    float pct1 = curFrame - frame1;
//    float pct2 = 1.0 - pct1;
    
    ofPixels pix1 = waveSeq[frame1].getPixelsRef();
//    ofPixels pix2 = waveSeq[frame2].getPixelsRef();
    
    for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 30; j++) {
            int x = (float)i / 40 * imgW;
            int y = (float)j / 30 * imgH;

            ofSetColor(pix1.getColor(x,y));
            float w = width / 40;
            float h = height / 30;
            
            if ( (i % 2) == 0) ofCircle(i*w,j*h,w/1.5);
            else ofCircle(i*w,(j+0.5)*h,w/1.5);
        }
    }
}