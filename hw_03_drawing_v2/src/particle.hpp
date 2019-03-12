//
//  particle.hpp
//  hw_03_drawing
//
//  Created by Jerry-Wanglaws on 3/10/19.
//

#ifndef particle_hpp
#define particle_hpp


#include "ofMain.h"
#include "ofxOpenCv.h"
#include <vector>
//#include <stdio.h>

class Particles{
    
public:
    Particles();
    void setup(int xMax, int yMax);
    void update();
    void draw(ofImage img);
    void follow(vector<ofVec2f> flowField, int scl, int col);
    void applyForce(ofVec2f force);
    void updatePrev();
    void edges();
    
    int size = 1;
    
    int xMax;
    int yMax;
    
    ofImage img;
    ofVec2f prePos;
    ofVec2f pos;
    ofVec2f vel;
    ofVec2f acc;
    
};

#endif /* particle_hpp */
