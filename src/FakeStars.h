//
//  Header.h
//  emptyExample
//
//  Created by Oriol Ferrer Mesià on 12/11/13.
//
//

#ifndef emptyExample_Header_h
#define emptyExample_Header_h

#define NUM_LAYERS	5

class FakeStars {

public:
	void setup(){

		ofLoadImage(tex, "stars.jpg");

		for(int i = 0 ; i < NUM_LAYERS; i++){
			ofSpherePrimitive *s = new ofSpherePrimitive();
			s->set( 1000 + powf(i, 2) * 1000 , 20);
			s->mapTexCoordsFromTexture(tex);
			s->rotate(ofRandom(0,360), ofVec3f(ofRandom(0,1),ofRandom(0,1),ofRandom(0,1)));
			spheres.push_back(s);
			rotSpeeds.push_back(0.01 * ofRandom(-1,1));
			ofVec3f d = ofVec3f(ofRandom(1), ofRandom(1), ofRandom(1));
			d.normalize();
			rotDirs.push_back(d);
		}
	}

	void update(float dt){
		for(int i = 0 ; i < NUM_LAYERS; i++){
			spheres[i]->rotate( dt * ofGetElapsedTimef() * rotSpeeds[i], rotDirs[i]);
		}
	}

	void draw(){
		tex.bind();
		for(int i = 0 ; i < NUM_LAYERS; i++){
			spheres[i]->draw();
		}
		tex.unbind();
	}

private:

	int numLayers;
	ofTexture tex;
	vector<ofSpherePrimitive*> 	spheres;
	vector<float> rotSpeeds;
	vector<ofVec3f> rotDirs;

};


#endif
