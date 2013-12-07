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
    post.createPass<FxaaPass>()->setEnabled(true);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<DofAltPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    post.createPass<RimHighlightingPass>()->setEnabled(false);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    post.createPass<ContrastPass>()->setEnabled(true);
    
    
    renderPasses = post.getPasses();
    shared_ptr<KaleidoscopePass> Kaleidoscope = static_pointer_cast<KaleidoscopePass>(renderPasses[4]);
    Kaleidoscope->setSegments(0);
	
    shared_ptr<ContrastPass> contrastPass = static_pointer_cast<ContrastPass>(renderPasses[6]);
    contrastPass->setBrightness(1.0);
	
    lookatX = 0;
    lookatY = 0;
    lookatZ = -600;
    
    waveStrength = 1.0;
    noiseStrength = 0.0;
    
	brightness = 1.0;
	contrast = 1.0;
	
    drawPost = false;
	
	gui->toggleVisible();
    gui2->toggleVisible();
}

void meshScene::update(int width, int height){
    vector<float> wave;
    aa->taps[0]->getSamples(wave, 0);
    waveHistory.push_back(wave);
    if (waveHistory.size() > 60) waveHistory.erase(waveHistory.begin());
    

    bassAccum += aa->amp[3] / waveSpeed;
    
    gui->update();
    gui2->update();
    
    cam.setPosition(camX, camY, camZ);
    cam.lookAt(ofVec3f(lookatX, lookatY, lookatZ));

}

void meshScene::draw(int x, int y, int width, int height, bool drawToScreen = true){
	
	glEnable(GL_DEPTH_TEST);
	
    float noiseVel = bassAccum;
    
    if (waveHistory[0].size() > 0) {
        int width = 60;
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
    
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
//    gui->setScrollAreaToScreen();
//    gui->setScrollableDirections(false, true);
    
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
    
    
    gui2 = new ofxUICanvas(length+xInit + 2, 0, length+xInit, ofGetHeight());
   

    gui2->addLabelToggle("Kaleidoscope", false);
    gui2->addSlider("numSegments", 1.0, 10.0, 1.0, length-xInit, dim);
    gui2->addLabelToggle("Bloom", false);
    gui2->addLabelToggle("Dof", false);
    gui2->addSlider("DofFStop", 1.0, 15.0, 1.0, length-xInit, dim);
    gui2->addSpacer(length-xInit, 1);
    gui2->addLabelToggle("GodRays", false);
    gui2->addLabelToggle("RimHighlighting", false);
    gui2->addSpacer(length-xInit, 1);
    gui2->addSlider("brightness", 0.0, 5.0, &brightness, length-xInit, dim);
    gui2->addSlider("contrast", 0.0, 5.0, &contrast, length-xInit, dim);
    gui2->addLabelButton("reset bri/con", false);
    gui2->addSpacer(length-xInit, 1);
    gui2->addLabelButton("save preset", false);
   
    string path = "meshPresets/";
    ofDirectory dir(path);
    dir.allowExt("GUI1");
    dir.listDir();
    
    vector<string> presets;
    for(int i = 0; i < dir.numFiles(); i++){
        presets.push_back(dir.getPath(i));
        cout << dir.getPath(i) << endl;
    }
    
    ddl = gui2->addDropDownList("presets", presets);
    ddl->setAllowMultiple(false);
    //    ddl->setAutoClose(true);
    gui2->autoSizeToFitWidgets();

//    gui2->getRect()->setWidth(ofGetWidth());
    
    
    ofAddListener(gui->newGUIEvent,this,&meshScene::guiEvent);
    ofAddListener(gui2->newGUIEvent,this,&meshScene::guiEvent);
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
        shared_ptr<KaleidoscopePass> Kaleidoscope = static_pointer_cast<KaleidoscopePass>(renderPasses[5]);
        Kaleidoscope->setSegments(slider->getScaledValue());
	}
    else if(name == "Bloom") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[1]->setEnabled(button->getValue());
    }
    else if(name == "Dof") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[2]->setEnabled(button->getValue());
    }
    else if(name == "GodRays") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[3]->setEnabled(button->getValue());
    }
    else if(name == "RimHighlighting") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[4]->setEnabled(button->getValue());
    }
    else if(name == "Kaleidoscope") //kind == OFX_UI_WIDGET_LABELBUTTON
    {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        post[5]->setEnabled(button->getValue());
    }
    else if (name == "save preset") {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        if (button->getValue()) {
            string filename1 = "meshPresets/" + ofGetTimestampString() + ".GUI1";
            string filename2 = "meshPresets/" + ofGetTimestampString() + ".GUI2";
//            cout << filename << endl;
            gui->saveSettings(filename1);
            gui2->saveSettings(filename2);
            ddl->addToggle(filename1);
        }
    }
    else if(name == "presets")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i = 0; i < selected.size(); i++)
        {
            string presetName = selected[0]->getName();
            gui->loadSettings(presetName);
            presetName[presetName.length()-1] = '2';
//            presetName.append("2");
            cout << presetName << endl;
            gui2->loadSettings(presetName);
        }
    }
	else if(name == "brightness")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		shared_ptr<ContrastPass> contrastPass = static_pointer_cast<ContrastPass>(renderPasses[6]);
		contrastPass->setBrightness( slider->getScaledValue() );
	}
	else if(name == "contrast")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		shared_ptr<ContrastPass> contrastPass = static_pointer_cast<ContrastPass>(renderPasses[6]);
		contrastPass->setContrast( contrast );
	}
	else if(name == "reset bri/con")
	{
		shared_ptr<ContrastPass> contrastPass = static_pointer_cast<ContrastPass>(renderPasses[6]);
		contrastPass->setContrast( 1.0 );
		contrastPass->setBrightness( 1.0 );
		contrast = 1.0;
		brightness = 1.0;
	}
}



void meshScene::toggleGUI(){
    gui->toggleVisible();
    gui2->toggleVisible();

}


