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

/**
 * Orbit.cpp
 * magic
 *
 * Created by Marek Bereza on 12/04/2011.
 *
 */

#include "Orbit.h"

#include "Settings.h"
#ifdef _WIN32
    #include "gui/ColorPicker.h"
#else
    #include "ColorPicker.h"
#endif
#include "constants.h"



void Orbit::setup(){
	ofSetCircleResolution(42); //42 is the answer to everything!

	numberOfShapes = 0;

	numberOfMiddleShapes = 0;
	numberOfInnerShapes = 0;

	timeOfLastInteraction = ofGetElapsedTimef();

	ofColor shapesColour = ofColor::red;
	ofColor bigShapeColour = ofColor::red;

	shapesRadius = 40.f;
	bigShapeRadius = 400.f;
	maxNumberOfShapes = 20;
	maxNumberOfMiddleShapes = 8;
	maxNumberOfInnerShapes = 4;

	centreOfBigShape = ofVec2f(WIDTH/2.f, HEIGHT/2.f);
	startOfShapes.x = centreOfBigShape.x;
	startOfShapes.y = centreOfBigShape.y - (bigShapeRadius/2) - (shapesRadius/2);

	startOfMiddleShapes.x = centreOfBigShape.x;
	startOfMiddleShapes.y = startOfShapes.y * 1.33f;

	startOfInnerShapes.x = centreOfBigShape.x;
	startOfInnerShapes.y = startOfShapes.y * 1.66f;

	shapeOnScreen = false;

	radiansPerShape = (2*PI)/maxNumberOfShapes;

	radiansPerMiddleShape = (2*PI)/maxNumberOfMiddleShapes;

	radiansPerInnerShape = (2*PI)/maxNumberOfInnerShapes;

	currentShapeType = 0;
}

void Orbit::update(){
	float timeNow = ofGetElapsedTimef();

	float timeSinceLastInteraction = timeNow - timeOfLastInteraction;

	if((volume > volumeThreshold) && (timeSinceLastInteraction > 0.3f )){
		touchDown(WIDTH/2.f, HEIGHT/2.f, 0); //simulate a touch in the centre of the screen
	}
}

void Orbit::draw() {

	ofPushStyle(); //for safety in terms of drawing, and not messing up anyone elses code
	ofBackground(0); //set to black (0 brightness)
	ofFill();
	int colorIndex = Settings::getInstance()->settings["fgColor"];
	if(colorIndex==20) {
		ofSetHexColor(0xFFFFFF);
	} else {
		ofSetHexColor(ColorPicker::colors[colorIndex]);
	}


	switch (mode) {
		case 0:
			if (shapeOnScreen) {


				drawShape(currentShapeType, centreOfBigShape, bigShapeRadius); //using the new MagicShapes.h file
			}
			break;
		case 1:
			if(numberOfShapes > 0){

				if(colorIndex==20) {
					ofSetColor(shapesColour);
				} else {
					ofSetHexColor(ColorPicker::colors[colorIndex]);
				}


				ofVec2f currentShapePosition = startOfShapes;

				for(int i=0; i< numberOfShapes; i++){
					drawShape(currentShapeType, currentShapePosition, shapesRadius);

					currentShapePosition = currentShapePosition.getRotatedRad(radiansPerShape, centreOfBigShape);
				}

			}else{

				if(colorIndex==20) {
					ofSetColor(bigShapeColour);
				} else {
					ofSetHexColor(ColorPicker::colors[colorIndex]);
				}

				drawShape(currentShapeType, centreOfBigShape, bigShapeRadius);
			}
			break;
		case 2:
			if(numberOfShapes > 0){
				if(colorIndex==20) {
					ofSetColor(shapesColour);
				} else {
					ofSetHexColor(ColorPicker::colors[colorIndex]);
				}

				ofVec2f currentShapePosition = startOfShapes;

				for(int i=0; i< numberOfShapes; i++){
					drawShape(currentShapeType, currentShapePosition, shapesRadius);

					currentShapePosition = currentShapePosition.getRotatedRad(radiansPerShape, centreOfBigShape);
				}

				if(numberOfMiddleShapes > 0){

					currentShapePosition = startOfMiddleShapes;

					for(int i=0; i< numberOfMiddleShapes; i++){
						drawShape(currentShapeType, currentShapePosition, shapesRadius);

						currentShapePosition = currentShapePosition.getRotatedRad(radiansPerMiddleShape, centreOfBigShape);
					}
				}

				if(numberOfInnerShapes > 0){
					currentShapePosition = startOfInnerShapes;

					for(int i=0; i< numberOfInnerShapes; i++){
						drawShape(currentShapeType, currentShapePosition, shapesRadius);

						currentShapePosition = currentShapePosition.getRotatedRad(radiansPerInnerShape, centreOfBigShape);
					}
				}
			}else{

				if(colorIndex==20) {
					ofSetColor(bigShapeColour);
				} else {
					ofSetHexColor(ColorPicker::colors[colorIndex]);
				}
				drawShape(currentShapeType, centreOfBigShape, bigShapeRadius);
			}

			break; //nothing for the most complicated version yet...
		default:
			break;
	}

	ofPopStyle(); //pop back what was there before
}

bool Orbit::touchDown(float x, float y, int touchId){
	switch (mode) {
		case 0:
			shapeOnScreen = !shapeOnScreen;
			if(!shapeOnScreen){
				nextShape();
			}

			timeOfLastInteraction = ofGetElapsedTimef();
			break;
		case 1:
			numberOfShapes++;

			if(numberOfShapes > maxNumberOfShapes){
				numberOfShapes = 0;
				bigShapeColour = ofColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
				shapesColour = bigShapeColour;
				nextShape();
			}

			timeOfLastInteraction = ofGetElapsedTimef();
			break;
		case 2:
			if (numberOfShapes < maxNumberOfShapes) {
				numberOfShapes++;
			}else if (numberOfMiddleShapes < maxNumberOfMiddleShapes){
				numberOfMiddleShapes++;
			}else if (numberOfInnerShapes <= maxNumberOfInnerShapes){
				numberOfInnerShapes++;
			}

			if((numberOfShapes >= maxNumberOfShapes) && (numberOfMiddleShapes >= maxNumberOfMiddleShapes) && (numberOfInnerShapes > maxNumberOfInnerShapes)){
				numberOfShapes = 0;
				numberOfMiddleShapes = 0;
				numberOfInnerShapes = 0;

				bigShapeColour = ofColor(ofRandom(0,255), ofRandom(0,255), ofRandom(0,255));
				shapesColour = bigShapeColour;
				nextShape();
			}

			timeOfLastInteraction = ofGetElapsedTimef();

			break;
		default:
			break;
	}

	return true;
}

void Orbit::nextShape(){
	currentShapeType++;

	if(currentShapeType >=NUM_MAGIC_SHAPES || currentShapeType < 0) { //safety!
		currentShapeType = 0;
	}
//	printf("currentShapeType: %d\n", currentShapeType);

#ifndef TARGET_OF_IPHONE
	ofxOscMessage m;
	m.setAddress( "/shapechange" );
	m.addIntArg( currentShapeType );
	ReactickleApp::instance->sender.sendMessage( m );
#endif
}
