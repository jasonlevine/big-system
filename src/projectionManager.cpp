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
    finalFbo.allocate(w, h, GL_RGBA);
	
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
	
    fgFbo.begin();
    ofClear(0,0,0,255);
    fgFbo.end();
    
    finalFbo.begin();
    ofClear(0,0,0,255);
    finalFbo.end();
    
    plane.set(w, h, w / 10, h / 10);
    plane.mapTexCoords(0, 0, w, h);
	
	Screen monitor    ( "monitor", ofRectangle(0, 0, MBP_WIDTH, APP_HEIGHT) );
	Screen projector_1( "proj_01", ofRectangle(MBP_WIDTH, 0, PJ1_WIDTH, APP_HEIGHT) );
	screens.push_back(monitor);
	screens.push_back(projector_1);
	
	
	
	gui = new ofxUICanvas(600, 0, 300, 400);
	
	for (int i=0; i<screens.size(); i++)
	{
		Screen &screen = screens[i];
		
		gui->addSpacer();
		gui->addLabel(screen.screen_name);
		gui->addIntSlider("draw_scene_index_"+ofToString(i), 0, NUM_SCENES-1, &screen.draw_scene_index);
		gui->addLabelToggle("use_mask_for_screen-"+ofToString(i), &screen.use_mask);
	}
}

void projectionManager::update(){
	
	
}

void projectionManager::draw(int wallScene, int bodyScene, float scale, float xOffset, float yOffset){
	
	// viewport adjusting
	// Key: p, [, ]
	if (ofGetKeyPressed('p')) {
		screens[0].viewport.activate();
	}
	else if (ofGetKeyPressed('[')) {
		screens[1].viewport.activate();
	}
	else if (ofGetKeyPressed(']')) {
		if (screens.size() >2) {
			screens[2].viewport.activate();
		}
	}
	else{
		screens[0].viewport.deactivate();
		screens[1].viewport.deactivate();
		if (screens.size() >2) {
			screens[2].viewport.deactivate();
		}
	}
	
	
    ofSetColor(255);
    
	// mask fbo
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
	
	for (int i=0; i<screens.size(); i++)
	{
		Screen &screen = screens[i];
		
		// making front fbo
		if (screen.use_mask)
		{
			fgFbo.begin();
			ofClear(0);
			if (screen.draw_scene_index == SCENE_SQUIGLLER_1)
				scenes[SCENE_SQUIGLLER_1]->draw(0, 0, w, h, true);
			
			if (screen.draw_scene_index == SCENE_SQUIGLLER_2)
				scenes[SCENE_SQUIGLLER_2]->draw(0, 0, w, h, true);
			
			if (screen.draw_scene_index == SCENE_MESH)
				scenes[SCENE_MESH]->draw(0, 0, w, h, true);
			
			fgFbo.end();
		}
		
		
		// final fbo
		{
			finalFbo.begin();
			ofClear(0, 0, 0, 0);
			
			if (screen.use_mask)
			{
				shader.begin();
				
				shader.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1 );
				shader.setUniformTexture("tex0", fgFbo.getTextureReference(), 2 );
				ofPushMatrix();
				ofTranslate(w * 0.5, h * 0.5);
				ofSetColor(255);
				plane.draw();
				ofPopMatrix();
				
				shader.end();
			}
			else
			{
				if (screen.draw_scene_index == SCENE_SQUIGLLER_1)
					scenes[SCENE_SQUIGLLER_1]->draw(0, 0, w, h, true);
				
				if (screen.draw_scene_index == SCENE_SQUIGLLER_2)
					scenes[SCENE_SQUIGLLER_2]->draw(0, 0, w, h, true);
				
				if (screen.draw_scene_index == SCENE_MESH)
					scenes[SCENE_MESH]->draw(0, 0, w, h, true);
				
				if (screen.draw_scene_index == SCENE_MASK)
					maskFbo.draw(0, 0);
			}
			
			finalFbo.end();
		}

		
		screen.begin();
		finalFbo.draw(0, 0);
		screen.end();
	}
}


void projectionManager::exit()
{
	for (int i=0; i<screens.size(); i++)
	{
		screens[i].save();
	}
}