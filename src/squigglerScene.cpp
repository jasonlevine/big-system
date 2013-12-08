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

    fbo.allocate(1920, 1080, GL_RGBA32F_ARB);
    fbo.begin();
    ofClear(0);
    fbo.end();

    cam.setScale(1, -1, 1);

    ofSetLineWidth(4);

    post.init(1920, 1080);
    post.createPass<FxaaPass>()->setEnabled(false);
    post.createPass<KaleidoscopePass>()->setEnabled(false);
    post.createPass<DofAltPass>()->setEnabled(false);
    post.createPass<BloomPass>()->setEnabled(false);
    post.createPass<GodRaysPass>()->setEnabled(false);
    post.createPass<RimHighlightingPass>()->setEnabled(false);
    post.createPass<ContrastPass>()->setEnabled(true);
    
    post.setFlip(false);

    renderPasses = post.getPasses();
    setupGUI();
    
    cam.setScale(1, -1, 1);
    cam.setPosition(posX, posY, posZ);
    cam.lookAt(ofVec3f(lookatX, lookatY, lookatZ));
    cam.setOrientation(ofVec3f(orientX, orientY, orientZ));
    
	gui->toggleVisible();
	
	brightness = 1.0;
	contrast = 1.0;
	
    presets.loadFile("presets.xml");
    presets.pushTag("root");
	
}

void squigglerScene::setupSquigglers(vector<int> &tracks){
    for (int i = 0; i < tracks.size(); i++) {
        squigglers.push_back(new squiggler);
        squigglers[i]->setup(255 * (i+1) + 2, tracks[i], 0.06, 140);
    }
}

void squigglerScene::update(int width, int height){
    gui->update();
    
    for (int i = 0; i < squigglers.size(); i++) {
        squigglers[i]->update(aa->pitch[squigglers[i]->track], aa->amp[squigglers[i]->track], width, height);
    }
}

void squigglerScene::draw(int x, int y, int width, int height, bool drawToScreen = true){

    glEnable(GL_DEPTH_TEST);
	
	ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofBackground(0);
    fbo.begin();
	
	ofClear(0, fadeAmt);
    
    cam.begin();
//    ofDrawAxis(100);
    for (int i = 0; i < squigglers.size(); i++) {
        squigglers[i]->draw();
    }
    cam.end();

    fbo.end();
    glDisable(GL_DEPTH_TEST);
    

    post.begin();
    fbo.draw(0,0);
    post.end(drawToScreen);
    
    ofDisableBlendMode();
}

ofTexture & squigglerScene::getTexRef(int width, int height){
//    draw(width, height, false);
//    return post.getProcessedTextureReference();
}


void squigglerScene::setupGUI(){
    fadeAmt = 255;
    useCam = false;
    posX = posY = 0;
    posZ = 2000;
    lookatX = lookatY = lookatZ = 0;
    orientX = orientY = orientZ = 0;
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
    gui->addSlider("posX", -1920, 1920, &posX, length-xInit, dim);
    gui->addSlider("posY", -1080, 1080, &posY, length-xInit, dim);
    gui->addSlider("posZ", 0, 2000, &posZ, length-xInit, dim);
    gui->addSlider("lookatX", -1920, 1920, &lookatX, length-xInit, dim);
    gui->addSlider("lookatY", -1080, 1080, &lookatY, length-xInit, dim);
    gui->addSlider("lookatZ", -1000, 1000, &lookatZ, length-xInit, dim);
    gui->addSlider("orientX", -180, 180, &orientX, length-xInit, dim);
    gui->addSlider("orientY", -180, 180, &orientY, length-xInit, dim);
    gui->addSlider("orientZ", -180, 180, &orientZ, length-xInit, dim);
    gui->addSpacer(length-xInit, 1);
    gui->addLabelToggle("flip?", false);
    gui->addLabelToggle("kaleidoscope", false);
    gui->addLabelToggle("dof", false);
    gui->addLabelToggle("bloom", false);
    gui->addLabelToggle("godrays", false);
    gui->addLabelToggle("highlight", false);
    gui->addSpacer(length-xInit, 1);
    gui->addSlider("brightness", 0.0, 5.0, &brightness, length-xInit, dim);
    gui->addSlider("contrast", 0.0, 5.0, &contrast, length-xInit, dim);
    gui->addLabelButton("reset bri/con", false);
    gui->addSpacer(length-xInit, 1);
    gui->addLabelButton("save preset", false);
    
    string path = "squigglerPresets/";
    ofDirectory dir(path);
    dir.listDir();
    
    vector<string> presets;
    for(int i = 0; i < dir.numFiles(); i++){
        presets.push_back(dir.getPath(i));
        cout << dir.getPath(i) << endl;
    }
    
    
    
    ddl = gui->addDropDownList("presets", presets);
    ddl->setAllowMultiple(false);

    
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
    else if(name == "orientX" || name == "orientY" || name == "orientZ")
	{
		ofxUISlider *slider = (ofxUISlider *) e.widget;
		cam.setOrientation(ofVec3f(orientX, orientY, orientZ));
	}
    else if(name == "flip?")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		flip = toggle->getValue();
        post.setFlip(flip);
	}
    else if(name == "kaleidoscope")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[1]->setEnabled(toggle->getValue());
        kaleidoscope = toggle->getValue();
	}
    else if(name == "dof")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[2]->setEnabled(toggle->getValue());
        dof = toggle->getValue();
	}
    else if(name == "bloom")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[3]->setEnabled(toggle->getValue());
        bloom = toggle->getValue();
	}
    else if(name == "godrays")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[4]->setEnabled(toggle->getValue());
        godrays = toggle->getValue();
	}
    else if(name == "highlight")
	{
		ofxUILabelToggle *toggle = (ofxUILabelToggle *) e.widget;
		post[5]->setEnabled(toggle->getValue());
        highlight = toggle->getValue();
	}
    if (name == "save preset") {
        ofxUILabelButton *button = (ofxUILabelButton *) e.widget;
        if (button->getValue()) {
            
            string path = "squigglerPresets/";
            ofDirectory dir(path);
            string folderName = ofGetTimestampString() + "/";
            dir.createDirectory(path + folderName);
            string filename = path + folderName + "main.xml";
            gui->saveSettings(filename);
            
            for (int i = 0; i < squigglers.size(); i++) {
                string filename = path + folderName + "squiggler" + ofToString(i) + ".xml";
                squigglers[i]->gui->saveSettings(filename);
            }
            
            ddl->addToggle(path + folderName);
        }
    }
    else if(name == "presets")
    {
        ofxUIDropDownList *ddlist = (ofxUIDropDownList *) e.widget;
        vector<ofxUIWidget *> &selected = ddlist->getSelected();
        for(int i = 0; i < selected.size(); i++)
        {
            string path = selected[0]->getName() + "/";
            string filename = path + "main.xml";
            cout << filename << endl;
            gui->loadSettings(filename);
            
            for (int i = 0; i < squigglers.size(); i++) {
                string filename = path + "squiggler" + ofToString(i) + ".xml";
                cout << filename << endl;
                squigglers[i]->gui->loadSettings(filename);
            }
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

void squigglerScene::toggleGUI() {
    gui->toggleVisible();
    for (int i = 0; i < squigglers.size(); i++) {
        squigglers[i]->gui->toggleVisible();
    }
}