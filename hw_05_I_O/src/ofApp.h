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
    
        // one unit of gui width and height
        int gWidth = 200;;
        int gHeight = 20;
    
        // poster width and height;
        const int size1 = 1220;
        const int size2 = 920;
        int pWidth;
        int pHeight;
    
        //  load panel
        ofxPanel gui;
    
        // direction of the poster
        ofxToggle vertical;
        ofxToggle horizontal;
    
        // load image
        ofxButton loadImage;
    
        // input field
        ofxInputField<string> header;
        ofxInputField<string> text;
    
        // color picker
        ofxColorSlider color;
    
        // save button
        ofxButton save;
    
        // text
        string headerText;
        string bodyText;
    
		
};
