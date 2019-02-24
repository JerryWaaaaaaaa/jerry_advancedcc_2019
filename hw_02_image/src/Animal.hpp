//
//  Animal.hpp
//  assignment2
//
//  Created by wenhe on 2019/2/23.
//

#ifndef Animal_hpp
#define Animal_hpp

#include "ofMain.h"
#include <vector>

using namespace std;

class Animal {
public:
    Animal();
    void setup(ofVec2f pos, vector<ofImage> imgs);
    void update();
    void draw();
    
    void select();
    void jump();
    
    vector<ofImage> imgs;
    ofVec2f pos;
    ofVec2f vel;
    ofVec2f baseVel;
    int current_frame;
    int counter;
    float xNoise;
    float xOffset;
    bool is_on_side;
    
};


#endif /* Animal_hpp */
