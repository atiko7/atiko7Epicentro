#pragma once
#include "ofMain.h"

class Particle{
    
public:
    Particle();
    Particle(float lifedec_);
    void rebirth(float x, float y);
    void updateLifedec(float ld);
    void updatePartSizeref(float partsizeref_);
    void update();
    void draw();
    Boolean isDead();
    void setColor(ofColor col);
    void setbColor(ofColor col);
    
    
    ofPoint pos;
    ofPoint vel;
    ofPoint gravity;
    
    float partSize=1;
    float partSizeRef=2;
    float lifespan=255;
    float lifedec=30;
    ofColor pColor;
    ofColor bColor;
    
};