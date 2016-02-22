#include "ofApp.h"
int maxPS = 4;
int maxPSS = 800;
bool useKinect = false;
int estado = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetWindowTitle("Atiko7-Epicentro");
    img.load("user.png");
    logoAtiko.load("logoAtiko.png");
    
    for(int i=0; i<maxPSS; i++){
        pss.push_back(new ParticleSystem(maxPS, 1));
    }
    
    if (useKinect){
        niContext.setup();
        niDepthGenerator.setup( &niContext );
        niImageGenerator.setup( &niContext );
        niUserGenerator.setup( &niContext );
        niUserGenerator.setUseMaskPixels(true);
        niUserGenerator.setUseCloudPoints(true);
        niContext.setMirror(true);
        niContext.registerViewport();
    }
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    userpixels.allocate(640, 480, 1);
    myImage.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    myImage2.allocate(640, 480, OF_IMAGE_COLOR);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    if (useKinect){
        niContext.update();
        niDepthGenerator.update();
        niImageGenerator.update();
        niUserGenerator.update();
        userpixels.setFromExternalPixels(niUserGenerator.getUserPixels(0), 640, 480, 1);
        myImage.setFromPixels(userpixels);
        myImage.update();
        for (int i=0; i<640; i++) {
            for (int j=0; j<480; j++) {
                if(myImage.getColor(i, j) == ofColor(255)){
                    myImage2.setColor(i, j, ofColor(255, 255, 255));
                }
                else{
                    myImage2.setColor(i, j, ofColor(0, 0, 0));
                }
            }
        }
        myImage2.update();
    }
    if(estado == 1){
        if(useKinect)contourFinder.findContours(myImage2);
        else contourFinder.findContours(img);
    }
    for(int i=0; i<pss.size(); i++){
        pss[i]->update();
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    ofPushMatrix();
    ofScale(ofGetWindowWidth()/640.0, ofGetWindowHeight()/480.0);
    ofEnableBlendMode ( OF_BLENDMODE_ADD ) ;
    ofSetColor(255);
    if(estado == 1){
        if(contourFinder.getContours().size()>0){
            vector<cv::Point> points = contourFinder.getContour(0);
            for(int i=0; i<pss.size(); i++){
                int ind = i*points.size()/pss.size();
                pss[i]->setEmitter(points[ind].x, points[ind].y);
            }
        }
    }
    else if (estado == 0){
        for(int i=0; i<pss.size(); i++){
            pss[i]->setEmitter(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        }
    }
    for(int i=0; i<pss.size(); i++){
        pss[i]->draw();
    }
    //myImage2.draw(0, 0);
    ofPopMatrix();
    ofSetColor(0, 255, 255);
    //contourFinder.draw();
    ofSetColor(255);
    ofDrawBitmapString("FPS: " + ofToString( ofGetFrameRate()), ofGetWidth()-100, 15);
    ofDisableBlendMode();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case '1':
            estado = 1;
            for(int i=0; i<pss.size(); i++){
                pss[i]->updateLifedec(10);
                pss[i]->setEmitter(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
            }
            for(int i=0; i<pss.size(); i+=20){
                pss[i]->updateLifedec(5);
            }
            break;
        case '0':
            estado = 0;
            for(int i=0; i<pss.size(); i++){
                pss[i]->updateLifedec(1);
            }
            break;
    }
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}