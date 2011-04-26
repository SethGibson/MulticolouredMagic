#pragma once

#include "ofMain.h"
#include "TTApp.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"
#include "TuioKinect.h"
#include "ofxTuioClient.h"

class MagicApp : public ofBaseApp {
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void keyPressed(int key);
	void keyReleased(int key);
	
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	
	void windowResized(int w, int h);
	
	void audioReceived 	(float * input, int bufferSize, int nChannels); 
	void initApps();
	
	void setupGui();
	
	
	ofxTuioClient tuioClient;
	
	
	TuioKinect kinect;
	
	float volume;
	float volumeThreshold;
	float movement;
	vector<TTApp*> apps;
	int appIndex;
	int lastAppIndex;
	
	
	

	ofxSimpleGuiToo gui;
	float gain;	
	bool mustTakeScreenshot;
	ofImage screenshot;
};

