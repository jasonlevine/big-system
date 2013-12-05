//
//  squigglerScene.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-02.
//
//

#include "squigglerScene.h"

void squigglerScene::setup(audioAnalytics * _aa, openNIManager * _oni) {

    aa = _aa;
    oni = _oni;
    
    voxSquiggler.setup(255+2, 0, 140, 0.06);
    bgVoxSquiggler.setup(255 * 2 + 2, 1, 140, 0.06);
    kickSquiggler.setup(255 * 3 + 2, 2, 160, 0.4);
    bassSquiggler.setup(255 * 4 + 2, 3, 100, 0.2);

    fbo.allocate(1024, 768, GL_RGBA32F_ARB);
    fbo.begin();
    ofClear(0);
    fbo.end();

    cam.setScale(1, -1, 1);

    //    voxStats.addChild("root");
    //    voxStats.setTo("root");

    ofSetLineWidth(4);

    post.init(1024, 768);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    post.createPass<DofAltPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    post.createPass<RimHighlightingPass>()->setEnabled(false);
    post.setFlip(false);


    renderPasses = post.getPasses();
    //    shared_ptr<KaleidoscopePass> Kaleidoscope = static_pointer_cast<KaleidoscopePass>(renderPasses[4]);
    //    Kaleidoscope->setSegments(4);
    setupGUI();

    presets.loadFile("presets.xml");
    presets.pushTag("root");
}

void squigglerScene::update(){
    gui->update();
    voxSquiggler.update(aa->pitch[voxSquiggler.track], aa->amp[voxSquiggler.track], useCam);
    bgVoxSquiggler.update(aa->pitch[bgVoxSquiggler.track], aa->amp[bgVoxSquiggler.track], useCam);
    kickSquiggler.update(aa->pitch[kickSquiggler.track], aa->amp[kickSquiggler.track], useCam);
    bassSquiggler.update(aa->pitch[bassSquiggler.track], aa->amp[bassSquiggler.track], useCam);
}

void squigglerScene::draw(int x, int y, int width, int height, bool drawToScreen = true){
//    ofPushStyle();
//    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofBackground(0);
    fbo.begin();

    ofSetColor(0,0,0,fadeAmt);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    
    
    if (useCam) cam.begin();
    //ofDrawAxis(100);
    kickSquiggler.draw();
    bassSquiggler.draw();
    bgVoxSquiggler.draw();
    voxSquiggler.draw();
    if (useCam) cam.end();
    fbo.end();
    

    post.begin();
    fbo.draw(0,0);
    
//    ofPushMatrix();
//    ofTranslate(x,y);
    post.end(drawToScreen);
//    ofPopMatrix();
    
    ofDisableBlendMode();
//    ofDisableAlphaBlending();
//    ofPopStyle();
}

ofTexture & squigglerScene::getTexRef(int width, int height){
//    draw(width, height, false);
//    return post.getProcessedTextureReference();
}


void squigglerScene::setupGUI(){
    fadeAmt = 255;
    useCam = false;
    posX = posY = posZ = 0;
    lookatX = lookatY = lookatZ = 0;
    usePost = false;
    kaleidoscope = dof = bloom = highlight = godrays = false;
    
    //init gui dims
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    //gui!
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    gui->addFPSSlider("FPS SLIDER", length-xInit, dim*.25, 1000);
    gui->addIntSlider("fadeAmt", 0, 255, &fadeAmt, length-xInit, dim);
    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &useCam, "useCam?", OFX_UI_FONT_MEDIUM));
    gui->addSlider("posX", -ofGetWidth()/2, ofGetWidth()/2, &posX, length-xInit, dim);
    gui->addSlider("posY", -ofGetHeight()/2, ofGetHeight()/2, &posY, length-xInit, dim);
    gui->addSlider("posZ", 0, 2000, &posZ, length-xInit, dim);
    gui->addSlider("lookatX", -ofGetWidth()/2, ofGetWidth()/2, &lookatX, length-xInit, dim);
    gui->addSlider("lookatY", -ofGetHeight()/2, ofGetHeight()/2, &lookatY, length-xInit, dim);
    gui->addSlider("lookatZ", -1000, 1000, &lookatZ, length-xInit, dim);
    gui->addSpacer(length-xInit, 1);
    gui->addWidgetDown(new ofxUILabelToggle(dim, dim, &usePost, "usePost?", OFX_UI_FONT_MEDIUM));
    gui->addLabelToggle("flip?", false);
    gui->addLabelToggle("kaleidoscope", false);
    gui->addLabelToggle("dof", false);
    gui->addLabelToggle("bloom", false);
    gui->addLabelToggle("highlight", false);
    gui->addLabelToggle("godrays", false);
    
    
    ofAddListener(gui->newGUIEvent,this,&squigglerScene::guiEvent);
}

void squigglerScene::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    
    //    else
    if(name == "posX" || name == "posY" || name == "posZ")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cam.setPosition(posX, posY, posZ);
	}
    else if(name == "lookatX" || name == "lookatY" || name == "lookatZ")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cam.lookAt(ofVec3f(lookatX, lookatY, lookatZ));
	}
    //    else if(name == "useCam?")
    //	{
    //		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
    //		ofSetDepthTest(toggle->getValue());
    //	}
    else if(name == "flip?")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		flip = toggle->getValue();
        post.setFlip(flip);
	}
    else if(name == "kaleidoscope")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[0]->setEnabled(toggle->getValue());
        kaleidoscope = toggle->getValue();
	}
    else if(name == "dof")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[1]->setEnabled(toggle->getValue());
        dof = toggle->getValue();
	}
    else if(name == "bloom")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[2]->setEnabled(toggle->getValue());
        bloom = toggle->getValue();
	}
    else if(name == "highlight")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[3]->setEnabled(toggle->getValue());
        highlight = toggle->getValue();
	}
    else if(name == "godrays")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[4]->setEnabled(toggle->getValue());
        godrays = toggle->getValue();
	}

}

void squigglerScene::toggleGUI() {
    gui->toggleVisible();
    voxSquiggler.gui->toggleVisible();
    bgVoxSquiggler.gui->toggleVisible();
    kickSquiggler.gui->toggleVisible();
    bassSquiggler.gui->toggleVisible();
}