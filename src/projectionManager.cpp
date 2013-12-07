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
	
	Screen monitor    ( "monitor", ofRectangle(0, 0, MBP_WIDTH, APP_HEIGHT) );
	Screen projector_1( "proj_01", ofRectangle(MBP_WIDTH, 0, PJ1_WIDTH, APP_HEIGHT) );
	screens.push_back(monitor);
	screens.push_back(projector_1);
}

void projectionManager::update(){
	
	
}

void projectionManager::draw(int wallScene, int bodyScene, float scale, float xOffset, float yOffset){
	
	if (ofGetKeyPressed('p')) {
		screens[0].viewport.activate();
	}
	else if (ofGetKeyPressed('[')) {
		screens[1].viewport.activate();
	}
	else{
		screens[0].viewport.deactivate();
		screens[1].viewport.deactivate();
	}
	
    ofSetColor(255);
    
	
	// mask fbo
    {
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
	}
    
	// fg fbo (body)
	{
		fgFbo.begin();
		scenes[bodyScene]->draw(0, 0, w, h, true);
		fgFbo.end();
	}
	
	// bg fbo (wall)
	{
		bgFbo.begin();
		scenes[wallScene]->draw(0, 0, w, h, true);
		bgFbo.end();
	}
    
	// final fbo
	{
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
	}
	
	
	
	
	for (int i=0; i<screens.size(); i++)
	{
		Screen &screen = screens[i];
		screen.begin();
		finalFbo.draw(0, 0);
		screen.end();
	}
	
	
//	for (int i=0; i<screens.size(); i++)
//	{
//		Screen &screen = screens[i];
//		
//		finalFbo.begin();
//		ofClear()
//		
//		
//		screen.begin();
//		{
//			// pass 1: mask
////			if (screen.draw_scene_index == SCENE_MESH) {
////				int numUsers = oni->openNIDevice.getNumTrackedUsers();
////				ofPushMatrix();
////				ofTranslate(w * 0.5 + xOffset, h * 0.5 + yOffset);
////				ofScale(scale, scale);
////				ofTranslate(-w * 0.5, -h * 0.5);
////				for (int i = 0; i < numUsers; i++){
////					ofxOpenNIUser & user = oni->openNIDevice.getTrackedUser(i);
////					user.drawMask();
////				}
////			}
//			
//			// pass 2: mesh
////			if (screen.draw_scene_index == SCENE_MESH) {
////				cout << 111 << endl;
////				scenes[SCENE_MESH]->draw(0, 0, w, h, true);
////			}
//
//			// pass 3: squiggler_1
//			if (screen.draw_scene_index == SCENE_SQUIGLLER_1) {
//				scenes[SCENE_SQUIGLLER_1]->draw(0, 0, w, h, true);
//			}
//			
//			// pass 4: squiggler_2
////			if (screen.draw_scene_index == SCENE_SQUIGLLER_1) {
////				scenes[SCENE_SQUIGLLER_2]->draw(0, 0, w, h, true);
////			}
//		}
//		screen.end();
//	}
}


void projectionManager::exit()
{
	for (int i=0; i<screens.size(); i++)
	{
		screens[i].save();
	}
}