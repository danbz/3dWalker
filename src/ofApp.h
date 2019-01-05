#pragma once

#include "ofMain.h"


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
    
    int staggerSize, verticalMotion, maxLineWidth;
    ofColor walkerColor;
    vector<ofVec3f> steps;
    vector<int> lineWidth;
    walker(); // constructor
    ~walker(); // destructor
    void addStagger();
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
};

