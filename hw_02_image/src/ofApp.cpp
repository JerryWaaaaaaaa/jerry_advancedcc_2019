#include "ofApp.h"
#include <Animal.hpp>

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    
    // load the audio file
    bgm.load("vivaldi.mp3");
    bgm.play();
    
    // load the background image
    bgImg = ofImage("grass.png");
    
    // load the image sequence for cat
    vector<ofImage> sources;
    Animal temp_animal;
    for (int i = 0; i < 20; i ++) {
        ofImage temp = ofImage("cat/" + ofToString(i) + ".png");
        sources.push_back(temp);
    }
    temp_animal.setup(ofVec2f(50, 0), sources);

    animals.push_back(temp_animal);
    
    // load the image sequence for horse 1
    vector<ofImage> sources_horse1;
    Animal temp_horse_1;
    for (int i = 0; i < 16; i ++) {
        ofImage temp = ofImage("horse1/" + ofToString(i) + ".png");
        sources_horse1.push_back(temp);
    }
    temp_horse_1.setup(ofVec2f(50, ofGetHeight() / 3), sources_horse1);
    
    animals.push_back(temp_horse_1);
    
    // load image sequence for horse 2
    vector<ofImage> sources_horse2;
    Animal temp_horse_2;
    for (int i = 0; i < 20; i ++) {
        ofImage temp = ofImage("horse2/" + ofToString(i) + ".png");
        sources_horse2.push_back(temp);
    }
    temp_horse_2.setup(ofVec2f(50, 3 * ofGetHeight() / 5), sources_horse2);
    
    animals.push_back(temp_horse_2);
}

//--------------------------------------------------------------
void ofApp::update(){
    for (int i = 0; i < animals.size(); i++)  animals[i].update();
}

//--------------------------------------------------------------
void ofApp::draw(){
    bgImg.draw(-animals[0].pos.x,0,ofGetWidth(),ofGetHeight());
    bgImg.draw(ofGetWidth() - animals[0].pos.x,0,ofGetWidth(),ofGetHeight());
    for (int i = 0; i < animals.size(); i++)  animals[i].draw();
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
