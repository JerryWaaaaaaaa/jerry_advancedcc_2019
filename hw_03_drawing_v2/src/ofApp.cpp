#include "ofApp.h"
#include <particle.hpp>
#include <ofxOpenCv.h>

//--------------------------------------------------------------
void ofApp::setup(){
    // set framerate
    ofSetFrameRate(60);
    // don't refresh background
    ofSetBackgroundAuto(false);
    
    // load the test image
    testImg.load("bottle02.jpg");
    
    // load the video grabber
    mVideoGrabber.setVerbose(true);
    mVideoGrabber.setup(320, 240);
    
    mFbo.allocate(mVideoGrabber.getWidth(), mVideoGrabber.getHeight(), GL_RGBA);
    
    // add particles to the array
    for ( int i = 0; i < 230; i ++ ) {
        particles.push_back( make_shared<Particles>() );
    }
    
    for (int i = 0; i < particles.size(); i ++ ) {
        particles[i] -> setup(ofGetWidth(), ofGetHeight(), scl, col);
    }
    
    // build grids
    col = floor(ofGetWidth()/scl);
    row = floor(ofGetHeight()/scl);
    //std::cout << "row: " << row << endl;
    //std::cout << "column: " << col << endl;
    flowField.resize(col * row);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    mVideoGrabber.update();

}

//--------------------------------------------------------------
void ofApp::draw(){
    
    // set the flowfield
    float xOff = 0;
    for ( int x = 0; x < col; x ++ ) {
        float yOff = 0;
        for ( int y = 0; y < row; y ++ ) {
            int index = x + y * col;
            // getting a angle
            float angle = ofNoise(xOff, yOff, zOff) * 360;
            //std::cout << "angle is " << angle << endl;
            // create a vector based on the angle and save it to flowField
            ofVec2f v = ofVec2f(1,0).getRotated(angle);
            flowField[index] = v;
            // draw a line based on the angle
            /*
             ofPushView();
             ofTranslate(x * scl, y * scl);
             ofRotateZDeg(ofVec2f(1,0).angle(v));
             ofSetColor(50, 100);
             ofSetLineWidth(2);
             ofDrawLine(0, 0, scl, 0);
             ofPopView();
             */
            yOff += inc;
        }
        xOff += inc;
    }
    zOff += 0.01;
    
    
    auto tex = mVideoGrabber.getTexture();
    mFbo.begin();
    ofClear(255, 255, 255, 255);
    tex.draw(0, 0);
    mFbo.end();
    // save image region
    //imgSave.grabScreen(0, 0, mVideoGrabber.getWidth(), mVideoGrabber.getHeight());
    
    // draw image based on mode
    if (currentMode == "draw") {
        //colorImg.draw(0, 0, ofGetWidth() - mVideoGrabber.getWidth() / 2, ofGetHeight() - mVideoGrabber.getHeight() / 2);
        grayImage.draw(0, 0, ofGetWidth(), ofGetHeight());
        mFbo.draw(ofGetWidth() - mVideoGrabber.getWidth(), ofGetHeight() - mVideoGrabber.getHeight(), mVideoGrabber.getWidth(), mVideoGrabber.getHeight());
    }
    else if (currentMode == "diff") {
        grayFrame1.draw(0, 0, ofGetWidth(), ofGetHeight());
        grayFrame2.draw(0, 0, ofGetWidth(), ofGetHeight());
        mFbo.draw(ofGetWidth() - mVideoGrabber.getWidth(), ofGetHeight() - mVideoGrabber.getHeight(), mVideoGrabber.getWidth(), mVideoGrabber.getHeight());
    }
    else if (currentMode == "flowField") {
        
        // draw the particles
        for (int i = 0; i < particles.size(); i ++ ) {
            particles[i] -> follow(flowField);
            particles[i] -> update();
            particles[i] -> edges();
            particles[i] -> draw(testImg);
        }
    }
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case ' ':
            colorImg.setFromPixels(mVideoGrabber.getPixels());
            // grab the image from the webcam
            // press space to capture
            if (currentMode == "draw") {
                grayImage = colorImg; //grayscale the image
            }
            
            else if (currentMode == "diff") {
                if (frameCounter == 0) {
                    grayFrame2.clear(); // clear any previous frame
                    grayFrame1 = colorImg;
                    frameCounter += 1;
                }
                else if (frameCounter == 1) {
                    grayFrame2 = colorImg;
                    grayFrame2.absDiff(grayFrame1);
                    frameCounter = 0;
                }
            }
            
            else if (currentMode == "flowField") {
                grayImage = colorImg; //grayscale the image
            }
            
            break;
            
        case '1':
            // press 1 to start drawing
            currentMode = "draw";
            break;
            
        case '2':
            // press 2 for abs diff mode
            currentMode = "diff";
            break;
            
        case '3':
            // press 3 for drawing flowfield
            currentMode = "flowField";
            break;
            
        case OF_KEY_RETURN:
            // press enter to save image
            imgSave.save("img"+to_string(imgSaveNum)+".jpg");
            imgSaveNum += 1;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
