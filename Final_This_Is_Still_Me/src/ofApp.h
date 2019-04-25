#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOpenCv.h"
#include "ofxCv.h"
#include "FaceOsc.h"
#include "Grid.hpp"
#include "Letter.hpp"


class ofApp : public ofBaseApp, public FaceOsc{


	public:
    
        static double moduleWidth;
        static double moduleHeight;
    
        const int cols = 12;
        const int rows = 8;
        const int gutter = 10;
    
        static vector<shared_ptr<Grid>> grids; // array that holds the Grid objects
    
        string header = "this is still me";
        vector<shared_ptr<Letter>> letters; // test letter
    
        int textRow = 1; // first text row
        int textCol = 2; // first text col
    
        bool showGui;
        ofxPanel gui;
        ofxSlider<double> cp1;
        ofxSlider<double> cp2;
        ofxSlider<double> cp3;
        ofxSlider<double> cp4;
        ofxSlider<double> rp1;
        ofxSlider<double> rp2;
        ofxSlider<double> rp3;
        ofxSlider<double> rp4;
        ofxSlider<int> thresholdChange;

        ofVideoGrabber vidGrabber;
        ofBaseVideoDraws *videoSource;

        int camWidth, camHeight;
        bool bDrawMesh;
        glm::mat4 rotationMatrix;   
    
        ofxCvColorImage colorImg;
    
        ofxCvGrayscaleImage grayImage;
        ofxCvGrayscaleImage grayBg;
        ofxCvGrayscaleImage grayDiff;
    
        ofxCvContourFinder contourFinder;

    
        int threshold;
        bool bLearnBackground;
    
    
        ofxFaceTracker tracker;
    
        ofFbo pattern;
    
        void loadSettings();
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
    
        int getIndex(int _row, int _col);
        void drawVideo();
    
};
