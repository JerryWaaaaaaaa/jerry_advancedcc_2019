//
//  Cell.cpp
//  project_challenge_4
//
//  Created by Casey Pan on 3/31/19.
//

#include "Cell.hpp"

//--------------------------------------------------------------
Cell::Cell(){
    
}

//--------------------------------------------------------------
Cell::Cell(ofVec2f _pos, ofColor _color, string _type){
    color = _color;
    pos = _pos;
    type = _type;
    state = false;
}

//--------------------------------------------------------------
void Cell::update(){
    
    // adjust alpha value according to button state
    if( state ){
        color.a = 255;
    } else {
        color.a = 60;
    }
}


//--------------------------------------------------------------
void Cell::draw(){
    ofSetColor(color);
    ofDrawRectangle(pos.x, pos.y, size.x, size.y);
    ofSetColor(255,255,255);
    ofDrawBitmapString(type, pos.x + size.x/2, pos.y + size.y/2);
}

void Cell::updateColor(ofColor _color){
    color = _color;
    h = color.getHue();
    s = color.getSaturation();
    b = color.getBrightness();
}

float Cell::generateFilter(float phase, int waveType){
        switch (waveType) {
            case 1://tri
                return abs(sin(phase*TWO_PI));
                ofLogNotice("tri");
                break;
            case 2://square
                return sin(phase*TWO_PI)>0?1:-1;
                ofLogNotice("square");
            case 3://sine
                return fmod(phase,TWO_PI);
            default:
                break;
        }
}



