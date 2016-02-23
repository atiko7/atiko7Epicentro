#ifndef PARTICLELOGO_H
#define PARTICLELOGO_H

#include "ofMain.h"

class ParticleLogo {
public:
    ParticleLogo();
    
    void setup(ofPixels * _pix, ofColor _c);
    void update();
    void display();
    void getPosition();
    bool isInText();
    
    ofPixels * pixPtr;
    ofColor c;
    
    ofVec2f loc;
    ofVec2f vel;
    
    float maxLife;
    float life;
    float lifeRate;
};

#endif // PARTICLELOGO_H
