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
    
        const int cols = 8;
        const int rows = 12;
        const int gutter = 6;
    
        static vector<shared_ptr<Grid>> grids; // array that holds the Grid objects
    
        string header = "this is still me";
        vector<shared_ptr<Letter>> letters; // test letter
    
        int textRow = 2; // first text row
        int textCol = 1; // first text col
    
        float r = 1.0f; // for shader control
        float b = 0.0f;
        float rStep = 0.001;
        float bStep = 0.001;
    
        bool showGui;
        ofxPanel gui;
        /*
        ofxSlider<double> cp1;
        ofxSlider<double> cp2;
        ofxSlider<double> cp3;
        ofxSlider<double> cp4;
        ofxSlider<double> rp1;
        ofxSlider<double> rp2;
        ofxSlider<double> rp3;
        ofxSlider<double> rp4;
        */
        ofxSlider<int> thresholdChange;
    
        float cp1, cp2, cp3, cp4;
        float rp1, rp2, rp3, rp4;

        ofVideoGrabber vidGrabber;
        ofBaseVideoDraws *videoSource;

        int camWidth, camHeight;
        bool bDrawMesh;
        glm::mat4 rotationMatrix;   
    
        ofxCvColorImage colorImg;
    
        ofxCvGrayscaleImage grayImage;
        ofxCvGrayscaleImage grayBg;
        ofImage grayScale;
        ofxCvGrayscaleImage grayDiff;
        ofxCvContourFinder contourFinder;

        int threshold;
        bool bLearnBackground;
    
        ofxFaceTracker tracker;
    
        // check if the senses are drawn
        int checkLeftEye = 0;
        int checkRightEye = 0;
        int checkMouse = 0;
        int checkNose = 0;
        int checkJaw = 0;
    
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
        void updateFBO();
        void drawGrid();
        int drawFacePart(const ofxFaceTracker::Feature &feature, int checkSet, float scale);
    
        ofShader wireFrameShader;
        ofShader fillShader;
    
    
    
};
