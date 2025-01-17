#pragma once

#include "ofMain.h"
#include "led/ledController.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
	
		void exit();
    
    led::LedController ledController;
    string choicedMode;
};
