//
//  audioAnalytics.h
//  just_you
//
//  Created by Jason Levine on 2013-11-06.
//
//
#pragma once

#include "ofMain.h"
#include "ofxAudioUnit.h"
#include "ofxAudioFeaturesChannel.h"

class audioAnalytics {
public:
    audioAnalytics();
    
    void setupVectors();
    void loadTracks();
    void setupAUGraph();
    void playStems(float timeInSeconds);
    void stopStems();
    
    void updateAnalytics();
    void findMinMax(int track, bool save);
    void recordPitchData(int track, bool save);
    
    void setMode(int chosenMode);
    void selectTrack(int x, int y);
    void drawAnalytics();
    void selectMode(int track, float height);
    void drawWaveForms(int track, float height);
    void drawAmpHistory(int track, float height);
    void drawDBHistory(int track, float height);
    void drawFFT(int track, float height);
    void drawPitchHistory(int track, float height);
    
    vector<ofxAudioUnitFilePlayer*> stems;
    vector<string> stemNames;
    vector<ofxAudioUnitTap*> taps;
    vector<ofxAudioUnitFftNode*> ffts;
    vector<ofPolyline> waves;
    ofxAudioUnitMixer mixer;
    ofxAudioUnitOutput output;

    vector<ofxAudioFeaturesChannel*> audioFeatures;

    vector<float> dB;
    vector<float> amp;
    vector<float> pitch;
    vector<vector<float> > ampHistory;
    vector<vector<float> > dBHistory;
    vector<vector<float> > pitchHistory;

    vector<vector<float>  > medianFilter;

    int mode;
    int selectedTrack;

    //analytics
    vector <float> maxdB, maxAmp, maxfft, maxPitch;
    float maxdBGlobal, maxAmpGlobal, maxfftGlobal, maxPitchGlobal;
    //xml
    ofXml xml;

    ofTrueTypeFont faucet;
};
