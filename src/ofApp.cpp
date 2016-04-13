#include "ofApp.h"
using namespace ofxCv;
using namespace cv;
int maxPS = 1;
int maxPSS = 20000;
int estado = 2;
int lastEstado = 2;
int timeChangeToZero = 0;
int lastTimeChangeToZero = 0;
int lastTimeMovHigh = 0;
int lastTimeChange = 0;
bool useKinect = true;
int lastNumUsers = 0;

//--------------------------------------------------------------
void ofApp::setup(){
    //------------- OSC
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofSetWindowTitle("Atiko7-Epicentro");
    atikoPlayer.load("vid_atiko_f_2.mp4");
    epicentroPlayer.load("vid_epicentro_f_2.mp4");
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
    
    imitate(previous, myImage);
    imitate(diff,myImage);
    
    
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
        userpixels.setFromExternalPixels(niUserGenerator.getUserPixels(0), 640, 480, 1);
        myImage.setFromPixels(userpixels);
        myImage.update();
        
       
        int currentNumUsers = niUserGenerator.getNumberOfTrackedUsers();

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
    calcMovement();
    ofLog(OF_LOG_NOTICE, "MOV: " + ofToString(mov));
    if(mov > 20){
        lastTimeMovHigh = ofGetElapsedTimeMillis();
    }
    
    if(estado == 0){
        for(int i=0; i<pss.size(); i++){
            pss[i]->setEmitter(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
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
        int numContours = contourFinder.getContours().size();
        if(numContours > 0){
            int numPartPerContour = pss.size()/numContours;
//*****************************************************************************// CAMBIO
            for(int i = 0; i < numContours; i++){
                vector<cv::Point> points = contourFinder.getContour(i); //cambiar a i
                ofLog(OF_LOG_NOTICE, ofToString(points.size()));
                for(int j=0; j<numPartPerContour; j++){
                    int ind = j*points.size()/numPartPerContour;
                    pss[i*numPartPerContour+j]->setEmitter(points[ind].x, points[ind].y);
                }
            }
//*****************************************************************************// CAMBIO

        }
        if(contourFinder.getContours().size()==0 || ofGetElapsedTimeMillis()-lastTimeMovHigh > 10000){
            genEstado(0);
            lastTimeChangeToZero = 0;
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
        if(estado != 0 && ofGetElapsedTimeMillis() - lastTimeChange > 5000){
            lastTimeChange = ofGetElapsedTimeMillis();
            lastTimeChangeToZero = ofGetElapsedTimeMillis();
            lastEstado = estado;
            genEstado(0);
        }
        else if(estado == 0 && ofGetElapsedTimeMillis() - lastTimeChangeToZero > 1500){
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
        ofTranslate(0.05*ofGetWindowWidth(), -0.023*ofGetWindowHeight());
        ofScale(ofGetWindowWidth()/(atikoPlayer.getWidth()*3.75), ofGetWindowHeight()/(atikoPlayer.getHeight()*3.75));
        atikoPlayer.draw(0, 0);
        ofPopMatrix();
        ofPushMatrix();
        ofTranslate(0.75*ofGetWindowWidth(), 0);
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
    
    myImage.draw(ofGetWindowWidth()-320, ofGetWindowHeight()-240, 320, 240);
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
            pss[i]->setEmitter(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
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
        lastTimeMovHigh = ofGetElapsedTimeMillis();
        estado = 3;
        for(int i=0; i<pss.size(); i++){
            pss[i]->updateLifedec(20);
            pss[i]->updateGravity(ofVec2f(0, -0.4));
            pss[i]->setEmitter(ofRandom(ofGetWindowWidth()), ofRandom(ofGetWindowHeight()));
        }
        for(int i=0; i<pss.size(); i+=20){
            pss[i]->updateGravity(ofVec2f(0, -0.3));
            pss[i]->updateLifedec(5);
        }
    }
    //ofLog(OF_LOG_NOTICE, "ESTADO:" + ofToString(estado));
}

void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            atikoPlayer.load("vid_atiko_f_2.mp4");
            epicentroPlayer.load("vid_epicentro_f_2.mp4");
            atikoPlayer.play();
            epicentroPlayer.play();
            break;
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
    for(int x=0; x < _pix->getWidth(); x++/*int(ofRandom(1, 2))*/){
        for(int y=0; y < _pix->getHeight(); y++/*int(ofRandom(1, 2))*/){
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

void ofApp::calcMovement(){
    absdiff(myImage, previous, diff);
    diff.update();
    copy(myImage, previous);
    diffMean = mean(toCv(diff));
//    diffMean *= Scalar(50);
    mov = diffMean[0];
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