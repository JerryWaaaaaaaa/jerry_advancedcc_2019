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
    Cell(ofVec2f _pos, ofColor _color, string _type);
   
    void update();
    void draw();
    void updateColor(ofColor color);
    void generateFilter();
    
    ofColor color;
    ofVec2f pos;
    ofVec2f size = ofVec2f(260.0f,260.0f);
    
    int h;
    int s;
    int b;
    bool state;
    string type;

};

#endif /* Cell_hpp */
