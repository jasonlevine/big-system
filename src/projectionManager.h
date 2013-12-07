//
//  projectionManager.h
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-02.
//
//

#pragma once

#include "openNIManager.h"
#include "ofxGLWarper.h"
#include "scene.h"

class Screen;

const bool use_auto_save = true;
const int APP_HEIGHT = 800;
const int MBP_WIDTH = 824;
const int PJ1_WIDTH = 1024;

enum SCENE_INDEX {
	SCENE_SQUIGLLER_1 = 0,
	SCENE_SQUIGLLER_2 = 1,
	SCENE_MESH = 2,
	NUM_SCENES = 3
};


class projectionManager {
public:
    void setup(openNIManager &_oni, vector<scene*> &_scenes);
    void update();
    void draw(int wallScene, int bodyScene, float scale, float xOffset, float yOffset);
	void exit();
	
    openNIManager * oni;
    vector<scene*> scenes;
	vector<Screen> screens;
    
    ofShader shader;
    
    int bodyScene, wallScene;
    
    ofFbo maskFbo, fgFbo, bgFbo;
    ofFbo finalFbo;
    
    ofPlanePrimitive plane;
    
    int w, h;
};


class Screen
{
private:
	
	float circleRadius = 20;
	
public:
	
	int draw_scene_index = 0;
	ofRectangle r;
	ofxGLWarper viewport;
	string screen_name;
	
	
	// -----
	Screen( string name, ofRectangle view )
	{
		if (use_auto_save)
			screen_name = "screen_" + name + ".xml";
		
		r = view;
		viewport.setup(view.x, view.y, view.width, view.height);
		viewport.load(screen_name);
	}
	
	void save()
	{
		if (use_auto_save)
			viewport.save(screen_name);
	}
	
	
	void begin()
	{
		viewport.begin();
		
		ofPushMatrix();
		ofTranslate(r.x, r.y);
	}
	
	void end()
	{
		ofPopMatrix();
		
		
		ofPushStyle();
		
//		if (viewport.isActive())
		{
			ofSetColor(ofColor(255, 0, 0));
			ofNoFill();
			
			ofRect(r);
			ofCircle(r.getTopLeft(), circleRadius);
			ofCircle(r.getTopRight(), circleRadius);
			ofCircle(r.getBottomLeft(), circleRadius);
			ofCircle(r.getBottomRight(), circleRadius);
		}
		ofPopStyle();
		viewport.end();
	}
};
