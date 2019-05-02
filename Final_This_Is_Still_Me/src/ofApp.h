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
    
        ofxSlider<float> brightness;
        ofxSlider<float> contrast;
    
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
    
        ofxFaceTracker tracker;
    
        // check if the senses are drawn
        int checkLeftEye = 0;
        int checkRightEye = 0;
        int checkRightEye2 = 0;
        int checkMouse = 0;
        int checkNose = 0;
    
        ofFbo pattern;
    
        ofShader formShader;
        ofShader fillShader;
    
        ofTrueTypeFont font;
        float fontOffset = 0;
    
        bool pdfRendering;
        bool oneShot;
    
        int shapeX;
        int shapeY;
        int xOffset;
        int yOffset;
    
        void loadSettings();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    
        void updateFBO();

        int drawFacePart(const ofxFaceTracker::Feature &feature, int checkSet, float scale);
        void drawDate();
        void drawShaders();
        void drawCenteredStringAsShapes(string output, float x, float y);
    
        int getIndex(int _row, int _col);
        void drawGrid();

    
};
