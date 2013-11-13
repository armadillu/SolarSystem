#pragma once

#include "ofMain.h"
#include "Satellite.h"
#include "Planet.h"
#include "ofxRemoteUIServer.h"
#include "FakeStars.h"
#include "ofxFboBlur.h"
#include "ofxTimeMeasurements.h"

#define NUM_SATELLITES	10


class testApp : public ofBaseApp{

public:

	enum CameraTarget {SUN = 0, MERCURY, VENUS, EARTH, MOON, MARS, JUPITER, SATURN,  NUM_CAMERA_TARGETS};

	void setup();
	void update();
	void draw();
	
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofEasyCam			cam;
	ofPlanePrimitive 	plane1;
	ofPlanePrimitive 	plane2;
	ofCylinderPrimitive cylinder;


	vector<Satellite*>	satellites;
	vector<Planet*>		planets;


	Planet				sun;
	Planet				mercury;
	Planet				venus;
	Planet				earth;
	Planet				moon;
	Planet				mars;
	Planet				jupiter;
	Planet				saturn;


	ofLight				sunLight;
	ofColor				sunLightColor;
	ofColor				sunLightAmbientColor;
	ofColor				sunColor;
	FakeStars			stars;
	ofColor				starsColor;


	float				k_attenuation;
	float				l_attenuation;
	float				q_attenuation;

	float				earthRotationSpeed;
	float				earthRotation;
	float				earthOrbitSpeed;
	float				moonOrbitSpeed;
	float				starsSpeed;
	bool				fog;
	bool				drawStars;
	bool				drawSatellites;
	bool				drawSatelliteTrails;
	bool				drawPlanetTrails;

	bool				timeSample;
	
	float				fogEnd;
	float				fogStart;

	float				fov;

	float				globalSpeed;

	ofxFboBlur			gpuBlur;

	CameraTarget		camTarget;

};
