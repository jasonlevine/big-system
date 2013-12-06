//
//  meshScene.cpp
//  just_you_AA_NI_GLSL
//
//  Created by Jason Levine on 2013-12-02.
//
//

#include "meshScene.h"

void meshScene::setup(audioAnalytics * _aa, openNIManager * _oni){
    aa = _aa;
    oni = _oni;
    bassAccum = 0.0;
    
    setupGUI();
    
    for (int i = 0; i < 120; i++) {
        vector<float> wave;
        wave.assign(512, 0);
        waveHistory.push_back(wave);
    }
    
    // Setup post-processing chain
    post.init(1024, 768);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<DofAltPass>()->setEnabled(true);
    post.createPass<GodRaysPass>()->setEnabled(true);
    post.createPass<RimHighlightingPass>()->setEnabled(false);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    
    
    renderPasses = post.getPasses();
    shared_ptr<KaleidoscopePass> Kaleidoscope = static_pointer_cast<KaleidoscopePass>(renderPasses[4]);
    Kaleidoscope->setSegments(0);
    lookatX = 0;
    lookatY = 0;
    lookatZ = -600;
    
    waveStrength = 1.0;
    noiseStrength = 0.0;
    
    drawPost = false;

}

void meshScene::update(){
    vector<float> wave;
    aa->taps[0]->getSamples(wave, 0);
    waveHistory.push_back(wave);
    if (waveHistory.size() > 120) waveHistory.erase(waveHistory.begin());
    

    bassAccum += aa->amp[3] / waveSpeed;
    
    gui->update();
    
    cam.setPosition(camX, camY, camZ);
    cam.lookAt(ofVec3f(lookatX, lookatY, lookatZ));

}

void meshScene::draw(int x, int y, int width, int height, bool drawToScreen = true){
	
	glEnable(GL_DEPTH_TEST);
	
    float noiseVel = bassAccum;
    
    if (waveHistory[0].size() > 0) {
        int width = 120;
        int height = waveHistory.size();
        
        ofMesh mesh;
        
        for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                float noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
                //                float h = waveHistory[y][x] * 0.5 + waveHistory[y][x] * noiseValue * 0.5 + noiseValue * 0.5;
                float h = waveHistory[y][x] * waveStrength + noiseValue * noiseStrength; //waveHistory[y][x] * noiseValue * 0.5 +
                
                mesh.addVertex(ofPoint(x, h * hScale, y)); //waveHistory[y][x] +
                float col = h * colScale;
                
                mesh.addColor(*meshHiCol * col + *meshCol * (1.0 - col));
            }
        }
        
        // now it's important to make sure that each vertex is correctly connected with the
        // other vertices around it. This is done using indices, which you can set up like so:
        for (int y = 0; y < height-1; y++){
            for (int x = 0; x < width-1; x++){
                mesh.addIndex(x+y*width);       // 0
                mesh.addIndex((x+1)+y*width);     // 1
                mesh.addIndex(x+(y+1)*width);     // 10
                
                mesh.addIndex((x+1)+y*width);     // 1
                mesh.addIndex((x+1)+(y+1)*width);   // 11
                mesh.addIndex(x+(y+1)*width);     // 10
            }
        }
        
        
        post.begin(cam);
        ofPushMatrix();
        ofRotateX(meshRotateX);
        ofRotateY(180);
        ofScale(scaleX, scaleY, scaleZ);
        ofTranslate(-width/2, 0, -height/2);
        ofSetLineWidth(lineWidth);
        mesh.drawWireframe();
        ofPopMatrix();
        post.end();
    }
	glDisable(GL_DEPTH_TEST);
}

//--------------------------------------------------------------
ofTexture & meshScene::getTexRef(int width, int height){
//    draw(width, height, false);
//    return post.getProcessedTextureReference();

}


