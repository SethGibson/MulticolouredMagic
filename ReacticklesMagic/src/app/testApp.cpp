/**
 * == Simplified BSD *** MODIFIED FOR NON-COMMERCIAL USE ONLY!!! *** ==
 * Copyright (c) 2011, Cariad Interactive LTD
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, are permitted 
 * provided that the following conditions are met:
 * 
 *   * Redistributions of source code must retain the above copyright notice, this list of 
 *     conditions and the following disclaimer.
 * 
 *   * Redistributions in binary form must reproduce the above copyright notice, this list of 
 *     conditions and the following disclaimer in the documentation and/or other materials provided 
 *     with the distribution.
 * 
 *   * Any redistribution, use, or modification is done solely for personal benefit and not for any 
 *     commercial purpose or for monetary gain
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
 * WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "testApp.h"
#include "MagicShapes.h"
#define BUTTON_PADDING 5
#ifndef TARGET_OF_IPHONE
#include "util.h"

#endif
//--------------------------------------------------------------
void testApp::setup(){	

	setupApp(this, "ReacticklesMagic");
	
	modeDisplay.setup();


#ifndef TARGET_OF_IPHONE
	setupGui();
#endif
	
	mainMenu = new MainMenu();
	
	currentApp = mainMenu;
	mainMenu->setup();
	
	backButton.setup("back", ofVec2f(0,0), IMAGE_ROOT + "backButton.png", IMAGE_ROOT + "backButtonDown.png");
	backButton.x = BUTTON_PADDING;
	backButton.y = HEIGHT - backButton.height - BUTTON_PADDING;
	backButton.setListener(this);
	backButton.setHoldMode(false);
	
	
	modeUpButton.setup("modeUp", ofVec2f(0,0), IMAGE_ROOT + "modeUpButton.png", IMAGE_ROOT + "modeUpButtonDown.png");
	modeDownButton.setup("modeDown", ofVec2f(0,0), IMAGE_ROOT + "modeDownButton.png", IMAGE_ROOT + "modeDownButtonDown.png");
	
	modeUpButton.x = WIDTH - modeUpButton.width - BUTTON_PADDING;
	modeUpButton.y = BUTTON_PADDING;
	
	modeDownButton.x = WIDTH - modeDownButton.width - BUTTON_PADDING;
	modeDownButton.y = HEIGHT - modeDownButton.height - BUTTON_PADDING;
	
	modeUpButton.setListener(this);
	modeDownButton.setListener(this);
	
	modeUpButton.setHoldMode(true);
	modeDownButton.setHoldMode(true);
	
	aboutPage.setup();
	settingsPage.setup();
	
#ifndef TARGET_OF_IPHONE
	// set up kinect
	kinect.setup();
	
	// set up tuio
	tuioClient.connect(3333);
	ofAddListener(ofEvents.touchDown, this, &testApp::tuioTouchDown);
	ofAddListener(ofEvents.touchUp, this, &testApp::tuioTouchUp);
	ofAddListener(ofEvents.touchMoved, this, &testApp::tuioTouchMoved);
	
#endif	
 
}

#ifndef TARGET_OF_IPHONE




void testApp::setupGui() {
	
	gui.addSlider("Threshold", kinect.threshold, 0, 255);
	gui.addSlider("Far clipping", kinect.farClip, 0, 255);
	gui.addSlider("blend amount", kinect.blendAmount, 0.01, 0.2);
	kinect.blendAmount = 0.1;
	
	gui.addTitle("SOUND");
	gui.addSlider("Sound Meter", volume, 0, 1);
	gui.addSlider("Sound input volume", gain, 0, 3);
	//gui.addSlider("Volume Sensitivity", (float &)volumeThreshold, 0, 1);
	

	gui.addTitle("CAMERA");
	gui.addToggle("Camera Flip Horizontal", kinect.flipX);
	gui.addToggle("Camera Flip Vertical", kinect.flipY);
	gui.addContent("Camera image", kinect.colorImage);
	gui.addContent("Depth image", kinect.grayImage);
	//gui.addContent("Thresholded image", kinect.th);
	gui.addContent("Bg image", kinect.bgImage);
	gui.addContent("Blobs", kinect.contourFinder);
	
	gui.setDraw(false);
	

	vector<ofxSimpleGuiPage*> pages = gui.getPages();
	string prefsDir = getPreferencesDirectory(APP_NAME);
	for(int i = 0; i < pages.size(); i++) {
		pages[i]->setXMLName(prefsDir+"/"+pages[i]->name + "_settings.xml");
	}
	
	gui.loadFromXML();
	gui.setAutoSave(true);
}


#endif

//--------------------------------------------------------------
void testApp::update(){
	
	updateOrientation();
	if(currentApp!=NULL) {
		gain = Settings::getInstance()->settings["volume"];
#ifndef TARGET_OF_IPHONE	
		kinect.update(currentApp->needsKinect());
		//printf("Needds kinect %d\n", currentApp->needsKinect());
#endif
		currentApp->volume = volume*gain;
		currentApp->volumeThreshold = VOLUME_THRESHOLD;
		currentApp->update();
	}

}

//--------------------------------------------------------------
void testApp::draw(){
	
	
	// this is more to reset the blend mode
	ofEnableAlphaBlending();
	// if we're running on the retina display,
	// the openGL coordinates aren't pixel coordinates
	// (it's still 480x320 on a 960x640 iphone4 display)
	// so this scales the modelview so we can always use
	// pixel coordinates.
	if(RETINA) { 
		glPushMatrix();
		glScalef(0.5, 0.5, 1);
	}
	

	if(FAKE_GAME_MODE) centerer.begin();	
	
	drawCurrentReactickle();
	
	
	
	if(currentApp!=mainMenu) {
		backButton.draw();
		if(currentApp!=&aboutPage && currentApp!=&settingsPage) {
			modeUpButton.draw();
			modeDownButton.draw();
		}
	}
	if(isReactickle(currentApp)) {
		modeDisplay.draw();
	}
	
	
	// do the brightness control by just drawing a black rectangle over
	// the whole screen
	float alpha = ofMap(Settings::getInstance()->settings["brightness"], 0, 1, 0.6, 0);
	glColor4f(0, 0, 0, alpha);
	ofEnableAlphaBlending(); // reset blend func
	ofRect(0, 0, WIDTH, HEIGHT);
	
	
	if(FAKE_GAME_MODE) centerer.end();
	
	
	// pops the pixel coordinates scaling stuff.
	if(RETINA) {
		glPopMatrix();
	}
	
#ifndef TARGET_OF_IPHONE
	// do the screenshot
	if(mustTakeScreenshot) {
		screenshot.grabScreen(0, 0, WIDTH, HEIGHT);
		string filename = getDesktopPath() + "/Magic-" + dateTimeString()+ ".jpg";
		
		screenshot.saveImage(filename);
		mustTakeScreenshot = false;
	}
	gui.draw(); 
#endif

}

bool testApp::isReactickle(Reactickle *reactickle) {
	return reactickle!=mainMenu && reactickle!=&aboutPage && reactickle!=&settingsPage;
}
void testApp::buttonPressed(string name) {
	if(name=="back") {
		switchReactickle(mainMenu);
	} else if(name=="modeUp") {
		// increment mode
		if(isReactickle(currentApp)) {
			int newMode = currentApp->getMode();
			newMode++;
			if(newMode>=currentApp->getNumModes()) {
				newMode = 0;
			}
			currentApp->setMode(newMode);
#ifndef TARGET_OF_IPHONE
			ofxOscMessage m;
			m.setAddress("/modechange");
			m.addIntArg( newMode );
			ReactickleApp::instance->sender.sendMessage( m );
#endif
			modeDisplay.setMode(newMode);
		}
	} else if(name=="modeDown") {
		// decrement mode
		if(isReactickle(currentApp)) {
			int newMode = currentApp->getMode();
			newMode--;
			if(newMode<0) {
				newMode = currentApp->getNumModes()-1;
			}
			currentApp->setMode(newMode);
#ifndef TARGET_OF_IPHONE
			ofxOscMessage m;
			m.setAddress("/modechange");
			m.addIntArg( newMode );
			ReactickleApp::instance->sender.sendMessage( m );
#endif
			modeDisplay.setMode(newMode);
		}
	}
}


void testApp::launchReactickle(Reactickle *reactickle) {
	switchReactickle(reactickle);
	if(isReactickle(reactickle)) {
		reactickle->setMode(modeDisplay.getMode());
	}
}


void testApp::showAbout() {
	switchReactickle(&aboutPage);
}

void testApp::showSettings() {
	switchReactickle(&settingsPage);
}


