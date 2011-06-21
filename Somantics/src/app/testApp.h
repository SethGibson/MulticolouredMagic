#pragma once

#include "ofMain.h"
#ifdef TARGET_OF_IPHONE
#include "ofxiPhone.h"
#include "ofxiPhoneExtras.h"
#include "ofxOpenCv.h"
#else
#include "KinectOrCamera.h"


#include "ofxSimpleGuiToo.h"
#include "TuioKinect.h"
#include "ofxTuioClient.h"

#endif
#include "constants.h"
#include "Reactickle.h"
#include "MainMenu.h"
#include "ReactickleApp.h"
#include "HoldButton.h"
#include "AboutPage.h"
#include "SettingsPage.h"

class testApp : public ReactickleApp, public SimpleButtonListener {
	
public:
	void setup();
	void update();
	void draw();
	void exit();
	
#ifndef TARGET_OF_IPHONE
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
#endif
	
	void touchDown(ofTouchEventArgs &touch);
	void touchMoved(ofTouchEventArgs &touch);
	void touchUp(ofTouchEventArgs &touch);
	void touchDoubleTap(ofTouchEventArgs &touch);
	void touchCancelled(ofTouchEventArgs &touch);
	
	void audioReceived( float * input, int bufferSize, int nChannels );
	
	void lostFocus();
	void gotFocus();
	void gotMemoryWarning();
	void deviceOrientationChanged(int newOrientation);

	// app stuff
	void launchReactickle(Reactickle *reactickle);
	void showSettings();
	void showAbout();
	

	void switchReactickle(Reactickle *reactickle);
	MainMenu mainMenu;
	AboutPage aboutPage;
	SettingsPage settingsPage;
	
	// this is the app that's currently receiving events
	Reactickle *currentApp;
	
	// back buttons for when on a reactickle
	HoldButton backButton;

	// button event (for back button)
	void buttonPressed(string name);
	
private:
	KinectOrCamera kinect;
	void startCrossFade(bool fadeIn);
	float crossFadeStartTime;
	// true if fading into an app, false if fading out to the menu.
	bool fadingIn;
	bool isReactickle(Reactickle *reactickle);
	int currOrientation;
	
	void setupGraphics();
	void setupOrientation();
	void updateOrientation();


};


