#ifndef _TEST_APP
#define _TEST_APP

#include "ofMain.h"


#include "ofxOpenCv.h"

#include "openNIManager.h"
//#include "audioAnalytics.h"
//#include "renderManager.h"
#include "projectionManager.h"

#include "scene.h"
#include "squigglerScene.h"
#include "meshScene.h"
//#include "hexagonScene.h"

#define USERECORDING


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
    
    vector<scene*> scenes;
    int currentScene;
    
//    renderManager rm;
    projectionManager pm;
    float xOffset, yOffset, scale;
    
    ofTrueTypeFont verdana;
    
    //to be abstracted away!
//    
//    ofxCvContourFinder contour;
//    ofxCvGrayscaleImage grayImage;

   
    

    

};



#endif
