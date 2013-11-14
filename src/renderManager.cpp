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
    
    shader.load("shadersGL2/shader");
}

//-----------------------------------------------------
void renderManager::update() {
    
}



//-----------------------------------------------------
void renderManager::draw(int wallScene, int bodyScene) {
    ofSetDepthTest(true);
//    cam.begin();
//    ofDrawAxis(100);
//    drawStage(wallScene);
    drawPerformer(bodyScene);
//    cam.end();
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
    ofSetColor(255, 255, 255);
    
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    
    cam.begin();
    //    ofDrawAxis(100);
    
    ofImage depthImage, maskImage;
    
    int numUsers = oni->openNIDevice.getNumTrackedUsers();
    for (int i = 0; i < numUsers; i++){
        
        
        ofxOpenNIUser & user = oni->openNIDevice.getTrackedUser(i);
        
        ofPixels depthPix = oni->openNIDevice.getDepthPixels();
        depthImage = depthPix;
        maskImage = user.getMaskPixels();
        
        
        ofMesh depthMesh;
        int width = oni->openNIDevice.getWidth();
        int height = oni->openNIDevice.getHeight();
        for (float y = 0; y < height; y++) {//+=10
            for (float x = 0; x < width; x++) {
                float lightness = depthPix.getColor(x, y).getLightness();
                float z = lightness == 0 ? 255 : lightness;
                depthMesh.addVertex(ofVec3f(x, y, z));//depthPix.getColor(x,y).r * 10
                depthMesh.addTexCoord(ofVec2f(x, y));
            }
        }
        
        
        for (int y = 0; y < height - 1; y++){
            for (int x = 0; x < width - 1; x++){
                depthMesh.addIndex(x+y*width);       // 0
                depthMesh.addIndex((x+1)+y*width);     // 1
                depthMesh.addIndex(x+(y+1)*width);     // 10
                
                depthMesh.addIndex((x+1)+y*width);     // 1
                depthMesh.addIndex((x+1)+(y+1)*width);   // 11
                depthMesh.addIndex(x+(y+1)*width);     // 10
            }
        }
        
        ofPushMatrix();
        ofTranslate(-320, -240, 0);
        shader.begin();
        shader.setUniformTexture("tex0", bodyProjo.getTextureReference(), 0);
        shader.setUniformTexture("tex1", maskImage, 1);
        depthMesh.draw();
        shader.end();
        ofPopMatrix();
        
    }
    cam.end();
    ofDisableBlendMode();

    
    ofSetColor(255);
//    depthImage.draw(0,0,160,120);
//    maskImage.draw(160,0,160, 120);
//    bodyProjo.draw(320,0,160, 120);
}