#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"

#include "openNIManager.h"
//#include "audioAnalytics.h"
//#include "renderManager.h"
#include "projectionManager.h"
#include "gamePadManager.h"

#include "colorSchemeDesigner.h"

#include "scene.h"
#include "squigglerScene.h"
#include "meshScene.h"


class testApp : public ofBaseApp{

public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);


    openNIManager oni;
    
    audioAnalytics aa;
    int mode;
    
    
    
    colorSchemeDesigner colorScheme;
    
    vector<scene*> scenes;
    int currentScene;
    
    int wallScene, bodyScene;
    
//    renderManager rm;
    projectionManager pm;
    float xOffset, yOffset, scale;
    
    gamePadManager gamePad;
    
    ofTrueTypeFont verdana;


};



#endif
