//
//  openNIManager.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-11-11.
//
//

#include "openNIManager.h"

#define USERECORDING

//--------------------------------------------------------------
void openNIManager::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
    
}
//--------------------------------------------------------------
void openNIManager::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    
#ifdef USERECORDING
    openNIDevice.setupFromONI("onis/mary'sy.oni"); //onis/mary'sy.oni
    //claudia mid.oni
#else
    openNIDevice.setup();
#endif
    //    openNIDevice.addImageGenerator();
//    openNIDevice.addDepthGenerator();
//    openNIDevice.setRegister(true);
//    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);
    openNIDevice.start();
    
    openNIDevice.setDepthColoring(COLORING_GREY);
    openNIDevice.setUseMaskTextureAllUsers(true);
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the
    openNIDevice.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel

    // you can alternatively create a 'base' user class
    //    ofxOpenNIUser user;
    //    user.setUseMaskTexture(true);
    //    user.setUsePointCloud(true);
    //    user.setPointCloudDrawSize(2);
    //    user.setPointCloudResolution(2);
    //    openNIDevice.setBaseUserClass(user);
    
}

//--------------------------------------------------------------
void openNIManager::update(){
    openNIDevice.update();
    
}

//--------------------------------------------------------------
void openNIManager::draw(){
    ofSetColor(255, 255, 255);
    
    ofPushMatrix();
    // draw debug (ie., image, depth, skeleton)
    openNIDevice.drawDebug();
    ofPopMatrix();
    
    ofPushMatrix();
    // use a blend mode so we can see 'through' the mask(s)
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // get number of current users
    int numUsers = openNIDevice.getNumTrackedUsers();
    
    // iterate through users
    for (int i = 0; i < numUsers; i++){
        
        // get a reference to this user
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
        // draw the mask texture for this user
        user.drawMask();
        
        // you can also access the pixel and texture references individually:
        
        // TEXTURE REFERENCE
        //ofTexture & tex = user.getMaskTextureReference();
        // do something with texture...
        
        // PIXEL REFERENCE
        //ofPixels & pix = user.getMaskPixels();
        // do something with the pixels...
        
        // and point clouds:
        
//        ofPushMatrix();
        // move it a bit more central
        //ofTranslate(320, 240, -1000);
//        user.drawPointCloud();
        
        // you can also access the mesh:
        
        // MESH REFERENCE
        //ofMesh & mesh = user.getPointCloud();
        // do something with the point cloud mesh
        
//        ofPopMatrix();
    }
    
    
    
    ofDisableBlendMode();
    ofPopMatrix();
    
}
