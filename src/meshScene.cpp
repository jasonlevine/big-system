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
    

    bassAccum += aa->amp[3] / 4;
    
    gui->update();
    
    cam.setPosition(camX, camY, camZ);
    cam.lookAt(ofVec3f(lookatX, lookatY, lookatZ));

}

void meshScene::draw(int width, int height){
    float noiseVel = bassAccum;
    
    if (waveHistory[0].size() > 0) {
        int width = 512;
        int height = waveHistory.size();
        
        ofMesh mesh;
        
        for (int y = 0; y < height; y++){
            for (int x = 0; x<width; x++){
                float noiseValue = ofNoise(x * noiseScale, y * noiseScale, noiseVel);
                //                float h = waveHistory[y][x] * 0.5 + waveHistory[y][x] * noiseValue * 0.5 + noiseValue * 0.5;
                float h = waveHistory[y][x] * (1.0 - noiseStrength) + noiseValue * noiseStrength; //waveHistory[y][x] * noiseValue * 0.5 +
                
                mesh.addVertex(ofPoint(x, h * hScale, y)); //waveHistory[y][x] +
                float col = h * colScale;
                
                
                
                mesh.addColor(ofFloatColor::darkGoldenRod * col + ofFloatColor::chocolate * (1.0 - col));
                
                //float h = (waveHistory[y][x] + 1.0) / 2;
                //                float r, g, b;
                //                r = h > 0.4 ? (h - 0.4) * 2: 0.0; // + aa.amp[3]
                //                g = r; //waveHistory2[y][x];
                //                b = h;
                //                float dist = float(abs(width / 2 - x)) / width;
                //                float distSq = dist * dist;
                //                ofFloatColor gradient = gradientStart * (1.0 - dist) + gradientEnd * dist;
                //                float yDist = float(y) / height;
                //                float yDistSq = yDist * yDist;
                //                mesh.addColor(ofFloatColor(r,g,b) * (1.0 - yDistSq) + gradient * yDistSq);  // add a color at that vertex
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
        
        
        //        ofDrawAxis(100);
        post.begin(cam);
        ofPushMatrix();
        ofRotateX(meshRotateX);
        ofRotateY(180);
        ofScale(7,180,10);
        ofTranslate(-width/2, 0, -height/2);
        mesh.drawWireframe();
        ofPopMatrix();
        post.end();
        
    }

}

//--------------------------------------------------------------
void meshScene::setupGUI(){
    noiseScale = 0.01;
    meshRotateX = 0.0;
    camX = 0;
    camY = 150;
    camZ = 600;
    
    float dim = 16;
	float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float length = 255-xInit;
    
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    gui->addFPSSlider("FPS SLIDER", length-xInit, dim*.25, 1000);
    gui->addSpacer(length-xInit, 1);
    gui->addSlider("noiseScale", 0.0, 0.1, &noiseScale, length-xInit, dim);
    gui->addSlider("noiseStrength", 0.0, 1.0, &noiseStrength, length-xInit, dim);
    gui->addSlider("hScale", 1.0, 20.0, &hScale, length-xInit, dim);
    gui->addSlider("colScale", 1.0, 10.0, &colScale, length-xInit, dim);
    gui->addSlider("meshRotateX", -90.0, 90, &meshRotateX, length-xInit, dim);
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


