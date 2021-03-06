//
//  Letter.hpp
//  Final_This_Is_Still_Me
//
//  Created by Jerry-Wanglaws on 4/23/19.
//

#pragma once

#ifndef Letter_hpp
#define Letter_hpp

#include <stdio.h>
#include <ofMain.h>
#include <Grid.hpp>
#include <ofxGui.h>

#endif /* Letter_hpp */

class Letter{
    
public:
    
    static vector<double> colProportion; // proportion of each column
    static vector<double> rowProportion; // proportion of each row
    static vector<vector<int>> letters; // alphabatical info
    
    double moduleWidth;
    double moduleHeight;
    
    vector<shared_ptr<Grid>> grids;
    
    int index;
    char input;
    glm::ivec2 pos; // origin position of the letter
    
    Letter(char _input, int _index);
    
    void setup();
    /*
    void update(ofxSlider<double> &cp1, ofxSlider<double> &cp2, ofxSlider<double> &cp3, ofxSlider<double> &cp4, ofxSlider<double> &rp1, ofxSlider<double> &rp2, ofxSlider<double> &rp3, ofxSlider<double> &rp4);
     */
    void update(float cp1, float cp2, float cp3, float cp4, float rp1, float rp2, float rp3, float rp4);
    void draw();
    
    
};
