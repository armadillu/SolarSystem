#include "testApp.h"


void testApp::setup(){

	ofSetFrameRate(65);
	ofBackground(0);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(2);
	ofEnableAlphaBlending();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	earthRotation = 0;

	OFX_REMOTEUI_SERVER_SETUP();
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("LIGHTING");
	OFX_REMOTEUI_SERVER_SHARE_PARAM(k_attenuation, 0, 1.5);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(l_attenuation, 0, 0.02);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(q_attenuation, 0, 0.0002);
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(sunColor);
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(sunLightColor);
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(sunLightAmbientColor);


	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("CAMERA");
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fov, 10, 120);
	vector<string> l; l.push_back("SUN"); l.push_back("MERCURY"); l.push_back("VENUS");
	l.push_back("EARTH"); l.push_back("MOON"); l.push_back("MARS");
	l.push_back("JUPITER"); l.push_back("SATURN"); l.push_back("NONE");
	OFX_REMOTEUI_SERVER_SHARE_ENUM_PARAM(camTarget, 0, NUM_CAMERA_TARGETS,l );

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("FOG");
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fog);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fogStart, 0, 2000);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(fogEnd, 1000, 10000);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("TOGGLE DRAWS");
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawStars);
	OFX_REMOTEUI_SERVER_SHARE_COLOR_PARAM(starsColor);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawSatellites);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawSatelliteTrails);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(drawPlanetTrails);
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SHARE_PARAM(timeSample);


	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("SPEEDS");
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SHARE_PARAM(globalSpeed, 0, 1);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(earthRotationSpeed, -50.0, 50.0);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(earthOrbitSpeed, 0.0, 5.0);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(moonOrbitSpeed, -50.0, 50.0);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(starsSpeed, -1, 1);

	OFX_REMOTEUI_SERVER_SET_UPCOMING_PARAM_GROUP("BLUR OVERLAY");
	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurPasses, 0, 4);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOffset, 0.0, 10);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.blurOverlayGain, 0, 255);
	OFX_REMOTEUI_SERVER_SHARE_PARAM(gpuBlur.numBlurOverlays, 0, 7);

	OFX_REMOTEUI_SERVER_SET_NEW_COLOR();


	if(fog)glEnable(GL_FOG);
	glFogf(GL_FOG_MODE, GL_LINEAR);
	glHint (GL_FOG_HINT, GL_NICEST);
	float fogC[4] = {0,0,0, 1};
	glFogfv(GL_FOG_COLOR, fogC);

	moon.setup("moon.jpg", &earth, 150, 8);

	sun.setup("sun.jpg", NULL, 0, 250);
	mercury.setup("mercury.jpg", &sun,  350, 7);
	venus.setup("venus.jpg", &sun,  600, 11);
	earth.setup("earth.jpg", &sun,  1000, 40);

	mars.setup("mars.jpg", &sun,  2000, 40);
	jupiter.setup("jupiter.jpg", &sun,  4000, 30);
	saturn.setup("saturn.jpg", &sun,  8000, 120);

	earth.setTrailColor(ofColor(46,64,180, 128));
	moon.setTrailColor(ofColor(255, 32));
	mars.setTrailColor(ofColor(255,90,90, 32));
	mercury.setTrailColor(ofColor(255, 32));
	jupiter.setTrailColor(ofColor(255, 32));
	venus.setTrailColor(ofColor(255, 32));
	saturn.setTrailColor(ofColor(255, 32));

	planets.push_back(&sun);
	planets.push_back(&mercury);
	planets.push_back(&venus);
	planets.push_back(&earth);
	planets.push_back(&moon);
	planets.push_back(&mars);
	planets.push_back(&jupiter);
	planets.push_back(&saturn);

	stars.setup();

	cam.setDistance(2000);
	cam.setFarClip(100000000);
	cam.setVFlip(true);

	sunLight.setPointLight();
	sunLight.enable();

	for(int i = 0; i < NUM_SATELLITES; i++){
		Satellite * s = new Satellite();
		s->setup(&earth);
		satellites.push_back(s);
	}

	ofFbo::Settings s;
	s.width = ofGetWidth();
	s.height = ofGetHeight();
	s.internalformat = GL_RGBA;
	//	s.textureTarget = GL_TEXTURE_RECTANGLE_ARB;
	s.maxFilter = GL_LINEAR; GL_NEAREST;
	s.numSamples = 4;
	s.numColorbuffers = 3;
	s.useDepth = true;
	s.useStencil = false;

	gpuBlur.setup(s, true);
	gpuBlur.setBackgroundColor(ofColor(0,255));

	OFX_REMOTEUI_SERVER_LOAD_FROM_XML();
}


