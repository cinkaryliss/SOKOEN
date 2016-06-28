#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
//#include "ofxGui.h"

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
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    //ofxPanel gui;
    //ofxSlider<float> scaleSlider1, scaleSlider2, scaleSlider3;
    ofxOpenNI kinect;
    ofTrueTypeFont font;
    ofPoint headpos1, headpos2;
    ofSoundPlayer add, more_add, start, end;
    int point1, point2;
    float timer;
    float fontscale1, fontscale2;
    float cw1, cw2, cwwin, cwlose, ch;
    float scale, offsetX, offsetY;
    bool hide, battle, result, exc;
};
