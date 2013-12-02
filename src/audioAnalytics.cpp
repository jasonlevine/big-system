//
//  audioAnalytics.cpp
//  just_you
//
//  Created by Jason Levine on 2013-11-06.
//
//

#include "audioAnalytics.h"
//#include "testApp.h"

#define NUMTRACKS 7

audioAnalytics::audioAnalytics() {
    setupVectors();
    loadTracks();
    setupAUGraph();
    
    faucet.loadFont("faucet.ttf", 18);
}


/////////////////////////////////SETUP//////////////////////////////////

//--------------------------------------------------------------
void audioAnalytics::setupVectors(){
    
    amp.assign(NUMTRACKS, 0.0);
    dB.assign(NUMTRACKS, 0.0);
    pitch.assign(NUMTRACKS, 0.0);
    stemNames.assign(NUMTRACKS, "");
    
    vector<float> fftValues;
    fftValues.assign(512, 0);
    
    vector<float> volValues;
    volValues.assign(ofGetWidth() -200, 0);
    
    vector<float> dBValues;
    dBValues.assign(ofGetWidth() -200, -120);
    
    for ( int i = 0; i < NUMTRACKS; i++ ) {
        ofxAudioUnitFilePlayer * stem = new ofxAudioUnitFilePlayer();
        stems.push_back(stem);
        
        ofxAudioUnitTap * tap = new ofxAudioUnitTap();
        taps.push_back(tap);
        
        ofxAudioUnitFftNode * fft = new ofxAudioUnitFftNode();
        ffts.push_back(fft);
        
        ofxAudioFeaturesChannel * channel = new ofxAudioFeaturesChannel();
        channel->setup(512, 64, 44100);
        channel->usingPitch = true;
        channel->usingOnsets = true;
        audioFeatures.push_back(channel);
        
        ofPolyline temp;
        waves.push_back(temp);
        
        dBHistory.push_back(dBValues);
        ampHistory.push_back(volValues);
        pitchHistory.push_back(volValues);
    }
    
    for (int i = 0; i < NUMTRACKS; i++) {
        vector<float> temp;
        temp.assign(7,0);
        medianFilter.push_back(temp);
    }
    
    //analytics
    maxdB.assign(NUMTRACKS, -120);
    maxAmp.assign(NUMTRACKS, 0);
    maxfft.assign(NUMTRACKS, 0);
    maxPitch.assign(NUMTRACKS, 0);
    
    xml.load("dataMinMax.xml");
    xml.setTo("limits");
    
    for (int i = 0; i < NUMTRACKS; i++) {
        string tag = "track-" + ofToString(i);
        xml.setTo(tag);
        
        maxAmp[i] = ofToFloat(xml.getValue("maxAmp"));
        maxdB[i] = ofToFloat(xml.getValue("maxdB"));
        maxfft[i] = ofToFloat(xml.getValue("maxfft"));
        maxPitch[i] = ofToFloat(xml.getValue("maxPitch"));
        
        xml.setToParent();
    }
    
    mode = 1;
    selectedTrack = -1;
}

//--------------------------------------------------------------
void audioAnalytics::loadTracks(){
    //vox
    stems[0]->setFile(ofFilePath::getAbsolutePath("STEMS/vox.wav"));
    stemNames[0] = "vox";
    //bgVox
    stems[1]->setFile(ofFilePath::getAbsolutePath("STEMS/bgVox.wav")); //vox-lowpass.aiff
    stemNames[1] = "bgVox";
    //beats
    stems[2]->setFile(ofFilePath::getAbsolutePath("STEMS/beats.wav"));
    stemNames[2] = "beats";
    //bass
    stems[3]->setFile(ofFilePath::getAbsolutePath("STEMS/bass.wav"));
    stemNames[3] = "bass";
    //moog
    stems[4]->setFile(ofFilePath::getAbsolutePath("STEMS/moog.wav"));
    stemNames[4] = "moog";
    //gtr
    stems[5]->setFile(ofFilePath::getAbsolutePath("STEMS/gtr.wav"));
    stemNames[5] = "gtr";
    //gtr solo
    stems[6]->setFile(ofFilePath::getAbsolutePath("STEMS/gtrSolo.wav"));
    stemNames[6] = "gtrSolo";
}

