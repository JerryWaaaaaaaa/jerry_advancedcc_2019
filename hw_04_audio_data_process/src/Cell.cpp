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
        color.a = 120;
    }
}


//--------------------------------------------------------------
void Cell::draw(){
    ofSetColor(color);
    ofDrawRectangle(pos.x, pos.y, size.x, size.y);
}

void Cell::updateColor(ofColor _color){
    color = _color;
    h = color.getHue();
    s = color.getSaturation();
    b = color.getBrightness();
}

void Cell::generateFilter(){
    if(type == "timbre"){
        // functions for changing the waveform
        
    }
    
    else if(type == "pitch"){
        // functions for changing the pitch
        
    }
    
    else if(type == "loudness"){
        // functions for changing the loudness
        
    }
}