//--------------------------------------------------------------
void meshScene::setupGUI(){
    noiseScale = 0.01;
    meshRotateX = 0.0;
    waveSpeed = 1.0;
    camX = 0;
    camY = 150;
    camZ = 600;
    lineWidth = 1.0;
    scaleX = 7;
    scaleY = 180;
    scaleZ = 10;
    
    meshCol = new ofFloatColor(0.5,0.5,0.5);
    meshHiCol = new ofFloatColor(1.0,1.0,1.0);
//    meshCol->set(0.5, 0.5, 0.5);
//    meshHiCol->set(1.0, 1.0, 1.0);
    
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    gui = new ofxUIScrollableCanvas(0, 0, length+xInit, ofGetHeight());
    gui->setScrollAreaToScreen();
    gui->setScrollableDirections(false, true);
    
    gui->addFPSSlider("FPS SLIDER", length-xInit, dim*.25, 1000);
//    gui->addSpacer(length-xInit, 1);
//    gui->addSlider("meshCol.r", 0.0, 1.0, &meshCol.r, length-xInit, dim);
//    gui->addSlider("meshCol.g", 0.0, 1.0, &meshCol.g, length-xInit, dim);
//    gui->addSlider("meshCol.b", 0.0, 1.0, &meshCol.b, length-xInit, dim);
//    gui->addSlider("meshHiCol.r", 0.0, 1.0, &meshHiCol.r, length-xInit, dim);
//    gui->addSlider("meshHiCol.g", 0.0, 1.0, &meshHiCol.g, length-xInit, dim);
//    gui->addSlider("meshHiCol.b", 0.0, 1.0, &meshHiCol.b, length-xInit, dim);
    gui->addSlider("lineWidth", 0.0, 10.0, &lineWidth, length-xInit, dim);

    gui->addSpacer(length-xInit, 1);
    gui->addSlider("noiseScale", 0.0, 0.1, &noiseScale, length-xInit, dim);
    gui->addSlider("noiseStrength", 0.0, 1.0, &noiseStrength, length-xInit, dim);
    gui->addSlider("waveSpeed", 1.0, 100.0, &waveSpeed, length-xInit, dim);
    gui->addSlider("waveStrength", 0.0, 1.0, &waveStrength, length-xInit, dim);
    gui->addSlider("hScale", 1.0, 20.0, &hScale, length-xInit, dim);
    gui->addSlider("colScale", 1.0, 10.0, &colScale, length-xInit, dim);
    gui->addSlider("meshRotateX", -90.0, 90, &meshRotateX, length-xInit, dim);
    gui->addSlider("scaleX", 0, 50, &scaleX, length-xInit, dim);
    gui->addSlider("scaleY", 0.0, 300, &scaleY, length-xInit, dim);
    gui->addSlider("scaleZ", 0, 50, &scaleZ, length-xInit, dim);
    gui->addSlider("camX", -260, 260, &camX, length-xInit, dim);
    gui->addSlider("camY", 0.0, 500, &camY, length-xInit, dim);
    gui->addSlider("camZ", -600, 600, &camZ, length-xInit, dim);
    gui->addSlider("lookatX", -260, 260, &lookatX, length-xInit, dim);
    gui->addSlider("lookatY", -500, 500, &lookatY, length-xInit, dim);
    gui->addSlider("lookatZ", -600, 600, &lookatZ, length-xInit, dim);
    gui->addSpacer(length-xInit, 1);
    gui->addLabelToggle("Kaleidoscope", false);
    gui->addSlider("numSegments", 1.0, 10.0, 1.0, length-xInit, dim);
    gui->addLabelToggle("Bloom", false);
    gui->addLabelToggle("Dof", false);
    gui->addSlider("DofFStop", 1.0, 15.0, 1.0, length-xInit, dim);
    gui->addSpacer(length-xInit, 1);
    gui->addLabelToggle("GodRays", false);
    gui->addLabelToggle("RimHighlighting", false);
    
    gui->autoSizeToFitWidgets();
    gui->getRect()->setWidth(ofGetWidth());
    
    
    ofAddListener(gui->newGUIEvent,this,&meshScene::guiEvent);
}


//--------------------------------------------------------------
void meshScene::guiEvent(ofxUIEventArgs &e){
    string name = e.widget->getName();
    int kind = e.widget->getKind();
    
    if(name == "DofFStop")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        shared_ptr<DofAltPass> Dof = static_pointer_cast<DofAltPass>(renderPasses[1]);
        Dof->setFStop(slider->getScaledValue());
	}
    else if(name == "numSegments")
    {
        ofxUISlider *slider = (ofxUISlider *) e.widget;
        shared_ptr<KaleidoscopePass> Kaleidoscope = static_pointer_cast<KaleidoscopePass>(renderPasses[4]);
        Kaleidoscope->setSegments(slider->getScaledValue());
	}
    else if(name == "Bloom") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[0]->setEnabled(button->getValue());
    }
    else if(name == "Dof") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[1]->setEnabled(button->getValue());
    }
    else if(name == "GodRays") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[2]->setEnabled(button->getValue());
    }
    else if(name == "RimHighlighting") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[3]->setEnabled(button->getValue());
    }
    else if(name == "Kaleidoscope") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[4]->setEnabled(button->getValue());
    }
}



void meshScene::toggleGUI(){
    gui->toggleVisible();
}


