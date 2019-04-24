//
//  Letter.cpp
//  Final_This_Is_Still_Me
//
//  Created by Jerry-Wanglaws on 4/23/19.
//

#include "Letter.hpp"
#include "ofApp.h"

vector<double> Letter::colProportion = {2,1,1,1}; // proportion of each column
vector<double> Letter::rowProportion = {1,1,1,1}; // proportion of each row

vector<vector<int>> Letter::letters {
 {4,7,10,11,13,14,15,16},
 {1,2,3,4,5,6,8,9,10,11,12,15,16},
 {2,3,5,8,9,12,13,16},
 {1,2,3,4,5,8,9,12,14,15},
 {1,2,3,4,5,6,7,8,9,10,11,12,13,16},
 {1,2,3,4,5,7,9,11,13,15},
 {1,2,3,4,5,8,9,12,13,15,16},
 {1,2,3,4,6,10,13,14,15,16},
 {1,4,5,8,9,10,11,12,13,16},
 {1,3,4,5,8,9,10,11,12,13},
 {1,2,3,4,6,7,9,12,13,16},
 {1,2,3,4,8,12,16},
 {1,2,3,4,5,6,7,9,10,11,13,14,15,16},
 {1,2,3,4,6,11,13,14,15,16},
 {2,3,5,8,9,12,14,15},
 {1,2,3,4,5,7,9,11,14},
 {1,2,3,4,5,8,9,11,12,13,14,15,16},
 {1,2,3,4,5,7,9,10,11,15,16},
 {4,5,6,8,9,11,12,13,15,16},
 {1,5,9,10,11,12,13},
 {1,2,3,4,8,12,13,14,15,16},
 {1,6,11,13,14,15,16},
 {1,2,3,4,6,7,8,10,11,12,13,14,15,16},
 {1,4,6,7,10,11,13,16},
 {1,6,10,11,12,13},
 {1,4,5,7,8,9,10,12,13,16}
};

Letter::Letter(char _input, int _index){
    index = _index;
    input = _input;
    pos = ofApp::grids[index]->pos;
    grids.resize(16);
}

void Letter::setup(){
    
    // calculate sum of proportions
    double colProportionSum = 0;
    double rowProportionSum = 0;
    
    for(int i = 0; i < 4; i ++ ){
        colProportionSum += colProportion[i];
        rowProportionSum += rowProportion[i];
    }
    cout << colProportionSum << " " << rowProportionSum << endl;
    
    double currentX = pos.x;
    for(int c = 0; c < 4; c ++ ){
        double currentY = pos.y;
        for (int r = 0; r < 4; r ++ ){
            // set up individual module width and module height
            double tempW = colProportion[c] * ofApp::moduleWidth / colProportionSum;
            double tempH = rowProportion[r] * ofApp::moduleHeight / rowProportionSum;
            
            ofVec2f tempPos = ofVec2f(currentX, currentY);
            shared_ptr<Grid> tempModule = make_shared<Grid>(tempPos, "letter", tempW, tempH);
            grids[r + c * 4] = tempModule;
            currentY += tempH;
        }
        currentX += colProportion[c] * ofApp::moduleWidth / colProportionSum;
    }
    
    // set up grids
    for(int i = 0; i < grids.size(); i ++ ){
        grids[i]->setup();
    }
    
}

void Letter::update(ofxSlider<double> &cp1, ofxSlider<double> &cp2, ofxSlider<double> &cp3, ofxSlider<double> &cp4, ofxSlider<double> &rp1, ofxSlider<double> &rp2, ofxSlider<double> &rp3, ofxSlider<double> &rp4){
    
    // update proportion
    colProportion = {cp1, cp2, cp3, cp4};
    rowProportion = {rp1, rp2, rp3, rp4};
    
    // calculate sum of proportions
    double colProportionSum = 0;
    double rowProportionSum = 0;
    
    for(int i = 0; i < 4; i ++ ){
        colProportionSum += colProportion[i];
        rowProportionSum += rowProportion[i];
    }
    
    // update moudle
    double currentX = pos.x;
    for(int c = 0; c < 4; c ++ ){
        double currentY = pos.y;
        for (int r = 0; r < 4; r ++ ){
            // set up individual module width and module height
            double tempW = colProportion[c] * ofApp::moduleWidth / colProportionSum;
            double tempH = rowProportion[r] * ofApp::moduleHeight / rowProportionSum;
            
            ofVec2f tempPos = ofVec2f(currentX, currentY);
            shared_ptr<Grid> tempModule = make_shared<Grid>(tempPos, "letter", tempW, tempH);
            grids[r + c * 4] = tempModule;
            currentY += tempH;
        }
        currentX += colProportion[c] * ofApp::moduleWidth / colProportionSum;
    }
    
    // set up grids
    for(int i = 0; i < grids.size(); i ++ ){
        grids[i]->setup();
    }
    
}

void Letter::draw(){
    int letterIndex = input - 0x0 - 97; // convert input to integer
    //cout << letterIndex << endl;
    for(int j = 0; j < letters[letterIndex].size(); j ++ ){
        int tempIndex = letters[letterIndex][j] - 1;
        grids[tempIndex]->draw();
    }
}
