#include "ofApp.h"
int maxPS = 1;
int maxPSS = 15000;
int estado = 2;
int lastEstado = 2;
int timeChangeToZero = 0;
int lastTimeChangeToZero = 0;
int lastTimeChange = 0;
bool useKinect = false;
int lastNumUsers = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    //------------- OSC
    receiver.setup(PORT);
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetWindowTitle("Atiko7-Epicentro");
    atikoPlayer.load("vid_atiko_f.mp4");
    epicentroPlayer.load("vid_epicentro_f.mp4");
    atikoPlayer.play();
    epicentroPlayer.play();

    img.load("user.png");
    logoAtiko.load("logo_atiko.png");
    logoEpicentro.load("logo_epicentro.png");
    logoAtiko.setAnchorPercent(0.5, 0.5);
    logoEpicentro.setAnchorPercent(0.5, 0.5);
    
    fbo.allocate(640, 480, GL_RGBA);
    pix.allocate(640, 480, OF_PIXELS_RGBA);
    fbo2.allocate(640, 480, GL_RGBA);
    pix2.allocate(640, 480, OF_PIXELS_RGBA);
    
    fbo.begin();
    ofClear(0, 0, 0, 0);
    logoAtiko.draw(320, 240, logoAtiko.getWidth(), logoAtiko.getHeight());
    fbo.end();
    fbo.readToPixels(pix);
    
    fbo2.begin();
    ofClear(0, 0, 0, 0);
    logoEpicentro.draw(320, 240, logoEpicentro.getWidth(), logoEpicentro.getHeight());
    fbo2.end();
    fbo2.readToPixels(pix2);
    
    for(int i=0; i<maxPSS; i++){
        pss.push_back(new ParticleSystem(maxPS, 1));
    }
    
    contourFinder.setMinAreaRadius(10);
    contourFinder.setMaxAreaRadius(200);
    userpixels.allocate(640, 480, 1);
    myImage.allocate(640, 480, OF_IMAGE_GRAYSCALE);
    myImage2.allocate(640, 480, OF_IMAGE_COLOR);
    
    
    //------------- Kinect
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
}

