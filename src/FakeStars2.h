//
//  Header.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 12/11/13.
//
//

#pragma once
#include "ofMain.h"
#define NUM_MESHES	1
#define NUM_STARS 10000

class FakeStars2 {

public:
	void setup(){

		speedMult = 1;
		float r = 9000;

		for(int i = 0 ; i < NUM_MESHES; i++){
			ofVboMesh * m = new ofVboMesh();
			m->setMode(OF_PRIMITIVE_POINTS);
			for(int j = 0; j < NUM_STARS; j++){
				ofVec3f p = ofVec3f( ofRandom(-r,r), ofRandom(-r,r), ofRandom(-r,r) );
				p = p * powf(ofRandom(1), 1.1);
				m->addVertex( p );
				rotSpeeds.push_back( 0.3 * ofRandom(-1,1));
				ofVec3f d = ofVec3f(ofRandom(1), ofRandom(1), ofRandom(1));
				d.normalize();
				rotDirs.push_back(d);
			}
			meshes.push_back(m);
		}
	}

	void update(float dt){
	}

	void setSpeed(float s){
		speedMult = s;
	}
	void setPointSize(float s){
		pointSize = s;
	}


	void draw(){
		ofPushStyle();
		glPointSize(pointSize);
		for(int i = 0 ; i < NUM_MESHES; i++){
			meshes[i]->draw();
		}
		ofPopStyle();
	}

private:

	vector<float> rotSpeeds;
	vector<ofVec3f> rotDirs;
	float speedMult;
	float pointSize;
	vector<ofVboMesh*> meshes;

};