//--------------------------------------------------------------
void audioAnalytics::setupAUGraph(){
    mixer.setInputBusCount(NUMTRACKS);
    
    for ( int i = 0; i < NUMTRACKS; i++ ) {
        stems[i]->connectTo(*taps[i]);
        (*taps[i]).connectTo(mixer, i);
        mixer.setInputVolume(1.0, i);
        mixer.enableInputMetering(i);
    }
    
    mixer.connectTo(output);
    output.start();
}

//--------------------------------------------------------------
void audioAnalytics::playStems(){
    for ( int i = 0; i < NUMTRACKS; i++ ) {
        stems[i]->prime();
    }
    
    for ( int i = 0; i < NUMTRACKS; i++ ) {
        stems[i]->play();
    }
    
    stems[0]->play();
    stems[1]->play();
    stems[2]->play();
    stems[3]->play();
    stems[4]->play();
    stems[5]->play();
    stems[6]->play();
}


///////////////////////////UPDATE//////////////////////////
//--------------------------------------------------------------
void audioAnalytics::updateAnalytics(){
    for ( int i = 0; i < NUMTRACKS; i++ ) {
        
        //volume
        float waveformSize = (selectedTrack == -1) ? ofGetHeight()/NUMTRACKS : ofGetHeight();
        taps[i]->getLeftWaveform(waves[i], ofGetWidth(), waveformSize);///NUMTRACKS
        dB[i] = mixer.getInputLevel(i);
        amp[i] = ofMap(taps[i]->getRMS(0), 0, maxAmp[i], 0.0, 1.0);
        
        dBHistory[i].push_back(dB[i]);
        if (dBHistory[i].size() > ofGetWidth() - 200) dBHistory[i].erase(dBHistory[i].begin());
        
        ampHistory[i].push_back(amp[i]);
        if (ampHistory[i].size() > ofGetWidth() - 200) ampHistory[i].erase(ampHistory[i].begin());
        
        
        //audio features
        taps[i]->getSamples(audioFeatures[i]->inputBuffer);
        audioFeatures[i]->process(0);
        
        medianFilter[i].push_back(audioFeatures[i]->pitch);
        medianFilter[i].erase(medianFilter[i].begin());
        vector<float> MFSorted = medianFilter[i];
        ofSort(MFSorted);
        pitch[i] = MFSorted[3];
        pitchHistory[i].push_back(pitch[i]);
        if (pitchHistory[i].size() > ofGetWidth() - 200) pitchHistory[i].erase(pitchHistory[i].begin());
    }
    
}

//--------------------------------------------------------------
void audioAnalytics::findMinMax(int track, bool save){
    if (save) xml.save("dataMinMax.xml");
    else {
        if (amp[track] > maxAmp[track]) maxAmp[track] = amp[track];
        if (dB[track] > maxdB[track]) maxdB[track] = dB[track];
        
        for (int j = 0; j < audioFeatures[track]->spectrum.size(); j++){
            float bin = audioFeatures[track]->spectrum[j];
            if (bin > maxfft[track]) maxfft[track] = bin;
        }
        
        float pitch = pitchHistory[track][pitchHistory.size()-1];
        if (pitch > maxPitch[track]) maxPitch[track] = pitch;
    }
    
}

//--------------------------------------------------------------
void audioAnalytics::recordPitchData(int track, bool save){
    if (save) xml.save("voice data.xml");
    else {
        if (amp[track] != 0) {
            ofXml frame;
            string child = "frame-" + ofToString(ofGetFrameNum);
            frame.addChild(child);
            frame.setTo(child);
            
            frame.addValue("pitch", pitchHistory[track][pitchHistory.size()-1]);
            frame.addValue("amplitude", amp[track]);
            
            xml.addXml(frame);
        }
    }
}




///////////////////////////DRAW/////////////////////////////
//--------------------------------------------------------------
void audioAnalytics::drawAnalytics(){
    
    ofBackgroundGradient(50, 100);
    
    ofSetColor(255);
    faucet.drawString("all tracks", 30, 20);
    
    
    for ( int i = 0; i < NUMTRACKS; i++ ) {
        ofSetColor(i * 30, 255 - i * 30, (i * 100) % 255);
        ofPushMatrix();
        ofTranslate( 0, ofGetHeight()/(NUMTRACKS) * i);
        faucet.drawString(stemNames[i], 30, ofGetHeight()/NUMTRACKS);
        if (selectedTrack == -1) selectMode(i, ofGetHeight()/NUMTRACKS);
        ofPopMatrix();
    }
    
    if (selectedTrack != -1) {
        int i = selectedTrack;
        ofSetColor(i * 30, 255 - i * 30, (i * 100) % 255);
        selectMode(i, ofGetHeight());
    }
}

