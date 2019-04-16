#pragma once

#include "ofMain.h"
#include "ofxGui.h"

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
        void generatePoster();
        void checkOrientation();
        void displayGrid();
        void displayImage();
        void uploadImage();
        void saveFrame();
        void changeColor1();
        void changeColor2();
        void changeColor3();
        void changeColor4();
        void changeColor5();
        void changeText1();
        void changeText2();
        void changeText3();
        void changeText4();
        int getIndex(int row, int col);
    
        int counter = 0;
    
        // one unit of gui width and height
        int gWidth = 200;;
        int gHeight = 20;
    
        ofFbo fbo;
    
        // poster width and height;
        const int size1 = 610;
        const int size2 = 460;
        float pWidth;
        float pHeight;
        int rows;
        int cols;
        float moduleWidth;
        float moduleHeight;
        ofVec2f origin;
        vector<ofVec2f> modules;
    
        //  load panel
        ofxPanel gui;
    
        // direction of the poster
        ofxButton vertical;
        ofxButton horizontal;
        bool verticalDir = true;
    
        // scale of the poster
        ofxFloatSlider scale;
    
        // load image
        ofxButton loadImage;
        bool showImg = true;
    
        // display image
        ofxButton showImage;
    
        // image position
        ofxIntSlider imageRow;
        ofxIntSlider imageCol;
        ofxIntSlider imageWidth;
        ofxIntSlider imageHeight;
    
        // input field
        ofxInputField<string> header;
        ofxInputField<string> text;
    
        // text position
        ofxIntSlider headerRow;
        ofxIntSlider headerCol;
        ofxIntSlider bodyRow;
        ofxIntSlider bodyCol;
    
        // color picker
        ofxColorSlider backgroundColor;
        ofxColorSlider fontColor;
    
        // show grid button
        ofxButton showGrid;
        bool show = true;
    
        // save button
        ofxButton saveAsPDF;
        bool savePDF = false;
    
        // image
        ofImage img;
        ofImage poster;
    
        // fonts
        ofTrueTypeFont HelveticaBold;
        ofTrueTypeFont HelveticaLight;
    
        //color combination button
        vector<ofxButton> combos;
        ofxButton colorCombo1;
        ofxButton colorCombo2;
        ofxButton colorCombo3;
        ofxButton colorCombo4;
        ofxButton colorCombo5;
    
    
        //color combination info
        //background array
        vector<ofColor> backgrounds;
    
        //font array
        vector<ofColor> fonts;
    
        //text buttons
        ofxButton textExample1;
        ofxButton textExample2;
        ofxButton textExample3;
        ofxButton textExample4;
};
    
