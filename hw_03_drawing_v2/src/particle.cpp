//
//  particle.cpp
//  hw_03_drawing
//
//  Created by Jerry-Wanglaws on 3/10/19.
//

#include "particle.hpp"
#include <ofMain.h>
#include <ofxOpenCv.h>


Particles::Particles() = default;

void Particles::setup(int _xMax, int _yMax, int _scl, int _col){
    xMax = _xMax;
    yMax = _yMax;
    scl = _scl;
    col = _col;
    pos = ofVec2f(floor(xMax), floor(yMax));
    prePos = pos;
    vel = ofVec2f(0,0);
    acc = ofVec2f(0,0);
}

void Particles::update(){
    vel += acc;
    // set a limit of the max velocity
    ofVec2f limitedVel = vel.limit(5);
    // add the limited velocity to position
    pos += limitedVel;
    acc = acc * 0;
}


void Particles::draw(ofImage _img){
    
    img = _img;
    
    //ofSetColor(50,20);
    //ofColor color = img.getColor(floor(pos.x), floor(pos.y));
    ofPixels pixels = img.getPixels();
    
    int pIndex = floor(pos.x) + floor(pos.y) * int(img.getWidth());
    
    int red = pixels[pIndex * 3];
    int green = pixels[pIndex * 3 + 1];
    int blue = pixels[pIndex * 3 + 2];
    
    int greyscale = round(red * 0.222 + green * 0.707 + blue * 0.071);
    //cout << "greyscale is " << greyscale << endl;
    //if ( greyscale < 200 ) {
        ofSetColor(greyscale, 20);
        ofSetLineWidth(1);
        ofDrawLine(pos.x, pos.y, prePos.x, prePos.y);
    //}
    prePos = pos;
}

void Particles::follow(vector<ofVec2f> _flowField){
    int x = floor(pos.x / scl);
    int y = floor(pos.y / scl);
    long index = int(x + y * col);
    cout << "index is " << index << endl;
    cout << "flowfield is " << _flowField.size() << endl;
    ofVec2f force = _flowField[index];
    //cout << "force is " << force << endl;
    applyForce(force);

}

void Particles::applyForce(ofVec2f _force){
    acc += _force;
    //cout << "acc is " << acc << endl;
}

void Particles::updatePrev(){
    prePos.x = pos.x;
    prePos.y = pos.y;
}

void Particles::edges(){
    if ( pos.x > xMax ) {
        pos.x = 0;
        updatePrev();
        
    }
    if ( pos.x < 0 ) {
        pos.x = xMax;
        updatePrev();
    }
    if ( pos.y > yMax ) {
        pos.y = 0;
        updatePrev();
    }
    if ( pos.y < 0 ) {
        pos.y = yMax;
        updatePrev();
    }
}
