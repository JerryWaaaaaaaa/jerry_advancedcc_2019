//
//  Animal.cpp
//  assignment2
//
//  Created by wenhe on 2019/2/23.
//

#include "Animal.hpp"
#include <ofMain.h>

Animal::Animal() = default;

void Animal::setup(ofVec2f _pos, vector<ofImage> _imgs) {
    pos = _pos;
    imgs = _imgs;
    ofLog() << _imgs.size();
    baseVel = ofVec2f(6,0);
    vel = baseVel;
    current_frame = 0;
    xOffset = ofRandom(0,20);
}

void Animal::update() {
    
    // update vel
    xNoise = ofMap(ofNoise(xOffset),0,1,0.2,2);
    vel = baseVel*xNoise;
    
    //update xOffset
    xOffset += 0.02;
    
    // update pos
    pos += vel;
    counter += 1;
    
    // switch to the next image every ten frames
    if (counter >= vel.x) {
        current_frame += 1;
        counter = 0;
    }
    
    current_frame %= imgs.size();
    
    // check if image is out of boundry
    if(pos.x >= ofGetWidth() - imgs[current_frame].getWidth() && !is_on_side) {
        is_on_side = true;
    }
    
    if (pos.x >= ofGetWidth()) {
        is_on_side = false;
        pos.x = 0;
    }
    
    
}

void Animal::draw() {
    imgs[current_frame].draw(pos, 400, 250);
    if (is_on_side) {
        float temp_pos = pos.x - ofGetWidth();
        imgs[current_frame].draw(temp_pos, pos.y, 400, 250);
    }
}


