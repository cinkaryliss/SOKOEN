#pragma once

#include "ofMain.h"
#include "ofxOpenNI.h"
#include "ofxOsc.h"

#define PORT 8000

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
    
    ofxOpenNI kinect;
    ofTrueTypeFont font, figfont;
    ofPoint headpos1, headpos2;
    ofSoundPlayer add, more_add, start, end;
    ofxOscReceiver receiver;
    ofxOscMessage m;
    int point1, point2;
    float timer;
    float fontscale1, fontscale2;
    float cw1, cw2, cwwin, cwlose, cwdraw, cwt, ch; //文字幅と文字高の補正項
    float scale, offsetX, offsetY;
    bool hide, battle, result, replace;
};
