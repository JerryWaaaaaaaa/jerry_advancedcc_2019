#pragma once

#include "Cell.hpp"
#include "ofMain.h"
#include "ofxUI.h"
#include "ofxXmlSettings.h"


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
        void exit();

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
    
        void drawGrid(float x, float y);
        void guiEvent(ofxUIEventArgs &e);
        void loadImgFile();
        void loadAudFile();
        void setGUI1();
        void setGUI2();
    
        // for audio output
        void audioOut(ofSoundBuffer &outBuffer);
 
        // for soundStream
        int bufferSize;
        int sampleRate;

    
        // for UI

        ofxUISuperCanvas *gui1;
        ofxUISuperCanvas *gui2;
    
        ofxUIMovingGraph *mg;
        ofxUIDropDownList *ddl;
        ofxUIToggleMatrix *tm;
    
        ofxUITextInput *textInput;
    
        bool hideGUI;
        
        float red, green, blue;
        bool bdrawGrid;
        bool bdrawPadding;
    
        float *buffer;
        shared_ptr<ofImage> img;
    
        ofImage *imgDisplay;
    
        ofSoundPlayer aud;
    
        float vol;
    
        float lowMin = 60.0f;
        float midMin = 800.0f;
        float highMin = 1600.0f;
        float highMax = 2500.0f;
    
    
        // initial color
        ofColor defaultColor = ofColor(20,20,20,255);
        // grid array
        vector<Cell> cells;
    
        float phase = 0;
    
       
    
};
