//
//  colorSchemeDesigner.cpp
//  colorSchemeDesigner
//
//  Created by Jason Levine on 2013-12-05.
//
//

#include "colorSchemeDesigner.h"

void colorSchemeDesigner::setup(){
    primary.set(1.0, 0.0, 0.0);
    hue = saturation = brightness = 0.85;
    angle = 0.1;
    mode = 0;
    
    width = 200;
    xOffset = 320;
    
    
    vector<ofFloatColor> hueScheme;
    hueScheme.assign(5, ofFloatColor::black);
    
    for (int i = 0; i < 4; i++ ) {
        colorScheme.push_back(hueScheme);
    }
    
    vector<string> names;
    names.push_back("mono");
    names.push_back("complement");
    names.push_back("triad");
    names.push_back("tetrad");
    names.push_back("analogic");
    names.push_back("accented");
    
    
    float xInit = OFX_UI_GLOBAL_WIDGET_SPACING;
    float dim = 32;
    float length = xOffset-xInit;
    
    gui = new ofxUICanvas(0, 0, length+xInit, ofGetHeight());
    
    gui->addSlider("primary hue", 0.0, 1.0, &hue, length-xInit, dim);
    gui->addSlider("primary brightness", 0.0, 1.0, &brightness, length-xInit, dim);
    gui->addSlider("primary saturation", 0.0, 1.0, &saturation, length-xInit, dim);
    gui->addSlider("angle", 0.01, 0.25, &angle, length-xInit, dim);
    gui->addSlider("distance", 0.1, 0.5, &distance, length-xInit, dim);
    
    
    gui->addSpacer(length-xInit, 1);
    gui->addRadio("RADIO VERTICAL", names, OFX_UI_ORIENTATION_VERTICAL, dim, dim);
    
    gui->setAutoDraw(false);
    
    ofAddListener(gui->newGUIEvent,this,&colorSchemeDesigner::guiEvent);
}

//--------------------------------------------------------------
void colorSchemeDesigner::draw(){
//    gui->draw();
    
    drawHueScheme(0, 0);
    
    if (mode == 1) {
        drawHueScheme(1, 1);
        
    }
    
    if (mode == 2 || mode == 4 || mode == 5) {
        drawHueScheme(2, 1);
        drawHueScheme(3, 2);
        
    }
    
    if (mode == 3) {
        drawHueScheme(2, 1);
        drawHueScheme(1, 2);
        drawHueScheme(3, 3);
    }
    
    if (mode == 5) {
        drawHueScheme(1, 3);
    }
}

void colorSchemeDesigner::drawHueScheme(int scheme, int position){
    ofSetColor(colorScheme[scheme][0]);
    ofRect(xOffset + width / 2,
           width / 2 * position,
           width, width / 2);
    
    ofSetColor(colorScheme[scheme][1]);
    ofRect(xOffset,
           width / 2 * position,
           width / 2, width / 4);
    
    ofSetColor(colorScheme[scheme][2]);
    ofRect(xOffset + width * 3 / 2,
           width / 4 + width / 2 * position,
           width / 2, width / 4);
    
    ofSetColor(colorScheme[scheme][3]);
    ofRect(xOffset,
           width / 4 + width / 2 * position,
           width / 2, width / 4);
    
    ofSetColor(colorScheme[scheme][4]);
    ofRect(xOffset + width * 3 / 2,
           width / 2 * position,
           width / 2, width / 4);
}

