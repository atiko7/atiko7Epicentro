#pragma once
#include "ofMain.h"

class Particle{
    
public:
    Particle();
    Particle(float lifedec_);

    void rebirth(float x, float y);
    void updateLifedec(float ld);
    void updatePartSizeref(float partsizeref_);
    void updateGravity(ofVec2f g);
    void updateVelocity(ofVec2f v);

    void update();
    void updateWithAngle();
    void draw();
    
    bool isDead();
    bool isInText();

    void setColor(ofColor col);
    void setbColor(ofColor col);
    void getPosition();
    
    
    ofVec2f pos;
    ofVec2f vel;
    ofVec2f gravity;
    
    float partSize=1;
    float partSizeRef=2;
    float lifespan=255;
    float lifedec=30;
    ofColor pColor;
    ofColor bColor;
    
    ofPixels * pixPtr;
};