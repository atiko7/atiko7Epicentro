#include "ParticleLogo.h"

ParticleLogo::ParticleLogo(){
    pixPtr = NULL;
}

//--------------------------------------------------------------
void ParticleLogo::setup(ofPixels * _pix, ofColor _c){
    pixPtr = _pix;
    c = _c;
    
    getPosition();
    maxLife = 1.25;
    life = ofRandom(0.5 * maxLife, maxLife);
    lifeRate = ofRandom(0.01, 0.02);
}

//--------------------------------------------------------------
void ParticleLogo::update(){
    // the velocity/direction of each ParticleLogo is based on a flowfield using OpenFrameworks's ofNoise() method
    // drawMode 0: no extras (an xy-position will always return the same angle)
    // drawMode 1: dynamic noise (an xy-position will return a slightly different angle on every frame)
    // drawMode 2: rotation (the angle of each xy-position is globally rotated)
    // drawMode 3: dynamic noise + rotation (combines drawModes 1 & 2)
    float angle = ofNoise(loc.x * 0.01, loc.y * 0.01, 0) * 360;
    vel.set(1, 0);
    vel.rotate(angle);
    loc += vel;
    life -= lifeRate;
    if(life < 0){
        getPosition();
        life = ofRandom(0.5 * maxLife, maxLife);
    }
}

//--------------------------------------------------------------
void ParticleLogo::display(){
    float r = 8 * life / maxLife; // radius of the ellipse
    ofFill();
    ofSetColor(255);
    ofEllipse(loc.x, loc.y, r, r); // draw ellipse
    ofNoFill();
    ofSetColor(0, 135);
    ofEllipse(loc.x, loc.y, r, r); // draw ellipse
}

//--------------------------------------------------------------
void ParticleLogo::getPosition(){
    // get a random position inside the text
    while(!isInText()){
        loc.set(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
    }
}

//--------------------------------------------------------------
bool ParticleLogo::isInText(){
    if(pixPtr){
        return (pixPtr->getColor(loc.x, loc.y) == c);
    }
    else{
        return true;
    }
}
