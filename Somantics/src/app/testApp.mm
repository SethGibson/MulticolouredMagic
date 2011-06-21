#include "testApp.h"
#include "MagicShapes.h"
#define	CROSS_FADE_TIME 2
//--------------------------------------------------------------
void testApp::setup(){	
	gain = 1;
	crossFadeStartTime = -100;
	volumeThreshold = 0.3;
	ReactickleApp::instance = this;
	setupGraphics();
	setupOrientation();
#ifdef TARGET_OF_IPHONE
	// register touch events
	ofRegisterTouchEvents(this);
	
	// initialize the accelerometer
	ofxAccelerometer.setup();
	
	//iPhoneAlerts will be sent to this.
	ofxiPhoneAlerts.addListener(this);
#endif
	
	kinect.setup();
	currentApp = &mainMenu;
	mainMenu.setup();
	
	backButton.setup("back", ofVec2f(0,0), IMAGE_ROOT + "backButton.png", IMAGE_ROOT + "backButtonDown.png");
	backButton.x = WIDTH - backButton.width;
	backButton.y = HEIGHT - backButton.height;
	backButton.setListener(this);
	backButton.setHoldMode(false);
	ofSoundStreamSetup(0, 1, this, 44100, 1024, 1);
	aboutPage.setup();
	settingsPage.setup();
}



void testApp::setupOrientation() {
#ifdef TARGET_OF_IPHONE
	[[UIDevice currentDevice] beginGeneratingDeviceOrientationNotifications];
	
	int orientation = [[UIDevice currentDevice] orientation];
	
	
	if(orientation==UIDeviceOrientationLandscapeLeft) {
		iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
	} else if(orientation==UIDeviceOrientationLandscapeRight) {
		iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);
	} else { // default
		iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);
	}
	
	currOrientation = UIDeviceOrientationPortrait;	
#endif
}


void testApp::updateOrientation() {
#ifdef TARGET_OF_IPHONE
	int orientation = [[UIDevice currentDevice] orientation];
	int orient = iPhoneGetOrientation();
	//printf("Phone orientation: %d, window orientation %d\n", orientation, orient);
	
	if(orientation!=currOrientation || orient==OFXIPHONE_ORIENTATION_PORTRAIT || orient==OFXIPHONE_ORIENTATION_UPSIDEDOWN) {
		if(orientation==UIDeviceOrientationLandscapeLeft) {
			iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_LEFT);
		} else if(orientation==UIDeviceOrientationLandscapeRight) {
			iPhoneSetOrientation(OFXIPHONE_ORIENTATION_LANDSCAPE_RIGHT);
		}
	}
	
	currOrientation = orientation;
#endif
}

void testApp::setupGraphics() {
	ofEnableNormalizedTexCoords();
	ofBackground(0, 0, 0);
	ofSetFrameRate(30.f);
	ofEnableAlphaBlending();
	ofSetVerticalSync(true);
}
//--------------------------------------------------------------
void testApp::update(){
	updateOrientation();
	if(currentApp!=NULL) {
		currentApp->volume = volume;
		currentApp->volumeThreshold = volumeThreshold;
		if(currentApp->needsKinect()) {
			kinect.update();
			//currentApp->colorPixels = kinect.getPixels();
			//currentApp->grayPixels = kinect.getDepthPixels();
		}
		currentApp->update();
	}
	
}

//--------------------------------------------------------------
void testApp::draw(){
	
	// if we're running on the retina display,
	// the openGL coordinates aren't pixel coordinates
	// (it's still 480x320 on a 960x640 iphone4 display)
	// so this scales the modelview so we can always use
	// pixel coordinates.
	if(RETINA) { 
		glPushMatrix();
		glScalef(0.5, 0.5, 1);
	}
	

	
	
	float crossFadeTime = ofGetElapsedTimef() - crossFadeStartTime;
	
	// if we're crossfading do this:
	if(crossFadeTime<CROSS_FADE_TIME) {
		
		Reactickle *first = &mainMenu;
		Reactickle *second = currentApp;
		
		// choose which way we're fading
		if(!fadingIn) {
			first = currentApp;
			second = &mainMenu;
		}
		// we want do draw the main menu fading out
		if(crossFadeTime<CROSS_FADE_TIME/2) {
			// fade out menu
			first->draw();
			ofSetColor(0, 0, 0, ofMap(crossFadeTime, 0, CROSS_FADE_TIME/2, 0, 255, true));
			ofRect(0, 0, WIDTH, HEIGHT);
		} else {
			// fade in app
			second->draw();
			ofSetColor(0, 0, 0, ofMap(crossFadeTime, CROSS_FADE_TIME/2, CROSS_FADE_TIME, 255, 0, true));
			ofRect(0, 0, WIDTH, HEIGHT);
		}
		
		
		// and then the app fading in
		
		
		// otherwise, just do this
	} else {
		currentApp->draw();
	}
	if(currentApp!=&mainMenu) {
		backButton.draw();
	}
	// pops the pixel coordinates scaling stuff.
	if(RETINA) {
		glPopMatrix();
	}
	
}