//--------------------------------------------------------------
void ofApp::update(){
    //------------- Kinect
    if (useKinect){
        niContext.update();
        niDepthGenerator.update();
        niImageGenerator.update();
        niUserGenerator.update();
        userpixels.setFromExternalPixels(niUserGenerator.getUserPixels(), 640, 480, 1);
        myImage.setFromPixels(userpixels);
        myImage.update();
        
        int currentNumUsers = niUserGenerator.getNumberOfTrackedUsers();
        ofLog(OF_LOG_NOTICE, ofToString(currentNumUsers));
        if(currentNumUsers != 0){
            contourFinder.findContours(myImage);
            if(lastNumUsers == 0 && contourFinder.getContours().size()>0 && estado!=3){
                lastNumUsers == currentNumUsers;
                genEstado(3);
            }
        }
        else{
            lastNumUsers = 0;
        }
    }
    checkTimes();
    
    if(estado == 0){
        for(int i=0; i<pss.size(); i++){
            pss[i]->setEmitter(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        }
    }
    else if(estado == 1 || estado == 2){
        for(int i=0; i<pss.size(); i++){
            pss[i]->setEmitter(pss[i]->pos.x, pss[i]->pos.y);
        }
    }
    else if(estado == 3){
        if(useKinect)contourFinder.findContours(myImage);
        else contourFinder.findContours(img);
        if(contourFinder.getContours().size()>0){
            for(int i=0; i<contourFinder.getContours().size();i++){
            vector<cv::Point> points = contourFinder.getContour(i);
                for(int j=0; j<pss.size(); j++){
                    int ind = j*points.size()/pss.size();
                    pss[j]->setEmitter(points[ind].x, points[ind].y);
                }
            }
        }
        if(contourFinder.getContours().size()==0){
            genEstado(0);
            lastTimeChange = 0;
            lastTimeChangeToZero = ofGetElapsedTimeMillis();
            lastEstado = 1;
        }
    }
    for(int i=0; i<pss.size(); i++){
        pss[i]->update();
    }
    atikoPlayer.update();
    epicentroPlayer.update();
}

//--------------------------------------------------------------
void ofApp::checkTimes(){
    if(estado!=3){
        if(estado != 0 && ofGetElapsedTimeMillis() - lastTimeChange > 8000){
            lastTimeChange = ofGetElapsedTimeMillis();
            lastTimeChangeToZero = ofGetElapsedTimeMillis();
            lastEstado = estado;
            genEstado(0);
        }
        else if(estado == 0 && ofGetElapsedTimeMillis() - lastTimeChangeToZero > 2500){
            if(lastEstado == 1 )genEstado(2);
            else if(lastEstado ==  2 )genEstado(1);
            lastTimeChange = ofGetElapsedTimeMillis();
        }
    }
}


//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0, 0, 0);
    if(estado==3){
        ofPushStyle();
        ofPushMatrix();
        ofTranslate(0.05*ofGetWindowWidth(), 0);
        ofScale(ofGetWindowWidth()/(atikoPlayer.getWidth()*5), ofGetWindowHeight()/(atikoPlayer.getHeight()*4.5));
        atikoPlayer.draw(0, 0);
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(0.8*ofGetWindowWidth(), 0);
        ofScale(ofGetWindowWidth()/(epicentroPlayer.getWidth()*5), ofGetWindowHeight()/(epicentroPlayer.getHeight()*5));
        epicentroPlayer.draw(0, 0);
        ofPopMatrix();
        ofPopStyle();
    }
    ofPushStyle();
    ofPushMatrix();
    ofScale(ofGetWindowWidth()/640.0, ofGetWindowHeight()/480.0);
    ofEnableBlendMode(OF_BLENDMODE_ADD ) ;
    ofSetColor(255);
    for(int i=0; i<pss.size(); i++){
        pss[i]->draw();
    }
    ofDisableBlendMode();
    ofPopMatrix();
    ofPopStyle();
    

//    ofSetColor(0, 255, 255);
//    ofSetColor(255);
//    ofDrawBitmapString("FPS: " + ofToString( ofGetFrameRate()), ofGetWidth()-100, 15);
    
    
}
//--------------------------------------------------------------
void ofApp::genEstado(int est){
    
    if(est==0){
        estado = 0;
        for(int i=0; i<pss.size(); i++){
            pss[i]->updateLifedec(1);
            pss[i]->updateGravity(ofVec2f(0, -0.2));
            pss[i]->setEmitter(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        }
    }
    else if(est==1){
        estado = 1;
        generateParticlesLogo(&pix);
    }
    else if(est==2){
        estado = 2;
        generateParticlesLogo(&pix2);
    }
    else if(est==3){
        estado = 3;
        for(int i=0; i<pss.size(); i++){
            pss[i]->updateLifedec(20);
            pss[i]->updateGravity(ofVec2f(0, -0.4));
            pss[i]->setEmitter(ofRandom(ofGetWidth()), ofRandom(ofGetHeight()));
        }
        for(int i=0; i<pss.size(); i+=20){
            pss[i]->updateGravity(ofVec2f(0, -0.3));
            pss[i]->updateLifedec(5);
        }
    }
    ofLog(OF_LOG_NOTICE, "ESTADO:" + ofToString(estado));
}

void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
        case '0':
            genEstado(0);
            break;
        case '1':
            genEstado(1);
            break;
        case '2':
            genEstado(2);
            break;
        case '3':
            genEstado(3);
            break;
    }
}

//--------------------------------------------------------------
void ofApp::generateParticlesLogo(ofPixels * _pix){
    int ind = 0;
    for(int x=0; x < _pix->getWidth(); x+=int(ofRandom(1, 2))){
        for(int y=0; y < _pix->getHeight(); y+=int(ofRandom(1, 3))){
            if(_pix -> getColor(x, y) == ofColor(255) && ind < pss.size()){
                pss[ind]->setEmitter(x, y);
                pss[ind]->updateLifedec(7);
                pss[ind]->updatePartSizeref(1);
                pss[ind]->updateGravity(ofVec2f(0, 0));
                ind++;
            }
        }
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