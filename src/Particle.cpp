#include "Particle.h"

//------------------------------------------------------------------
Particle::Particle(){
    pColor = ofColor(255, 255, 255);
    bColor = ofColor(255, 255, 255);
    lifespan = 0;
    gravity.x = 0;
    gravity.y = -0.3;
    pos.x = 0;
    pos.y = 0;
}

//------------------------------------------------------------------
Particle::Particle(float ld){
    pColor = ofColor(255, 255, 255);
    lifespan = 0;
    lifedec = ld;
    gravity.x = 0;
    gravity.y = -0.3;
    pos.x = 0;
    pos.y = 0;
}

//------------------------------------------------------------------
void Particle::rebirth(float x, float y){
    pos.x = x;
    pos.y = y;
    partSize = ofRandom(partSizeRef, partSizeRef+10);
    float a = ofRandom(TWO_PI);
    float speed = ofRandom(-0.3, 0.3);
    vel.x = cos(a);
    vel.y = sin(a);
    vel *= speed;
    lifespan = ofRandom(100, 255);
}

//------------------------------------------------------------------
void Particle::updateLifedec(float ld){
    lifedec = ld;
}

//------------------------------------------------------------------
void Particle::updatePartSizeref(float ps){
    partSizeRef = ps;
}

//------------------------------------------------------------------
void Particle::update(){
    lifespan = lifespan - lifedec;
    vel.x += gravity.x;
    vel.y += gravity.y;
    pos.x += vel.x;
    pos.y += vel.y;
    
}
//------------------------------------------------------------------
void Particle::draw(){
    if(!(pColor == ofColor(0) && bColor == ofColor(0))){
        ofPushMatrix();
        ofTranslate(pos.x, pos.y, 0);
        ofNoFill();
        for (int j=1; j<4; j++) {
            ofSetColor(pColor, lifespan*j*0.1);
            ofDrawCircle(0, 0, partSize*(0.45-j*0.09));
        }
        ofSetLineWidth(0);
        ofFill();
        ofSetColor(ofColor(bColor, lifespan));
        ofDrawCircle(0, 0, partSize*0.15*lifespan*0.01);
        ofPopMatrix();
    }
}

//------------------------------------------------------------------
Boolean Particle::isDead() {
    if (lifespan < 0) {
        return true;
    } else {
        return false;
    }
}

//------------------------------------------------------------------
void Particle::setColor(ofColor col){
    pColor = col;
}

//------------------------------------------------------------------
void Particle::setbColor(ofColor col){
    bColor = col;
}
