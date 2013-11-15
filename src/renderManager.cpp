//
//  renderManager.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//

#include "renderManager.h"
//-----------------------------------------------------
void renderManager::setup(openNIManager &_oni, vector<scene*> &_scenes) {
    oni = &_oni;
    scenes = _scenes;
    
    cam.setScale(1, -1, 1);
    cam.lookAt(ofVec3f(320, 240, 2000));
    
    floorTex.loadImage("textures/LT_ColoredHaze_01.jpg");
    
    backProjo.allocate(800, 600, GL_RGBA);
    bodyProjo.allocate(800, 600, GL_RGBA);
    
    int width = oni->openNIDevice.getWidth();
    int height = oni->openNIDevice.getHeight();
    
    shader.load("shadersGL3/shader");
}

//-----------------------------------------------------
void renderManager::update() {
    
}



//-----------------------------------------------------
void renderManager::draw(int wallScene, int bodyScene) {
    ofSetDepthTest(true);
    cam.begin();
//    ofDrawAxis(100);
    drawStage(wallScene);
    drawPerformer(bodyScene);
    cam.end();
    ofSetDepthTest(false);
   }


//-----------------------------------------------------
void renderManager::drawStage(int wallScene){
    
    ofPlanePrimitive floor, backWall;
    floor.set(800, 1400, 80, 130);
    floor.mapTexCoords(0,0,800,600);
    backWall.set(800, 600, 80, 60);
    backWall.mapTexCoords(0, 0, 800, 600);
    
    //floor
    floorTex.bind();
    ofPushMatrix();
    ofTranslate(0, 240, 900);
    ofRotateX(90);
    ofSetColor(255);
    floor.draw();
    ofPopMatrix();
    floorTex.unbind();
    
    //backWall
    backProjo.begin();
    ofClear(255);
    ofSetColor(255,255,255,255);
    scenes[wallScene]->draw(800, 600);
    backProjo.end();
    
    backProjo.getTextureReference().bind();
    ofPushMatrix();
    ofTranslate(0, -60, 1500);
    ofSetColor(255);
    ofRotateZ(180);
    backWall.draw();
    ofPopMatrix();
    backProjo.getTextureReference().unbind();

}


//-----------------------------------------------------
void renderManager::drawPerformer(int bodyScene){
    //human!
    bodyProjo.begin();
    ofClear(255);
    ofSetColor(255,255,255);
    scenes[bodyScene]->draw(800,600);
    bodyProjo.end();
    
    
    ///mesh2oni
    int numUsers = oni->openNIDevice.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++){
        
        ofxOpenNIUser & user = oni->openNIDevice.getTrackedUser(i);
        
        if (user.isTracking()) {
            ofPlanePrimitive plane;
            plane.set(640, 480, 64, 48);
            plane.mapTexCoords(0, 0, 640, 480);
            
            ofEnableBlendMode(OF_BLENDMODE_ALPHA);
            
            ofPushMatrix();
            
            shader.begin();
            shader.setUniformTexture("tex0", oni->openNIDevice.getDepthTextureReference(), 0);
            shader.setUniformTexture("tex1", oni->openNIDevice.getimageTextureReference(), 1);
            shader.setUniformTexture("tex2", bodyProjo.getTextureReference(), 2);
            shader.setUniformTexture("tex3", user.getMaskTextureReference(), 3);
            plane.draw();
            shader.end();
            
            ofPopMatrix();
            
            ofDisableBlendMode();
        }
    }
}