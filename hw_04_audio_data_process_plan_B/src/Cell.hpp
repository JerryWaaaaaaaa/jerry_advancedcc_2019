//
//  Cell.hpp
//  project_challenge_4
//
//  Created by Casey Pan on 3/31/19.
//

#ifndef Cell_hpp
#define Cell_hpp

#include <stdio.h>
#include "ofMain.h"

class Cell{
    
public:
    
    Cell();
    Cell(ofVec2f _pos, ofColor _color);
   
    void update();
    void draw();
    void updateColor(ofColor color);
    vector<float> generateFilter(float phase);
    
    ofColor color;
    ofVec2f pos;
    ofVec2f size = ofVec2f(170.0f,170.0f);
    
    int h;
    int s;
    int b;
    bool state;
    string type;

};

#endif /* Cell_hpp */