//--------------------------------------------------------------
void audioAnalytics::selectMode(int track, float height){
    switch (mode) {
        case 1:
            drawWaveForms(track, height);
            break;
            
        case 2:
            drawAmpHistory(track, height);
            break;
            
        case 3:
            drawDBHistory(track, height);
            break;
            
        case 4:
            drawFFT(track, height);
            break;
            
        case 5:
            drawPitchHistory(track, height);
            break;
    }
}

//--------------------------------------------------------------
void audioAnalytics::drawWaveForms(int track, float height){
    //ofSetLineWidth(10);
    waves[track].draw();
}

//--------------------------------------------------------------
void audioAnalytics::drawAmpHistory(int track, float height){
    ofBeginShape();
    for (unsigned int j = 0; j < ampHistory[track].size(); j++){
        if( j == 0 ) ofVertex(j + 100, height);
        ofVertex(j + 100, height - ampHistory[track][j] * height);
        
        if( j == ampHistory[track].size() -1 ) ofVertex(j + 100, height);
    }
    ofEndShape(false);

    ofRect(ofGetWidth() - 90, height, 80, -amp[track] * height);
//    float size = ampNormalized * height;
    //ofCircle(ofGetWidth() - 50, height - size / 2, size/2);

}

//--------------------------------------------------------------
void audioAnalytics::drawDBHistory(int track, float height){
    ofBeginShape();
    for (unsigned int j = 0; j < dBHistory[track].size(); j++){
        if( j == 0 ) ofVertex(j + 100, height);
        float dBNormalized = ofMap(dBHistory[track][j], -120, maxdB[track], 0.0, 1.0);
        ofVertex(j + 100, height - dBNormalized * height);
        
        if( j == dBHistory[track].size() -1 ) ofVertex(j + 100, height);
    }
    ofEndShape(false);
    
    float dBNormalized = ofMap(dB[track], -120, maxdB[track], 0.0, 1.0);
    ofRect(ofGetWidth() - 90, height, 80, -dBNormalized * height);
//    float size = dBNormalized * height;
//    ofCircle(ofGetWidth() - 50, height - size / 2, size/2);
}

//--------------------------------------------------------------
void audioAnalytics::drawFFT(int track, float height){
    ofBeginShape();
    for (unsigned int j = 0; j < audioFeatures[track]->spectrum.size(); j++){
        int x = (float)j / audioFeatures[track]->spectrum.size() * (ofGetWidth()-100) + 100;
        
        if( j == 0 ) ofVertex(x, height);
        
        float binNormalized = ofMap(audioFeatures[track]->spectrum[j], 0, maxfft[track], 0.0, 1.0);
        ofVertex(x, height - binNormalized * height);
        
        if( j == audioFeatures[track]->spectrum.size() -1 ) ofVertex(x, height);
    }
    ofEndShape(false);
}

//--------------------------------------------------------------
void audioAnalytics::drawPitchHistory(int track, float height){
    ofNoFill();
    ofBeginShape();
    for (unsigned int j = 0; j < pitchHistory[track].size(); j++){
        float pitchNormalized = ofMap(pitchHistory[track][j], 0, maxPitch[track], 0.0, 1.0);
        ofVertex(j + 100, height - pitchNormalized * height);
    }
    ofEndShape(false);
    ofFill();
    
    // ((testApp*)ofGetAppPtr())->.....;
    
}



////////////////////////////EVENTS///////////////////////////////

//--------------------------------------------------------------
void audioAnalytics::setMode(int chosenMode){
    mode = chosenMode;
}

//--------------------------------------------------------------
void audioAnalytics::selectTrack(int x, int y){
    if (x < 100) {
        float height = ofGetHeight() / NUMTRACKS;
        if ( y < 20 ) selectedTrack = -1;
        else if ( y > 20 && y < height) selectedTrack = 0;
        else if ( y > height && y < height * 2) selectedTrack = 1;
        else if ( y > height * 2 && y < height * 3) selectedTrack = 2;
        else if ( y > height * 3 && y < height * 4) selectedTrack = 3;
        else if ( y > height * 4 && y < height * 5) selectedTrack = 4;
        else if ( y > height * 5 && y < height * 6) selectedTrack = 5;
        else if ( y > height * 6 && y < height * 7) selectedTrack = 6;
    }
}


