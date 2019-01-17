#include "ofApp.h"

/*
 Project Title: 3d walker plotting paths for time machine video installation
 Description:
 ©Daniel Buzzo 2019
 dan@buzzo.com
 http://buzzo.com
 https://github.com/danbz
 
 // branch mesh
 // uses mesh triangles to generate walker drawpaths
 */

//--------------------------------------------------------------
void ofApp::setup(){
    numOfWalkers = 1;
    b_drawGui = true; // BOOLEAN (on or off) variable to indicate whether or not to show the gui display
    ofSetBackgroundColor(0, 0, 0);
    b_autoRotate = b_addstagger =  false;
    rotAngle = 0.0;
    ofSetFrameRate(120);
    
    //ofSetLineWidth(3);
    for (int i=0; i<numOfWalkers;i++){
        walker newWalker; // create a walker and push into our vector of 12 walkers
        walkers.push_back(newWalker);
    }
    
    // set DOF parameters

    depthOfField.setup(ofGetWidth(), ofGetHeight());
    focalDist = 200;
    focalRange = 200;
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
    ofVec3f curStep, prevStep(0.0,0.0,0.0);
    ofEnableDepthTest();
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofColor wfColor(50,50,50,255);
    depthOfField.begin();

    
    cam.begin(); // begin using our easyCam objectfor 3D viewing
    // light.enable();
    ofPushMatrix();
    ofRotateYDeg(rotAngle);
    for (int i=0; i<walkers.size();i++){ // iterate though all the values in our vector of steps/points
        ofSetColor(walkers[i].walkerColor);
        walkers[i].mesh.draw();
        
        ofSetColor(walkers[i].walkerColor + wfColor);
        walkers[i].mesh.drawWireframe();
    }
    ofPopMatrix();
    // light.disable();
    cam.end(); // end using our easyCam object
    
    depthOfField.end();
    
    if(ofGetKeyPressed(' ')){
        depthOfField.drawFocusAssist(0, 0);
    }
    else{
        depthOfField.getFbo().draw(0, 0);
    }
    
    ofSetColor(150, 150, 150);
    if (b_drawGui){ // check if we should show the onscreen gui/text
        ofDrawBitmapString(" 'w' to toggle walks, 'r' to autoRotate, 'f' fullscreen, 'g' hide text\nfps: " + ofToString(ofGetFrameRate())  + "\niteration number "+ ofToString(walkers[0].steps.size()), 10, 10);
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
            depthOfField.setup(ofGetWidth(), ofGetHeight());

            break;
            
        case 'g':
            b_drawGui = !b_drawGui; //toggle the guid overlay on and off
            break;
        case OF_KEY_UP:
            focalDist +=10;
            depthOfField.setFocalDistance(focalDist);
            break;
            
        case OF_KEY_DOWN:
            if (focalDist>10) focalDist -=10;
            depthOfField.setFocalDistance(focalDist);
            break;
            
        case OF_KEY_RIGHT:
            focalRange+=10;
            depthOfField.setFocalRange( focalRange );
            break;
            
        case OF_KEY_LEFT:
            if (focalRange>10) focalRange-=10;
            depthOfField.setFocalRange( focalRange );
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
    staggerSize =60; // set how far to stagger when we add staggers to the walk of points
    ofVec3f new3dpoint(ofRandom(-staggerSize, staggerSize),ofRandom(-staggerSize, staggerSize),ofRandom(-staggerSize, staggerSize));
    steps.push_back(new3dpoint);
    verticalMotion = 0.0f;
    maxLineWidth =10;
    lineWidth.push_back(5);
    walkerColor = ofColor(ofRandom(255), ofRandom(255), ofRandom(255), 100);
    mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
    // mesh.setMode(OF_PRIMITIVE_TRIANGLES);

}

//--------------------------------------------------------------
walker::~walker(){
    // destructor
}

//--------------------------------------------------------------
void walker::addStagger(){
    // generate a new small step/stagger that is within +/- our staggerSize distance of the last step
    ofVec3f lastStep = steps[steps.size()-1];
    
    // stochastic random walk
    //ofVec3f newStep(lastStep.x + ofRandom(-staggerSize, staggerSize), lastStep.y + ofRandom(-staggerSize, staggerSize), lastStep.z + ofRandom(-staggerSize, staggerSize  ) + verticalMotion ); // make a new vec2f object and add x,y, z value to it + vector size to give passage in time direction
    
    // perlin noise walk
     ofVec3f newStep(lastStep.x + (ofNoise(lastStep.x)-0.5f) * staggerSize, lastStep.y + (ofNoise(lastStep.y)-0.5f) * staggerSize, lastStep.z + (ofNoise(lastStep.z)-0.5f + verticalMotion)*staggerSize ); // make a new vec2f object and add x,y, z value to it + vector size to give passage in time direction
    
    
    steps.push_back(newStep); // add a new step to the end of our vector of steps
    lineWidth.push_back(ofRandom(maxLineWidth));
    
    //   mesh based line draw - elements from mouse draw oF example
    //get the direction from one to the next.
    //the ribbon should fan out from this direction
    ofVec3f direction = (newStep - lastStep);
    
    //get the distance from one point to the next
    float distance = direction.length();
    
    //get the normalized direction. normalized vectors always have a length of one
    //and are really useful for representing directions as opposed to something with length
    ofVec3f unitDirection = direction.getNormalized();
    
    //find both directions to the left and to the right
    ofVec3f toTheLeft = unitDirection.getRotated(-90, ofVec3f(0,0,1));
    ofVec3f toTheRight = unitDirection.getRotated(90, ofVec3f(0,0,1));
    
    //use the map function to determine the distance.
    //the longer the distance, the narrower the line.
    //this makes it look a bit like brush strokes
    // float thickness = ofMap(distance, 0, 60, 10, 2, true);
    // float thickness = ofRandom(maxLineWidth);
    float thickness = ofNoise(lastStep.x) * maxLineWidth;
    //calculate the points to the left and to the right
    //by extending the current point in the direction of left/right by the length
    ofVec3f leftPoint = lastStep+toTheLeft*thickness;
    ofVec3f rightPoint = lastStep+toTheRight*thickness;
    
    //add these points to the triangle strip
    mesh.addVertex(ofVec3f(leftPoint.x, leftPoint.y, leftPoint.z));
    mesh.addVertex(ofVec3f(rightPoint.x, rightPoint.y, rightPoint.z));
    
    
}
