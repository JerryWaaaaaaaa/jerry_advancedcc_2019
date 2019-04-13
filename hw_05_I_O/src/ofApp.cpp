#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    // set up event listener for the buttons
    // TBD
    
    // set up GUI
    gui.setup();
    gui.add(vertical.setup("vertical", true, gWidth, gHeight));
    gui.add(horizontal.setup("horizontal", false, gWidth, gHeight));
    gui.add(loadImage.setup("load image", gWidth, gHeight));
    gui.add(header.setup("header text", "Poster Generator", "a", "z", gWidth, gHeight * 3));
    gui.add(text.setup("text", "Poster Generator is fantastic!", "a", "z", gWidth, gHeight * 6));
    gui.add(color.setup("tint color", (40,60,80), (0,0,0), (255,255,255)));
    gui.add(save.setup("save as PDF"));
    
    // setup poster size
    pWidth = size2;
    pHeight = size1;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
