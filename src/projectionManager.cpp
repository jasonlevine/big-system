//
//  projectionManager.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-02.
//
//

#include "projectionManager.h"

void projectionManager::setup(openNIManager &_oni, vector<scene*> &_scenes){
    
    oni = &_oni;
    scenes = _scenes;
    
//    shader.load("shadersGL3/userMask");
    shader.load("shadersGL2/shader");

    w = 1024;
    h = 768;
    
    maskFbo.allocate(w, h, GL_RGBA);
    fgFbo.allocate(w, h, GL_RGBA);
    bgFbo.allocate(w, h, GL_RGBA);
    finalFbo.allocate(w, h, GL_RGBA);

    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    finalFbo.begin();
    ofClear(0,0,0,255);
    finalFbo.end();
    
    fgFbo.begin();
    ofClear(0,0,0,255);
    fgFbo.end();
    
    bgFbo.begin();
    ofClear(0,0,0,255);
    bgFbo.end();
    
    plane.set(w, h, w / 10, h / 10);
    plane.mapTexCoords(0, 0, w, h);

}

void projectionManager::update(){

}

void projectionManager::draw(int wallScene, int bodyScene, float scale, float xOffset, float yOffset){

    ofSetColor(255);
    
    maskFbo.begin();
    ofClear(0);
    int numUsers = oni->openNIDevice.getNumTrackedUsers();
    ofPushMatrix();
    ofTranslate(w * 0.5 + xOffset, h * 0.5 + yOffset);
    ofScale(scale, scale);
    ofTranslate(-w * 0.5, -h * 0.5);
    for (int i = 0; i < numUsers; i++){
        ofxOpenNIUser & user = oni->openNIDevice.getTrackedUser(i);
        user.drawMask();
    }
    ofPopMatrix();
    maskFbo.end();
    
    fgFbo.begin();
    scenes[bodyScene]->draw(0, 0, w, h, true);
    fgFbo.end();
//    ofTexture &tex0 = scenes[bodyScene]->post.getProcessedTextureReference();

//    fgFbo.draw(0,0);
    
    bgFbo.begin();
    scenes[wallScene]->draw(0, 0, w, h, true);
    bgFbo.end();
    
//    bgFbo.draw(1024,0);
    
//    ofTexture &tex1 = bgFbo.getTextureReference();
    

//
    finalFbo.begin();
    ofClear(0, 0, 0, 0);
    
    shader.begin();
    shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
    shader.setUniformTexture("tex0", fgFbo.getTextureReference(), 2 );
    shader.setUniformTexture("tex1", bgFbo.getTextureReference(), 3 );
    ofPushMatrix();
    ofTranslate(w * 0.5, h * 0.5);
    ofSetColor(255);
    plane.draw();
    ofPopMatrix();
    
    shader.end();
    finalFbo.end();
//
//
//    
//    finalFbo.draw(0,0);
  
}