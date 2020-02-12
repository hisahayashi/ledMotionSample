#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
    
    ledController.setup("127.0.0.1", 7890, 64, 2);
}

//--------------------------------------------------------------
void ofApp::update()
{
    ledController.update();
}

//--------------------------------------------------------------
void ofApp::draw()
{
    ledController.draw();
    
    // Report Messages
    float traceX = 15;
    float traceY = ofGetHeight() - 15;
    float lineHeight = 20;
    ofDrawBitmapStringHighlight("FPS: " + ofToString((int)(ofGetFrameRate())), traceX, traceY);
    ofDrawBitmapStringHighlight("e : Effect ON/OFF.", traceX, traceY -= lineHeight);
    ofDrawBitmapStringHighlight("h : Grid ON/OFF.", traceX, traceY -= lineHeight);
    ofDrawBitmapStringHighlight("<- -> : Change Effect.", traceX, traceY -= lineHeight);
    ofDrawBitmapStringHighlight("Mode: " + choicedMode, traceX, traceY -= lineHeight);
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
        case OF_KEY_LEFT:
            ledController.effect--;
            break;
        case OF_KEY_RIGHT:
            ledController.effect++;
            break;
        case 'h':
            ledController.isHideGrid = !ledController.isHideGrid;
            break;
        case 'e':
            ledController.isHideEffect = !ledController.isHideEffect;
            break;
            
        /*
         0: Fade
         1: Beam
         */
            
        case '1':
            // Fade to Red
            ledController.startPattern(0, 1000, ofColor(255, 0, 0));
            choicedMode = "Fade to Red";
            break;
        case '2':
            // Fade to Black
            ledController.startPattern(0, 1000, ofColor(0, 0, 0));
            choicedMode = "Fade to Black";
            break;
            // Beam
        case '3':
            ledController.startPattern(1, 1000, ofColor(255, 255, 0));
            choicedMode = "Beam Yellow (Front to Back)";
            break;
            // Beam
        case '4':
            ledController.startPattern(1, 300, ofColor(0, 100, 230));
            choicedMode = "Beam Blue (Front to Back)";
            break;
            
        default:
            break;
    }
}
//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
	
}
//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::exit()
{
    ledController.exit();
}