void testApp::update(){

	TIME_SAMPLE_START("update");
	float dt = globalSpeed * 1.0 / 60.0;

	cam.setFov(fov);
	if(drawStars) stars.update(dt);
	earth.longitudeSpeed = earthOrbitSpeed;
	moon.longitudeSpeed = moonOrbitSpeed;

	stars.setSpeed(starsSpeed);

	for(int i = 0; i < planets.size(); i++){
		planets[i]->update(dt);
	}

	earthRotation += dt * earthRotationSpeed;
	earth.rotate( earthRotation, ofVec3f(0,1,0));


	if(drawSatellites || drawSatelliteTrails){
		for(int i = 0; i < NUM_SATELLITES; i++){
			satellites[i]->update(dt);
		}
	}

	sunLight.setDiffuseColor(sunLightColor);
	sunLight.setAmbientColor(sunLightAmbientColor);
	ofSetGlobalAmbientColor(sunLightAmbientColor);
	sunLight.setSpecularColor(ofColor(0));
	sunLight.setAttenuation(k_attenuation, l_attenuation, q_attenuation);

	TIME_SAMPLE_SET_ENABLED(timeSample);

	glFogi(GL_FOG_START, fogStart );
	glFogi(GL_FOG_END, fogEnd );
	TIME_SAMPLE_STOP("update");
}


void testApp::draw(){

	//point camera at target
	int targetID = camTarget;
	if (targetID >= 0 && targetID < NUM_CAMERA_TARGETS){
		cam.lookAt(planets[targetID]->getGlobalPosition());
	}

	TIME_SAMPLE_START("draw");
	ofSetColor(255);
	gpuBlur.beginDrawScene();
	ofClear(0, 0, 0, 255);
	cam.begin();

		TIME_SAMPLE_START("drawStars");
		ofDisableDepthTest();
		glDisable(GL_FOG);
		ofEnableBlendMode(OF_BLENDMODE_ADD);
		if(drawStars){
			ofSetColor(starsColor);
			stars.draw();
		}
		ofEnableAlphaBlending();
		if(fog)glEnable(GL_FOG);
		ofEnableDepthTest();
		TIME_SAMPLE_STOP("drawStars");

		//no lighting
		ofSetColor(255);
		TIME_SAMPLE_START("drawTrails");
		if(drawPlanetTrails){
			for(int i = 0; i < planets.size(); i++){
				planets[i]->drawTrails();
			}
		}
		if(drawSatelliteTrails) {
			for(int i = 0; i < NUM_SATELLITES; i++){
				satellites[i]->drawTrails();
			}
		}
		TIME_SAMPLE_STOP("drawTrails");

		ofSetColor(sunColor);
		sun.draw();

		//lighting
		ofEnableLighting();
			TIME_SAMPLE_START("drawPlanets");
			sunLight.enable();
			for(int i = 1; i < planets.size(); i++){ //skip the sun [0]
				ofSetColor(255);
				planets[i]->draw();
			}
			TIME_SAMPLE_STOP("drawPlanets");

			TIME_SAMPLE_START("drawSatellites");
			if(drawSatellites){
				for(int i = 0; i < NUM_SATELLITES; i++){
					satellites[i]->draw();
				}
			}
			TIME_SAMPLE_STOP("drawSatellites");
		ofDisableLighting();
		//sunLight.draw();
	cam.end();
	gpuBlur.endDrawScene();

	TIME_SAMPLE_START("performBlur");
	gpuBlur.performBlur();
	TIME_SAMPLE_STOP("performBlur");

	//draw the "clean" scene
	TIME_SAMPLE_START("drawSceneFBO");
	ofEnableBlendMode(OF_BLENDMODE_DISABLED);
	gpuBlur.drawSceneFBO();
	TIME_SAMPLE_STOP("drawSceneFBO");

	//overlay the blur on top
	ofEnableBlendMode(OF_BLENDMODE_ADD);
	TIME_SAMPLE_START("drawBlurFBO");
	gpuBlur.drawBlurFbo();
	TIME_SAMPLE_STOP("drawBlurFBO");

	ofEnableBlendMode(OF_BLENDMODE_ALPHA);

	TIME_SAMPLE_STOP("draw");
	
	TIME_SAMPLE_DRAW_BOTTOM_RIGHT();

}


void testApp::keyPressed(int key){

}


void testApp::keyReleased(int key){

}


void testApp::mouseMoved(int x, int y){

}


void testApp::mouseDragged(int x, int y, int button){

}


void testApp::mousePressed(int x, int y, int button){

}


void testApp::mouseReleased(int x, int y, int button){

}


void testApp::windowResized(int w, int h){

}


void testApp::gotMessage(ofMessage msg){

}


void testApp::dragEvent(ofDragInfo dragInfo){ 

}