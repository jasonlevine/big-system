//
//  squiggler.cpp
//  just_you
//
//  Created by Jason Levine on 2013-12-01.
//
//

#include "squiggler.h"


void squiggler::setup(float guiX, int _track, float _maxAmp, float _maxPitch) {

    track = _track;
    maxAmp = _maxAmp;
    maxPitch = _maxPitch;
    
    
    trailLength = 10;
    curve = true;
    smoothing = 5;
    baseThickness = 20;
    taper = false;
    calligraphy = false;
    colorStart = new ofFloatColor(1.0,1.0,1.0);
    colorEnd = new ofFloatColor(1.0,1.0,1.0);//&base;
    colorLine = new ofFloatColor(1.0,1.0,1.0);//&base;
//    colorStart->set(1.0, 0.7, 1.0, 1.0);
//    colorEnd->set(1.0, 0.7, 1.0, 1.0);
//    colorLine->set(1.0, 0.7, 1.0, 1.0);
    redFade = greenFade = blueFade = alphaFade = false;
    zFade = 0;
    melt = 0;

    
    
    //init gui dims
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    

    //gui!
    gui = new ofxUICanvas(guiX, 0, length+xInit, ofGetHeight());
    gui->addSlider("maxPitch", 50, 150, &maxPitch, length-xInit, dim);
    gui->addSlider("maxAmp", 0.01, 0.2, &maxAmp, length-xInit, dim);
    gui->addSpacer(length-xInit, 1);
    gui->addIntSlider("trailLength", 1, 50, &trailLength, length-xInit, dim);
    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &curve, "curve?", OFX_UI_FONT_MEDIUM));
    gui->addIntSlider("smoothing", 0, 50, &smoothing, length-xInit, dim);
    gui->addSlider("baseThickness", 0, 100, &baseThickness, length-xInit, dim);
    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &taper, "taper?", OFX_UI_FONT_MEDIUM));
    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &calligraphy, "calligraphy?", OFX_UI_FONT_MEDIUM));
    gui->addSpacer(length-xInit, 1);
//    gui->addSlider("red", 0.0, 1.0, &color.r, length-xInit, dim);
//    gui->addSlider("green", 0.0, 1.0, &color.g, length-xInit, dim);
//    gui->addSlider("blue", 0.0, 1.0, &color.b, length-xInit, dim);
    gui->addSlider("colorStart.a", 0.0, 1.0, &colorStart->a, length-xInit, dim);
    gui->addSlider("colorEnd.a", 0.0, 1.0, &colorEnd->a, length-xInit, dim);
    gui->addSlider("colorLine.a", 0.0, 1.0, &colorLine->a, length-xInit, dim);
    
    gui->addSpacer(length-xInit, 1);
//    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &redFade, "redFade?", OFX_UI_FONT_MEDIUM));
//    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &greenFade, "greenFade?", OFX_UI_FONT_MEDIUM));
//    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &blueFade, "blueFade?", OFX_UI_FONT_MEDIUM));
//    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &alphaFade, "alphaFade?", OFX_UI_FONT_MEDIUM));
    gui->addSlider("zFade", 0, 20, &zFade, length-xInit, dim);
    gui->addSlider("melt", -2, 2, &melt, length-xInit, dim);
    gui->addButton("zero melt", false);
	
	gui->toggleVisible();
}

void squiggler::update(float pitch, float amp, int w, int h) {
    gui->update();
    
    float x = pitch / maxPitch * w;
//    x -= ofGetWidth()/2;
    
    float y = ofGetHeight() - amp / maxAmp * h;
//    y -= ofGetHeight()/2;
    
    if ( pitch > 1 && amp > 0.005 ) trail.push_back(ofVec3f(x,y,0));
    if ( trail.size() > trailLength ) trail.erase(trail.begin());
    
    polyline.clear();
    for ( int i = 0; i < trail.size(); i++) {
        trail[i].z-=zFade;
        ofVec3f diff = trail[i] - trail[i+1];
        diff.normalize();
        trail[i] += diff * melt;
        curve ? polyline.curveTo(trail[i]) : polyline.addVertex(trail[i]);
    }
    if (smoothing > 0) polyline = polyline.getSmoothed(smoothing);
    
    leftLine.clear();
    rightLine.clear();
    
    vector<ofVec3f> points = polyline.getVertices();
	for(unsigned int i = 1; i < points.size(); i++){
        
		ofVec3f thisPoint = points[i-1];
		ofVec3f nextPoint = points[i];
        
		ofVec3f direction = (nextPoint - thisPoint);
        
		float distance = direction.length();
        
		ofVec3f unitDirection = direction.normalized();
        
		ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
		ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));
        
        float thickness;
        if (calligraphy) {
            thickness = ofMap(distance, 0, 40, baseThickness, 2, true);
        }
        else if (taper) {
            float pct = (float)i / points.size();
            thickness = baseThickness * pct;
        }
        else thickness = baseThickness;
        
		ofVec3f leftPoint = thisPoint+toTheLeft*thickness;
        leftLine.addVertex(leftPoint);
        
		ofVec3f rightPoint = thisPoint+toTheRight*thickness;
        rightLine.addVertex(rightPoint);
        
	}


}

void squiggler::draw() {
    
    ofMesh mesh;
	mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    
    
	for(unsigned int i = 0; i < leftLine.size(); i++){
        float pct = (float)(leftLine.size() - i) / leftLine.size();
        ofFloatColor meshCol = *colorStart * pct + *colorEnd * ( 1.0 - pct);
//        if (redFade) meshCol.r = color.r - (float)(leftLine.size() - i) / leftLine.size();
//        if (greenFade) meshCol.g = color.g - (float)(leftLine.size() - i) / leftLine.size();
//        if (blueFade) meshCol.b = color.b - (float)(leftLine.size() - i) / leftLine.size();
//        if (alphaFade) meshCol.a = color.a - (float)(leftLine.size() - i) / leftLine.size();
        
		mesh.addVertex(leftLine[i]);
        mesh.addColor(meshCol);
        
		mesh.addVertex(rightLine[i]);
        mesh.addColor(meshCol);
        
	}
    
    ofSetColor(255);
    mesh.draw();
    ofSetColor(*colorLine);
    rightLine.draw();
    leftLine.draw();
}

void squiggler::guiEvent(ofxUIEventArgs &e) {
    
}
