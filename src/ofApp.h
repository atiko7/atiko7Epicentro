#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "ofxGui.h"
#include "ofxOpenNI.h"
#include "ofxCv.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void resetParticles();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxOpenNIContext                niContext;
    ofxDepthGenerator               niDepthGenerator;
    ofxImageGenerator               niImageGenerator;
    ofxUserGenerator                niUserGenerator;
    ofPixels userpixels;
    
    void trackUser();
    
    ofImage myImage;
    ofImage myImage2;
    ofImage img;
    ofImage logoAtiko;
    ofxCv::ContourFinder contourFinder;

    ParticleSystem *ps;
    vector<ParticleSystem*> pss;
    ofxPanel gui;
    ofxIntSlider guiLife1;
    ofxIntSlider guiSize1;
    ofxIntSlider guiLife2;
    ofxIntSlider guiSize2;
    
};