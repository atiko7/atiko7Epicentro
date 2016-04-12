#include "Particle.h"

//------------------------------------------------------------------
Particle::Particle(){
    pColor = ofColor(200, 100, 0);
    bColor = ofColor(200, 100, 0);
    lifespan = 0;
    partSize = ofRandom(partSizeRef, partSizeRef+10);
    gravity.set(0, -0.3);
    pos.set(0, 0);
}

//------------------------------------------------------------------
Particle::Particle(float ld){
    pColor = ofColor(200, 100, 0);
    bColor = ofColor(200, 100, 0);
    lifespan = 0;
    lifedec = ld;
    partSize = ofRandom(partSizeRef, partSizeRef+10);
    gravity.set(0, -0.3);
    pos.set(0, 0);
}

//------------------------------------------------------------------
void Particle::rebirth(float x, float y){
    pos.set(x, y);
    float a = ofRandom(TWO_PI);
    float speed = ofRandom(-0.3, 0.3);
    vel.set(cos(a), sin(a));
    vel *= speed;
    lifespan = ofRandom(100, 255);
}

//------------------------------------------------------------------
void Particle::updateLifedec(float ld){
    lifedec = ld;
}

//------------------------------------------------------------------
void Particle::updateGravity(ofVec2f g){
    gravity.set(g.x, g.y);
}

//------------------------------------------------------------------
void Particle::updateVelocity(ofVec2f v){
    vel.set(v.x, v.y);
}

//------------------------------------------------------------------
void Particle::updatePartSizeref(float ps){
    partSizeRef = ps;
}

//------------------------------------------------------------------
void Particle::update(){
    lifespan = lifespan - lifedec;
    vel += gravity;
    pos += vel;
}


//------------------------------------------------------------------
void Particle::draw(){
    if(!(pColor == ofColor(0) && bColor == ofColor(0))){
        ofPushMatrix();
        ofTranslate(pos.x, pos.y);
        ofNoFill();
        ofRotate(lifespan);
        for (int j=1; j<4; j++) {
            ofSetColor(pColor, lifespan*j*0.1);
            ofRect(0, 0, partSize*(0.55-j*0.05), partSize*(0.055-j*0.05));
        }
        ofSetLineWidth(0);
        ofFill();
        ofSetColor(ofColor(bColor, lifespan));
        ofRect(0, 0, partSize*0.25*lifespan*0.01, partSize*0.025);
        ofPopMatrix();
    }
}

//------------------------------------------------------------------
bool Particle::isDead() {
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
