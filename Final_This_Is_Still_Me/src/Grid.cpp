//
//  Grid.cpp
//  Final_This_Is_Still_Me
//
//  Created by Jerry-Wanglaws on 4/23/19.
//

#include "Grid.hpp"
#include "ofApp.h"
#include "Letter.hpp"

Grid::Grid(ofVec2f _pos, string _mode){
    pos = _pos;
    mode = _mode;
    moduleWidth = ofApp::moduleWidth;
    moduleHeight = ofApp::moduleHeight;
}

Grid::Grid(ofVec2f _pos, string _mode, double _moduleWidth, double _moduleHeight){
    pos = _pos;
    mode = _mode;
    moduleWidth = _moduleWidth;
    moduleHeight = _moduleHeight;
}

void Grid::setup(){
    solid = false;
}

void Grid::update(double _moduleWidth, double _moduleHeight){
    moduleWidth = _moduleWidth;
    moduleHeight = _moduleHeight;
}

void Grid::draw(){
    if(mode == "poster"){
        ofNoFill();
    }
    else if(mode == "letter"){
        ofFill();
    }
    ofSetColor(255, 255, 255);
    ofDrawRectangle(pos.x, pos.y, moduleWidth, moduleHeight);
}