bool testApp::isReactickle(Reactickle *reactickle) {
	return currentApp!=&mainMenu && currentApp!=&aboutPage && currentApp!=&settingsPage;
}

void testApp::switchReactickle(Reactickle *reactickle) {
	if(currentApp!=NULL) {
		currentApp->stop();
	}
	// take care of previous reactickle - i.e. delete it if it's an actual reactickle
	if(isReactickle(currentApp)) {
		delete currentApp;
		currentApp = NULL;
	}
	

	// start the new one
	currentApp = reactickle;
	if(isReactickle(currentApp)) {
		currentApp->setup();
		backButton.setHoldMode(true);
		startCrossFade(true);
	} else {
		backButton.setHoldMode(false);
	}
	
	currentApp->start();

}
void testApp::startCrossFade(bool fadeIn) {
	fadingIn = fadeIn;
	crossFadeStartTime = ofGetElapsedTimef();
}

void testApp::buttonPressed(string name) {
	if(name=="back") {
		switchReactickle(&mainMenu);
	}
}


void testApp::launchReactickle(Reactickle *reactickle) {
	switchReactickle(reactickle);
}


void testApp::showAbout() {
	switchReactickle(&aboutPage);
}

void testApp::showSettings() {
	switchReactickle(&settingsPage);
}



//--------------------------------------------------------------
void testApp::exit(){
#ifdef TARGET_OF_IPHONE
	[[UIDevice currentDevice] endGeneratingDeviceOrientationNotifications];
#endif
}

//--------------------------------------------------------------
void testApp::touchDown(ofTouchEventArgs &touch){
	if(currentApp!=&mainMenu) {
		if(backButton.touchDown(touch.x, touch.y, touch.id)) {
			return;
		}
	}
	currentApp->touchDown(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void testApp::touchMoved(ofTouchEventArgs &touch){
	if(currentApp!=&mainMenu) {
		if(backButton.touchMoved(touch.x, touch.y, touch.id)) {
			return;
		}
	}
	currentApp->touchMoved(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void testApp::touchUp(ofTouchEventArgs &touch){
	if(currentApp!=&mainMenu) {
		if(backButton.touchUp(touch.x, touch.y, touch.id)) {
			return;
		}
	}
	currentApp->touchUp(touch.x, touch.y, touch.id);
}

//--------------------------------------------------------------
void testApp::touchDoubleTap(ofTouchEventArgs &touch){

}

//--------------------------------------------------------------
void testApp::lostFocus(){

}

//--------------------------------------------------------------
void testApp::gotFocus(){

}

//--------------------------------------------------------------
void testApp::gotMemoryWarning(){

}

//--------------------------------------------------------------
void testApp::deviceOrientationChanged(int newOrientation){

}


//--------------------------------------------------------------
void testApp::touchCancelled(ofTouchEventArgs& args){

}


void testApp::audioReceived( float * input, int bufferSize, int nChannels ) {
	// samples are "interleaved"
	float max = 0;
	
	for (int i = 0; i < bufferSize; i++){
		float val = gain*ABS(input[i]);
		if(val>max) max = val;
	}
	
	if(max>volume) volume = max;
	else volume *= 0.96;
	
	//volume *= gain;
	currentApp->audioReceived(input, bufferSize, nChannels);
}
#ifndef TARGET_OF_IPHONE
void testApp::mouseDragged(int x, int y, int button) {
	ofTouchEventArgs touch;
	touch.x = x;
	touch.y = y;
	touch.id = button;
	touchMoved(touch);
}
void testApp::mousePressed(int x, int y, int button) {
	ofTouchEventArgs touch;
	touch.x = x;
	touch.y = y;
	touch.id = button;
	touchDown(touch);
}
void testApp::mouseReleased(int x, int y, int button) {
	ofTouchEventArgs touch;
	touch.x = x;
	touch.y = y;
	touch.id = button;
	touchUp(touch);
}
#endif