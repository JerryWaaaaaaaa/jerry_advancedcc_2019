#include "ofApp.h"

double ofApp::moduleWidth;
double ofApp::moduleHeight;
vector<shared_ptr<Grid>> ofApp::grids;

//--------------------------------------------------------------
void ofApp::setup(){
    
    moduleWidth = ( ofGetWidth() - gutter*(cols-1) ) / cols;
    moduleHeight = ( ofGetHeight() - gutter*(rows-1) ) / rows;
    
    grids.resize(cols * rows);
    
    // make the grid
    for(int i = 0; i < rows; i ++ ){
        for(int j = 0; j < cols; j ++ ){
            double x = (moduleWidth + gutter) * j;
            double y = (moduleHeight + gutter) * i;
            glm::ivec2 tempPos = glm::ivec2(x, y);
            shared_ptr<Grid> tempModule = make_shared<Grid>(tempPos, "poster");
            grids[j + i * cols] = tempModule;
        }
    }
    
    // set up the grid
    for(int i = 0; i < grids.size(); i ++ ){
        grids[i]->setup();
    }
    
    // make the header text
    int tRow = textRow;
    int tCol = textCol;
    for(int i = 0; i < header.size(); i ++ ){
        char tempChar = header[i];
        
        if(tempChar != ' '){
            shared_ptr<Letter> tempLetter = make_shared<Letter>(tempChar, getIndex(tRow, tCol));
            letters.push_back(tempLetter);
            tCol ++ ;
        }
        else{
            tRow ++;
            tCol = textCol;
        }

    }
    
    // set up the text
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->setup();
        grids[letters[i]->index]->solid = true; // the grid is occupied
    }
    
    // set up GUI
    gui.setup();
    gui.add(cp1.setup("cp1", 1, 0.5, 3));
    gui.add(cp2.setup("cp2", 1, 0.5, 3));
    gui.add(cp3.setup("cp3", 1, 0.5, 3));
    gui.add(cp4.setup("cp4", 1, 0.5, 3));
    gui.add(rp1.setup("rp1", 1, 0.5, 3));
    gui.add(rp2.setup("rp2", 1, 0.5, 3));
    gui.add(rp3.setup("rp3", 1, 0.5, 3));
    gui.add(rp4.setup("rp4", 1, 0.5, 3));
    gui.add(thresholdChange.setup("threshold", 80, 0, 200));
    
    // set up videoGrabber
    
    vidGrabber.setVerbose(true);
    vidGrabber.initGrabber(320,240);
    glm::ivec2 vidSize = glm::ivec2(vidGrabber.getWidth(),vidGrabber.getHeight());
    cout << vidSize.x << "," << vidSize.y << endl;

    //vidGrabber.setup(320,240);
    colorImg.allocate(vidSize.x, vidSize.y);
    grayImage.allocate(vidSize.x, vidSize.y);
    grayBg.allocate(vidSize.x, vidSize.y);
    grayDiff.allocate(vidSize.x, vidSize.y);
    
    bLearnBackground = true;
    threshold = thresholdChange;
    
    // set up fbo
    pattern.allocate(vidSize.x, vidSize.y, GL_RGBA);
    pattern.begin();
    ofClear(255,255,255,0);
    pattern.end();
    
    ofSetBackgroundAuto(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    // update font size
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->update(cp1, cp2, cp3, cp4, rp1, rp2, rp3, rp4);
    }
    
    // update threshold
    threshold = thresholdChange;
    
    // update video info
    vidGrabber.update();
    
    if(vidGrabber.isFrameNew()){
        colorImg.setFromPixels(vidGrabber.getPixels());
        grayImage = colorImg;
        
        if(bLearnBackground == true){
            grayBg = grayImage;
            bLearnBackground = false;
            cout << "grayBg created!" << endl;
        }
        grayDiff.absDiff(grayBg, grayImage);
        grayDiff.threshold(threshold);
        
        contourFinder.findContours(grayDiff, 0, 320*240, 5, true);
    }
    
    // draw video in fbo
    drawVideo();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(0,0,0);
    
    // draw grid
    /*
    for(int i = 0; i < rows; i ++ ){
        for(int j = 0; j < cols; j ++ ){
            int index = j + i * cols;
            grids[index]->draw();
        }
    }
     */
    
    // draw FBO
    int imgStartPosIndex = getIndex(0, 0);
    int imgEndPosIndex = getIndex(rows - 1, cols - 1);
    glm::ivec2 imgStartPos = grids[imgStartPosIndex]->pos;
    glm::ivec2 imgEndPos = grids[imgEndPosIndex]->pos;
    pattern.draw(imgStartPos.x,imgStartPos.y,imgEndPos.x+moduleWidth-imgStartPos.x,imgEndPos.y+moduleHeight-imgStartPos.y);
    
    // draw text
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->draw();
    }
    
    // draw GUI
    gui.draw();
    
}

//--------------------------------------------------------------
int ofApp::getIndex(int _row, int _col){
    int index = _col + _row * cols;
    return index;
}

//--------------------------------------------------------------
void ofApp::drawVideo(){
    // draw video
    ofSetHexColor(0xffffff);
    
    pattern.begin();
    
    ofClear(255,255,255,0);
    // draw grayBg image
    // grayImage.draw(0,0);

    /*
    //we loop through each of the detected blobs
    //contourFinder.nBlobs gives us the number of detected blob
    ofNoFill();
    ofSetColor(130,224,255);
    ofSetLineWidth(3);
    ofBeginShape();
    for (int i = 0; i < contourFinder.nBlobs; i++){
        //each of our blobs contains a vector<ofPoints> pts
        for(int j=0; j < contourFinder.blobs[i].pts.size(); j+=10){
            ofVertex(contourFinder.blobs[i].pts[j].x, contourFinder.blobs[i].pts[j].y);
        }
    }
    ofEndShape();
    */
    
    /*
    // draw ellipse based on boudingRect position
    for (int i = 0; i < contourFinder.nBlobs; i++){
        //each of our blobs contains a vector<ofPoints> pts
        glm::vec3 pos = contourFinder.blobs[i].boundingRect.getCenter();
        ofFill();
        ofSetColor(230,230,230);
        ofDrawEllipse(pos.x, pos.y, 50, 50);
    }
     */

    pattern.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    bLearnBackground = true;
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
