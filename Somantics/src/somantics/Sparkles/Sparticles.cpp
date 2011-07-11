
#include "Sparticles.h"

Sparticles::Sparticles(){
	
	pos = 0;
    maxAge = 50;
    numParticles = 1000;
    positions = new ofVec2f[numParticles];
    velocities = new ofVec2f[numParticles];
    ages = new int[numParticles];
    
    for(int i = 0; i < numParticles; i++) {
        positions[i] = ofVec2f(-10, -10);
        velocities[i] = ofVec2f(0, 0);
        ages[i] = maxAge;
    }
	
	
}

void Sparticles::setup() {
	
	vector<string> imgs;
	imgs.push_back("img/particles/blob.png");
	imgs.push_back("img/particles/glitter.png");
	imgs.push_back("img/particles/sparkle.png");
	imgs.push_back("img/particles/star.png");
	for(int i = 0; i < imgs.size(); i++) {
		images.push_back(ofImage());
		images.back().loadImage(imgs[i]);
		images.back().setAnchorPercent(0.5, 0.5);
	}
}


Sparticles::~Sparticles(){
    delete [] positions;
    delete [] velocities;
    delete [] ages;
}


void Sparticles::update(){
    for(int i = 0; i < numParticles; i++) {
        ages[i]++;
        positions[i] += velocities[i];
    }
}

void Sparticles::draw(){
    
    
	
    for(int i = 0; i < numParticles; i++) {
        if(ages[i]<maxAge) {
            float size = ofMap(ages[i], 0, maxAge, 10, 0);
            
			//            if (particleDisplayMode == 2){
			//            ofTranslate(1, 1, -size);
			//            }
			//            else {
			//             ofTranslate(1, 1, 1);
			//            }
            //ofTranslate(1, 1, ages[i]/10); ///moves them in 3D
            
            ofSetColor(0, 150,255, ofMap(ages[i], 0, maxAge, 255, 0));
            //ofCircle(positions[i].x - size/2, positions[i].y - size/2, size);
			images[0].draw(positions[i].x, positions[i].y, size*4, size*4);
//            ofRect(positions[i].x - size/2, positions[i].y - size/2, size, size);
        }
    }
	
}

void Sparticles::spawn(float x, float y, float dx, float dy){
    ages[pos] = 0;
    positions[pos] = ofVec2f(x, y);
    velocities[pos] = ofVec2f(dx, dy);
    
    pos++;
    pos %= numParticles;
}
