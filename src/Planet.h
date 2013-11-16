//
//  Planet.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 12/11/13.
//
//

#ifndef emptyExample_Planet_h
#define emptyExample_Planet_h

#include "ofMain.h"
#define MAX_PLANET_TRAIL	500

class Planet : public ofNode{

public:

	void setup(string textureName, ofNode * parent_, float oribtDist_ = 1000, float radius = 600){

		parent = parent_;
		cout << textureName << " " << oribtDist_ << endl;
		oribtDist = oribtDist_;
		sphere.set(radius, 40);
		sphere.setParent(*this);
		ofLoadImage(tex, textureName);
		sphere.mapTexCoordsFromTexture(tex);
		longitude = ofRandom(-180, 180);
		latitude = ofRandom(0,0);
		longitudeSpeed = ofRandom(-30,30);
		latitudeSpeed = ofRandom(-0,0);
	}

	void update(float dt){

		if(parent){
			latitude += latitudeSpeed * dt;
			longitude += longitudeSpeed * dt;

			if(latitude >= 90 || latitude <= -90){
				latitudeSpeed *= -1;
				longitude += 180;
			}

			ofVec3f p = parent->getGlobalPosition();
			this->orbit( longitude, latitude, oribtDist, p );

			if(ofGetFrameNum() % 20 == 0){
				trails.push_back( this->getGlobalPosition() );
				if( trails.size() > MAX_PLANET_TRAIL ){
					trails.erase(trails.begin());
				}
			}
		}
	}


	void draw(){
		//ofPushStyle();
			//ofSetColor(255); //wont work with the programmable renderer
			tex.bind();
			//sphere.drawWireframe();
			//ofLine(0,-1.1 * sphere.getRadius(), 0, 1.1 * sphere.getRadius());
			sphere.draw();
			tex.unbind();
		//ofPopStyle();
		//ofDrawBitmapString( ofToString(getGlobalPosition()), getGlobalPosition() );
	}

	void drawTrails(){
		ofPushStyle();
		ofSetColor(trailColor);
		ofMesh m;
		m.addVertices(trails);
		m.setMode(OF_PRIMITIVE_LINE_STRIP);
		m.draw();
		m.setMode(OF_PRIMITIVE_POINTS);
		m.draw();
		ofPopStyle();
	}

	void setTrailColor(ofColor c){
		trailColor = c;
	}

	float getRadius(){return sphere.getRadius();}

//private:

	float				oribtDist;
	float				longitudeSpeed, latitudeSpeed;
	float				longitude, latitude;

	ofSpherePrimitive 	sphere;
	ofTexture			tex;
	ofNode*				parent;
	vector<ofVec3f>		trails;
	ofColor				trailColor;

};



#endif