void colorSchemeDesigner::guiEvent(ofxUIEventArgs &e) {
    string name = e.widget->getName();
	int kind = e.widget->getKind();
    
    if(kind == OFX_UI_WIDGET_TOGGLE)
    {
        //        ofxUIToggle *toggle = (ofxUIToggle *) e.widget;
        //        cout << name << "\t value: " << toggle->getValue() << endl;
        if (name == "mono") mode = 0;
        else if (name == "complement") mode = 1;
        else if (name == "triad") mode = 2;
        else if (name == "tetrad") mode = 3;
        else if (name == "analogic") mode = 4;
        else if (name == "accented") mode = 5;
    }
    
    primary.setHsb(hue, saturation, brightness);
    
    ofFloatColor primaryHB, primaryHS, primaryLB, primaryLS;
    primaryHB = primaryHS = primaryLB = primaryLS = primary;
    
    primaryHB.setBrightness(primary.getBrightness() + distance);
    primaryLB.setBrightness(primary.getBrightness() - distance);
    primaryHS.setSaturation(primary.getSaturation() + distance);
    primaryLS.setSaturation(primary.getSaturation() - distance);
    
    colorScheme[0][0] = primary;
    colorScheme[0][1] = primaryHB;
    colorScheme[0][2] = primaryLB;
    colorScheme[0][3] = primaryHS;
    colorScheme[0][4] = primaryLS;
    
    
    if (mode == 1 || mode == 5) {
        
        ofFloatColor complement, complementHB, complementHS, complementLB, complementLS;
        complement = primary;
        float cHue = primary.getHue() - 0.5;
        if ( cHue < 0.0 ) cHue += 1.0;
        complement.setHue(cHue);
        complementHB = complementHS = complementLB = complementLS = complement;
        
        complementHB.setBrightness(complement.getBrightness() + distance);
        complementLB.setBrightness(complement.getBrightness() - distance);
        complementHS.setSaturation(complement.getSaturation() + distance);
        complementLS.setSaturation(complement.getSaturation() - distance);
        
        colorScheme[1][0] = complement;
        colorScheme[1][1] = complementHB;
        colorScheme[1][2] = complementLB;
        colorScheme[1][3] = complementHS;
        colorScheme[1][4] = complementLS;
        
    }
    
    if (mode == 2 || mode == 4 || mode == 5) {
        
        
        ofFloatColor second, secondHB, secondHS, secondLB, secondLS;
        ofFloatColor third, thirdHB, thirdHS, thirdLB, thirdLS;
        
        float mod = (mode == 2) ? 0.5 : 0.0;
        
        second = third = primary;
        float sHue = primary.getHue() - mod + angle;
        if ( sHue < 0.0 ) sHue += 1.0;
        second.setHue(sHue);
        
        float tHue = primary.getHue() - mod - angle;
        if ( tHue < 0.0 ) tHue += 1.0;
        third.setHue(tHue);
        
        secondHB = secondHS = secondLB = secondLS = second;
        
        secondHB.setBrightness(second.getBrightness() + distance);
        secondLB.setBrightness(second.getBrightness() - distance);
        secondHS.setSaturation(second.getSaturation() + distance);
        secondLS.setSaturation(second.getSaturation() - distance);
        
        colorScheme[2][0] = second;
        colorScheme[2][1] = secondHB;
        colorScheme[2][2] = secondLB;
        colorScheme[2][3] = secondHS;
        colorScheme[2][4] = secondLS;
        
        
        //thirds
        thirdHB = thirdHS = thirdLB = thirdLS = third;
        
        thirdHB.setBrightness(third.getBrightness() + distance);
        thirdLB.setBrightness(third.getBrightness() - distance);
        thirdHS.setSaturation(third.getSaturation() + distance);
        thirdLS.setSaturation(third.getSaturation() - distance);
        
        colorScheme[3][0] = third;
        colorScheme[3][1] = thirdHB;
        colorScheme[3][2] = thirdLB;
        colorScheme[3][3] = thirdHS;
        colorScheme[3][4] = thirdLS;
        
    }
    
    if (mode == 3) {
        ofFloatColor complement, complementHB, complementHS, complementLB, complementLS;
        complement = primary;
        float cHue = primary.getHue() - 0.5;
        if ( cHue < 0.0 ) cHue += 1.0;
        complement.setHue(cHue);
        complementHB = complementHS = complementLB = complementLS = complement;
        
        complementHB.setBrightness(complement.getBrightness() + distance);
        complementLB.setBrightness(complement.getBrightness() - distance);
        complementHS.setSaturation(complement.getSaturation() + distance);
        complementLS.setSaturation(complement.getSaturation() - distance);
        
        colorScheme[1][0] = complement;
        colorScheme[1][1] = complementHB;
        colorScheme[1][2] = complementLB;
        colorScheme[1][3] = complementHS;
        colorScheme[1][4] = complementLS;
        
        
        ofFloatColor second, secondHB, secondHS, secondLB, secondLS;
        ofFloatColor third, thirdHB, thirdHS, thirdLB, thirdLS;
        
        second = primary;
        float sHue = primary.getHue() + angle;
        if ( sHue > 1.0 ) sHue -= 1.0;
        second.setHue(sHue);
        
        third = complement;
        float tHue = complement.getHue() + angle;
        if ( tHue > 1.0 ) tHue -= 1.0;
        third.setHue(tHue);
        
        secondHB = secondHS = secondLB = secondLS = second;
        
        secondHB.setBrightness(second.getBrightness() + distance);
        secondLB.setBrightness(second.getBrightness() - distance);
        secondHS.setSaturation(second.getSaturation() + distance);
        secondLS.setSaturation(second.getSaturation() - distance);
        
        colorScheme[2][0] = second;
        colorScheme[2][1] = secondHB;
        colorScheme[2][2] = secondLB;
        colorScheme[2][3] = secondHS;
        colorScheme[2][4] = secondLS;
        
        
        //thirds
        thirdHB = thirdHS = thirdLB = thirdLS = third;
        
        thirdHB.setBrightness(third.getBrightness() + distance);
        thirdLB.setBrightness(third.getBrightness() - distance);
        thirdHS.setSaturation(third.getSaturation() + distance);
        thirdLS.setSaturation(third.getSaturation() - distance);
        
        colorScheme[3][0] = third;
        colorScheme[3][1] = thirdHB;
        colorScheme[3][2] = thirdLB;
        colorScheme[3][3] = thirdHS;
        colorScheme[3][4] = thirdLS;
    }
    
}

