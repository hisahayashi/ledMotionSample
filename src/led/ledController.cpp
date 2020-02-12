#include "ledController.h"

namespace led{

//--------------------------------------------------------------
void LedController::setup(const string ip, const int port, const int _numberofleds, const int _fadecandys)
{
    numberofleds = _numberofleds;
    fadecandys = _fadecandys;
    
	effect = 0;
	isHideGrid = false;
    isHideEffect = true;
    
    patternMode = 0;
    currentColor = ofColor(0);
    nextColor = ofColor(0);
	
	// Connect to the fcserver
	opcClient.setup(ip, port, fadecandys);
	
	// You define the stage size and draw into the stage
	opcClient.setupStage(500, 500);
	
	defaultEffects.setup(opcClient.getStageCenterX(), opcClient.getStageCenterY(), opcClient.getStageWidth(), opcClient.getStageHeight());
	
    layoutStart = ofVec2f(50, 50);
    layoutStep = 50;
    ledStep = 2;
    ledSize = 8;
	
	for (int i = 0; i < ledSize; i++)
	{
		float angle = ofMap(i, 0, ledSize, 0, M_TWO_PI);
        int startX = layoutStart.x + layoutStep * (i % (ledSize / 2));
        int startY = layoutStart.y + 0;
        
        if(i >= ledSize / 2){
            startY = numberofleds * ledStep + layoutStart.y + 50;
        }
		
		vector <ofPoint> pts;
		for (int j = 0; j < numberofleds; j++)
		{
            ofPoint p(startX, startY + (j * ledStep));
			pts.push_back(p);
		}
		neoPixelObjects.insert(std::pair<int, ofxNeoPixelObject*>(i, new ofxNeoPixelCustomObject(0, 0, pts, numberofleds, 5)));
	}
	
    // Tween listener
    ofAddListener(fadeTween.end_E, this, &LedController::fadeTweenCallback);
}

//--------------------------------------------------------------
void LedController::update()
{
    fadeTween.update();
    beamInTween.update();
    beamOutTween.update();
    
	ofSetWindowTitle("example_multiple_fadecandy_units");
	opcClient.update();
	
	// Now Draw the effects to the stage
	opcClient.beginStage();
    
    drawPatterns();
    
    if(!isHideEffect){
        defaultEffects.draw(effect);
    }
	
	opcClient.endStage();
//	
//	mergedColor.clear();
//	
	// Get Objects
	for (int i = 0; i < neoPixelObjects.size(); i++)
	{
		opcClient.getStagePixels(neoPixelObjects.at(i)->getPixelCoordinates(), neoPixelObjects.at(i)->colors);
	}
//	
//	// If the client is not connected do not try and send information
//	if (!opcClient.isConnected())
//	{
//		// Will continue to try and reconnect to the Pixel Server
//		opcClient.tryConnecting();
//	}
//	else
//	{
//		// Loop through the NeoPixel Objects and store the number of pixels in each one
//		// Then copy the color data across from each object on to the end of the merge vector
//		for (int i = 0; i < neoPixelObjects.size(); i++)
//		{
//			// I know this is resource heavy but insert was causing odd issues with the size of the Vector
//			for(int e = 0; e < neoPixelObjects.at(i)->colorData().size(); e++)
//			{
//				mergedColor.push_back(neoPixelObjects.at(i)->colorData()[e]);
//			}
//		}
//		
//		opcClient.autoWriteData(mergedColor);
//	}
	
	// If the client is not connected do not try and send information
	if (!opcClient.isConnected())
	{
		// Will continue to try and reconnect to the Pixel Server
		opcClient.tryConnecting();
	}
	else
	{
		// Write out the first set of data
		for (int i = 0; i < neoPixelObjects.size(); i++)
		{
			opcClient.writeChannel(i+1, neoPixelObjects[i]->colorData());
		}
	}
}

//--------------------------------------------------------------
void LedController::draw()
{
	ofBackground(0);
	ofPushStyle();
    
    if(isHideGrid){
        opcClient.drawStage(false);
    }
    else{
        opcClient.drawStage(true);
    }
	
	for (int i = 0; i < neoPixelObjects.size(); i++)
	{
        if(isHideGrid){
            neoPixelObjects[i]->drawGrabRegion(false);
        }
        else{
            neoPixelObjects[i]->drawGrabRegion(true);
        }
	}
	
	for(int i = 0; i < neoPixelObjects.size(); i++)
	{
		neoPixelObjects.at(i)->draw(opcClient.getStageWidth()+(i * 25), 10);
	}
    
	ofPopStyle();
}

//--------------------------------------------------------------
void LedController::exit()
{
	// Close Connection
	opcClient.close();
}

//--------------------------------------------------------------
void LedController::startPattern(int mode, int duration, ofColor color){
    ofLog() << ">> startPattern";
    
    patternMode = mode;
    currentColor = nextColor;
    nextColor = color;
    
    fadeTween.setParameters(0, easingLinear, ofxTween::easeOut, 0.0, 1.0, duration, 0);
    beamInTween.setParameters(0, easingCirc, ofxTween::easeInOut, 0.0, 1.0, duration / 2, 0);
    beamOutTween.setParameters(0, easingCirc, ofxTween::easeInOut, 0.0, 1.0, duration / 2, duration / 2);
}

//--------------------------------------------------------------
void LedController::drawPatterns(){
    
    float tweenValueFade = fadeTween.getTarget(0);
    float tweenValueBeamIn = beamInTween.getTarget(0);
    float tweenValueBeamOut = beamOutTween.getTarget(0);
    
    ofLog() << "Fade: " << tweenValueFade << ", BeamIn: " << tweenValueBeamIn << ", BeamOut: " << tweenValueBeamOut;

    ofVec2f ledPosA(layoutStart.x - layoutStep / 2, layoutStart.y);
    ofVec2f ledSizeA(layoutStep * ledSize / 2, numberofleds * ledStep);

    ofVec2f ledPosB(ledPosA.x, ledPosA.y + numberofleds * ledStep + 50);
    ofVec2f ledSizeB(ledSizeA.x, ledSizeA.y);
    
    // Fade
    if(patternMode == 0){
        ofSetColor(currentColor * (1 - tweenValueFade) + nextColor * tweenValueFade);
        ofDrawRectangle(ledPosA.x, ledPosA.y,
                        ledSizeA.x, ledSizeA.y);
        ofDrawRectangle(ledPosB.x, ledPosB.y,
                        ledSizeB.x, ledSizeB.y);
    }
    // Beam
    else if(patternMode == 1){
        ofSetColor(nextColor);
        ofDrawRectangle(ledPosA.x, ledPosA.y + ledSizeA.y * tweenValueBeamOut,
                        ledSizeA.x, ledSizeA.y * tweenValueBeamIn - ledSizeA.y * tweenValueBeamOut);
        ofDrawRectangle(ledPosB.x, ledPosB.y + ledSizeB.y * tweenValueBeamOut,
                        ledSizeB.x, ledSizeB.y * tweenValueBeamIn - ledSizeB.y * tweenValueBeamOut);

    }
}

//--------------------------------------------------------------
void LedController::fadeTweenCallback(int &e){
    ofLog() << "LedController::fadeTween Callback " << e;

    if(e == 0){
        // BeamのMotion後にcurrentColorを黒に
        if(patternMode == 1){
            nextColor = ofColor(0, 0, 0);
        }
    }
}


}
