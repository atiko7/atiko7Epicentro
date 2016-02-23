#pragma once
#include "ofMain.h"
#include "Particle.h"

class ParticleSystem{
    
public:
    ParticleSystem(int n);
    ParticleSystem(int n, float lifedec_);

    void reset();
    void update();
    void updateWithAngle();
    void updateLifedec(float ld);
    void updatePartSizeref(float ps);
    void draw();
    void setColor(ofColor col);
    void setEmitter(float x, float y);
    void updateGravity(ofVec2f g);
    void updateVelocity(ofVec2f v);

    vector <Particle*> particles;
    int nParticles;
    ofVec2f pos;
    
    ofPixels * pixPtr;
};