void colorSchemeDesigner::assignRandom(bool unique) {
    vector<ofVec2f> colorSchemeIndices;
    
    for (int i = 0; i < colorScheme[0].size(); i++) {
        colorSchemeIndices.push_back(ofVec2f(0,i));
    }
    
    if  (mode == 1 || mode == 3 || mode == 5) {
        for (int i = 0; i < colorScheme[0].size(); i++) {
            colorSchemeIndices.push_back(ofVec2f(1,i));
        }
    }
    
    if  (mode == 2 || mode == 3 || mode == 4 || mode == 5) {
        for (int i = 0; i < colorScheme[0].size(); i++) {
            colorSchemeIndices.push_back(ofVec2f(2,i));
        }
        for (int i = 0; i < colorScheme[0].size(); i++) {
            colorSchemeIndices.push_back(ofVec2f(3,i));
        }
    }
    
    for (int i = 0; i < assignColors.size(); i++ ) {
        int index = ofRandom(0, colorSchemeIndices.size());
        int hue = colorSchemeIndices[index].x;
        int variant = (int)colorSchemeIndices[index].y;
        assignColors[i]->set(colorScheme[hue][variant]);
    }
}

//void colorSchemeDesigner::assignRandom(bool unique) {
//    vector<vector<ofFloatColor> > colorSchemeCopy;
//    
//    colorSchemeCopy.push_back(colorScheme[0]);
//    if  (mode == 1 || mode == 3 || mode == 5) colorSchemeCopy.push_back(colorScheme[1]);
//    if  (mode == 2 || mode == 3 || mode == 4 || mode == 5) {
//        colorSchemeCopy.push_back(colorScheme[2]);
//        colorSchemeCopy.push_back(colorScheme[3]);
//    }
//    
//    int copySize = colorSchemeCopy.size();
//    for (int i = 0; i < assignColors.size(); i++ ) {
//        
//        int mainIndex;
////        do {
//            mainIndex = ofRandom(0, copySize);
////        } while ( colorSchemeCopy[mainIndex].size() == 0 );
//        
//        int subIndex = ofRandom(0, colorSchemeCopy[mainIndex].size());
////        assignColors[i]->set(colorSchemeCopy[mainIndex][subIndex]);
//        assignColors[i] = &colorSchemeCopy[mainIndex][subIndex];
////        if( unique ) colorSchemeCopy[mainIndex].erase( colorSchemeCopy[mainIndex].begin() + subIndex );
//    }
//}