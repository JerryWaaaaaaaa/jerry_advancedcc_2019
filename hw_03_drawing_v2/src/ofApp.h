#pragma once

#include "ofMain.h"
#include "particle.hpp"
#include "ofxOpenCv.h"
#include <vector>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        int frameCounter = 0;
        int imgSaveNum = 0; // counter for saving screens
    
        int scl = 10; // the scale of each pixel
        int col; // colume
        int row; // row
        float inc = 0.05; // noise increment
    
        float zOff = 0;
    
        ofImage testImg;
    
        ofVideoGrabber mVideoGrabber; //declare a ofVideoGrabber object
        ofFbo mFbo;
        ofxCvGrayscaleImage grayImage;
        ofxCvColorImage colorImg;
        ofxCvHaarFinder eyeFinder;
    
        ofImage imgSave;
    
        ofxCvGrayscaleImage grayFrame1;
        ofxCvGrayscaleImage grayFrame2;
    
        string currentMode = "none";
    
        vector< shared_ptr<Particles> > particles;
        //vector<Particle> particles; // array for all particles
        vector <ofVec2f> flowField; // array for direction vectors
		
};
