//
//  Satellite.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 11/11/13.
//
//

#ifndef emptyExample_Satellite_h
#define emptyExample_Satellite_h

#include "ofMain.h"
#include "Planet.h"

#define MAX_SATELLITE_TRAIL	500

class Planet;

class Satellite : public ofNode{

public:

	void setup(Planet * parentPlanet_){
		plane1.set(30,20, 2, 2);
		plane2.set(30,20, 2, 2);

		plane1.setPosition(30, 0, 0);
		plane2.setPosition(-30, 0, 0);

		cylinder.set(10, 20, 10, 2);

		plane1.setParent(*this);
		plane2.setParent(*this);
		cylinder.setParent(*this);

		parentPlanet = parentPlanet_;

		longitude = ofRandom(-180, 180);
		latitude = ofRandom(89, -89);

		longitudeSpeed = 3 *  ofRandom(-30,30);
		latitudeSpeed = ofRandom(-10,10);

		myColor = ofColor( ofRandom(255), ofRandom(255), ofRandom(255), 128);
		this->setScale(0.2);

	}

	void update(float dt){

		latitude += latitudeSpeed * dt;
		longitude += longitudeSpeed * dt;

		if(latitude >= 90 || latitude <= -90){
			latitudeSpeed *= -1;
			longitude += 180;
		}

		this->orbit( longitude, latitude, parentPlanet->getRadius() * 1.2, *parentPlanet );
		this->lookAt(*parentPlanet, ofVec3f(0,1,0));

		if(ofGetFrameNum() % 8 == 0){
			trails.push_back( this->getGlobalPosition() );
			if( trails.size() > MAX_SATELLITE_TRAIL ){
				trails.erase(trails.begin());
			}
		}
	}


	void draw(){
		ofPushStyle();
			ofSetColor(myColor);
			plane1.drawWireframe();
			plane2.drawWireframe();
			cylinder.drawWireframe();
		ofPopStyle();
	}

	void drawTrails(){
		ofPushStyle();
		ofSetColor(myColor);
		ofMesh m;
		m.addVertices(trails);
		m.setMode(OF_PRIMITIVE_LINE_STRIP);
		m.draw();
		m.setMode(OF_PRIMITIVE_POINTS);
		m.draw();
		ofPopStyle();
	}



private:

	ofPlanePrimitive 	plane1;
	ofPlanePrimitive 	plane2;
	ofCylinderPrimitive cylinder;
	Planet *			parentPlanet;

	float				longitudeSpeed, latitudeSpeed;
	float				longitude, latitude;

	vector<ofVec3f>		trails;
	ofColor				myColor;

};

#endif
