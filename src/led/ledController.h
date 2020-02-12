#pragma once

#include "ofMain.h"
#include "ofxOPC.h"
#include "ofxTween.h"

namespace led{

class LedController : public ofBaseApp{

/* METHOD */
public:
    void setup(const string ip, const int port, const int _numberofleds, const int _fadecandys);
    void update();
    void draw();
    void exit();
    
    void startPattern(int mode, int duration, ofColor color);

private:
    void drawPatterns();
    // Tween Callback Method
    void fadeTweenCallback(int &e);
    

/* MEMBER */
public:
    int effect;
    bool isHideGrid;
    bool isHideEffect;
    
private:
    
    ofxOPC opcClient;
    Effects defaultEffects;
    map<int, ofxNeoPixelObject*> neoPixelObjects;
    
    int numberofleds;
    int fadecandys;

    ofVec2f layoutStart;
    int layoutStep;
    int ledStep;
    int ledSize;
    
    int patternMode;
    ofColor currentColor;
    ofColor nextColor;

    vector <ofColor> mergedColor;
    
    // Tween
    ofxEasingLinear easingLinear;
    ofxEasingCirc easingCirc;
    ofxEasingExpo easingExpo;
    ofxTween fadeTween;
    ofxTween beamInTween;
    ofxTween beamOutTween;


};

}
