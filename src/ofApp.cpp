#include "ofApp.h"

/*
 Project Title: 3d walker plotting paths for time machine video installation
 Description:
 ©Daniel Buzzo 2019
 dan@buzzo.com
 http://buzzo.com
 https://github.com/danbz
 */

//--------------------------------------------------------------
void ofApp::setup(){
    numOfWalkers = 12;
    b_drawGui = true; // BOOLEAN (on or off) variable to indicate whether or not to show the gui display
    ofSetBackgroundColor(0, 0, 0);
    b_autoRotate = b_addstagger =  false;
    rotAngle = 0.0;
    ofSetFrameRate(120);
    
    ofSetLineWidth(3);
    for (int i=0; i<numOfWalkers;i++){
        walker newWalker; // create a walker and push into our vector of 12 walkers
        walkers.push_back(newWalker);
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    if (b_addstagger){
        for (int i=0; i < walkers.size(); i++){
            walkers[i].addStagger();
        }
    }
    if (b_autoRotate){
        rotAngle +=0.1 ;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    cam.begin(); // begin using our easyCam objectfor 3D viewing
    
    ofPushMatrix();
    ofRotateYDeg(rotAngle);
    ofVec3f curStep, prevStep(0.0,0.0,0.0);
    for (int i=0; i<walkers.size();i++){ // iterate though all the values in our vector of steps/points
        ofSetColor(walkers[i].walkerColor);
        for (int j=0; j<walkers[i].steps.size();j++){
            curStep = walkers[i].steps[j];
            ofSetLineWidth(walkers[i].lineWidth[j]);
            ofDrawLine(prevStep, curStep); // draw a line between the last point and the current point
            prevStep=curStep;
            
        }
    }
    ofPopMatrix();
    cam.end(); // end using our easyCam object
    
    if (b_drawGui){ // check if we should show the onscreen gui/text
        ofDrawBitmapString(" 'w' to toggle walks, 'r' to autoRotate, 'f' fullscreen, 'g' hide text\nfps: " + ofToString(ofGetFrameRate())  + "\niteration number "+ ofToString(walkers[1].steps.size()), 10, 10);
    }
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch (key) {
            
        case 'r':
            b_autoRotate=!b_autoRotate;
            break;
            
        case 'w':
            b_addstagger=!b_addstagger;
            break;
            
        case 'f':
            ofToggleFullscreen(); // toggle full screen display
            break;
            
        case 'g':
            b_drawGui = !b_drawGui; //toggle the guid overlay on and off
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
walker::walker(){ // constructor
    ofVec3f new3dpoint(0,0,0);
    steps.push_back(new3dpoint);
    staggerSize =2; // set how far to stagger when we add staggers to the walk of points
    verticalMotion = 0.1;
    maxLineWidth =40;
    lineWidth.push_back(5);
    walkerColor = ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 100);
}

//--------------------------------------------------------------
walker::~walker(){
    // destructor
}

//--------------------------------------------------------------
void walker::addStagger(){
    // generate a new small step/stagger that is within +/- our staggerSize distance of the last step
    ofVec3f lastStep = steps[steps.size()-1];
    ofVec3f newStep(lastStep.x + ofRandom(-staggerSize, staggerSize), lastStep.y + ofRandom(-staggerSize, staggerSize), lastStep.z + ofRandom(-staggerSize, staggerSize  ) + verticalMotion ); // make a new vec2f object and add x,y, z value to it + vector size to give passage in time direction
    steps.push_back(newStep); // add a new step to the end of our vector of steps
    lineWidth.push_back(ofRandom(maxLineWidth));
    
}
