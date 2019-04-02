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
Cell::Cell(ofVec2f _pos, ofColor _color){
    color = _color;
//    h = color.getHue();
//    s = color.getSaturation();
//    b = color.getBrightness();
    pos = _pos;
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

vector<float> Cell::generateFilter(float phase){
    float sample;
    float frequency;
    float volumn;
    // deal with timbre
        //square
        if(h >= 255*2/3){
            //sample = sin(phase*TWO_PI)>0?1:-1;
            sample = sin(phase*TWO_PI);
        }
        //tri
        if(h > 255/3 && h < 255*2/3){
            //sample = abs(sin(phase*TWO_PI));
            sample = sin(phase*TWO_PI);
        }
        //sin
        if(h <= 255/3){
            sample = sin(phase*TWO_PI);
        }
    // deal with frequency
        frequency = ofMap(s, 0, 255, 60, 2500);
    // deal with loudness
        volumn = ofMap(b, 0, 255, 0.3, 1);
    return {sample, frequency, volumn};
}



