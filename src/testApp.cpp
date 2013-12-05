#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    
    oni.setup();
    

    scenes.push_back(new squigglerScene());
    scenes.push_back(new meshScene());
    
    for (int i = 0; i < scenes.size(); i++) {
        scenes[i]->setup(&aa, &oni);
    }
    
    currentScene = 0;
    mode = 2;
    scene1 = 0;
    scene2 = 1;
    
//    rm.setup(oni, scenes);
    pm.setup(oni, scenes);
    xOffset = yOffset = 0;
    scale = 1.0;
    
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
//    grayImage.allocate(640,480);
    
    aa.playStems();
    
    
    
}

//--------------------------------------------------------------
void testApp::update(){
    oni.update();
    aa.updateAnalytics();
    
    for (int i = 0; i < scenes.size(); i++) {
        scenes[i]->update();
    }
    
    /*
(    if mode == 2) {
        // get number of current users
        int numUsers = openNIDevice.getNumTrackedUsers();
        
        // iterate through users
        for (int i = 0; i < numUsers; i++){
            
            // get a reference to this user
            ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
            user.setMaskPixelFormat(OF_PIXELS_MONO);
            if (user.getMaskPixels().size() > 0) grayImage.setFromPixels(user.getMaskPixels());
            // draw the mask texture for this user
            //        user.drawMask();
            
            //        ofPopMatrix();
            
        }
        
        if (numUsers > 0 && aa.amp[2] > 0.3) contour.findContours(grayImage, 20, (640*480)/3, 10, true);
    }
    */
    

}

//--------------------------------------------------------------
void testApp::draw(){
	switch (mode) {
        case 0:
            oni.draw();
            break;
            
        case 1:
            aa.drawAnalytics();
            break;
            
        case 2:
            scenes[currentScene]->draw(0, 0, ofGetWidth(), ofGetHeight(), true);
            break;
            
        case 3:
            pm.draw(scene1, scene2, scale, xOffset, yOffset);
            break;
            
//        case 4:
//            rm.draw(0,0);
//            break;
    }
    
    
	ofSetColor(0, 255, 0);
	string msg = "FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(oni.openNIDevice.getFrameRate());
    
	verdana.drawString(msg, ofGetWidth() - 600, 20);
}


//--------------------------------------------------------------
void testApp::exit(){
    oni.openNIDevice.stop();
}

/*
//--------------------------------------------------------------
void testApp::drawGraphics(int width, int height){
    
     for (int i = 0; i < 7; i++) {
         ofPushMatrix();
         ofTranslate(width / 2, height / 2);
         ofRotateZ(360 / float(i));
         ofScale(width / 640, height / 480);
         
         ofTranslate(-320, -240);
         
         ofSetColor(aa.amp[i] *255 , aa.amp[i] * 255, aa.amp[i] * 255, aa.amp[i] * 255);
         
         if (contour.blobs.size() > 0) {
             ofBeginShape();
             for (int i = 0; i < contour.blobs[0].nPts; i++) {
                 ofVertex(contour.blobs[0].pts[i]);
             }
             ofEndShape();
         }
         ofPopMatrix();
     }
}*/


//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case 'k':
            mode++;
            mode %=4;
            cout << "mode = " << mode << endl;
            break;
            
        case 's':
            currentScene++;
            currentScene%=2;
            break;
            
        case 'f':
            ofToggleFullscreen();
            break;
            
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
            aa.setMode(key - 48);
            break;
            
        case OF_KEY_LEFT:
            xOffset-=10;
            break;
            
        case OF_KEY_RIGHT:
            xOffset+=10;
            break;
            
        case OF_KEY_DOWN:
            yOffset-=10;
            break;
            
        case OF_KEY_UP:
            yOffset+=10;
            break;
            
        case '-':
            scale-=0.1;
            break;
            
        case '=':
            scale+=0.1;
            break;
            
        case ' ':
            cout << "scale = " << scale << "xOffsest = " << xOffset << " yoffste = " << yOffset << endl;
            break;
            
        case 'g':
            scenes[currentScene]->toggleGUI();
            break;
            
        case 'x':
            scene1 = abs(scene1 - 1);
            scene2 = abs(scene2 - 1);
            break;
            
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}