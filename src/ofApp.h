#pragma once

#include "ofMain.h"
#include "ofxDOF.h"


/*
 Project Title: 3d walker plotting paths for time machine video installation
 Description:
 ©Daniel Buzzo 2019
 dan@buzzo.com
 http://buzzo.com
 https://github.com/danbz
 */

class walker{
    
public:
    
    float staggerSize, verticalMotion, maxLineWidth;
    ofColor color;
    vector<ofVec3f> steps;
    vector<int> lineWidth;
    ofMesh mesh;
    walker(); // constructor
    ~walker(); // destructor
    void addStagger();
    
    // for building rings
    float pie, radius, tubeHeightStep;
    int tubeMeshRes, tubeHeight, randomKey;
    
    void triangulateMesh();
    void addRing();
};

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    vector<walker> walkers;
    int numOfWalkers;
    float rotAngle;
    bool b_drawGui, b_addstagger, b_autoRotate;
    ofEasyCam cam;
    ofLight light;
    
    int focalDist, focalRange;
    ofxDOF depthOfField;
    
    
};

