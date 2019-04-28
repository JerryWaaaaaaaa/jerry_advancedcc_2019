#include "ofApp.h"
#include "ofxXmlPoco.h"

using namespace ofxCv;
using namespace cv;

double ofApp::moduleWidth;
double ofApp::moduleHeight;
vector<shared_ptr<Grid>> ofApp::grids;

//--------------------------------------------------------------
void ofApp::loadSettings(){
    // load setting files
    ofxXmlPoco xml;
    if(!xml.load(ofToDataPath("settings.xml"))) {
        return;
    }
    
    // expects following tags to be wrapped by a main "faceosc" tag
    
    bool bUseCamera = true;
    
    xml.setTo("source");
    if(xml.exists("useCamera")) {
        bUseCamera = xml.getValue("useCamera", false);
    }
    xml.setToParent();
    
    xml.setTo("camera");
    if(xml.exists("device")) {
        vidGrabber.setDeviceID(xml.getValue("device", 0));
    }
    if(xml.exists("framerate")) {
        vidGrabber.setDesiredFrameRate(xml.getValue("framerate", 30));
    }
    camWidth = xml.getValue("width", 600);
    camHeight = xml.getValue("height", 900);
    vidGrabber.initGrabber(camWidth, camHeight);
    xml.setToParent();
    
    xml.setTo("face");
    if(xml.exists("rescale")) {
        tracker.setRescale(xml.getValue("rescale", 1.0));
    }
    if(xml.exists("iterations")) {
        tracker.setIterations(xml.getValue("iterations", 5));
    }
    if(xml.exists("clamp")) {
        tracker.setClamp(xml.getValue("clamp", 3.0));
    }
    if(xml.exists("tolerance")) {
        tracker.setTolerance(xml.getValue("tolerance", 0.01));
    }
    if(xml.exists("attempts")) {
        tracker.setAttempts(xml.getValue("attempts", 1));
    }
    bDrawMesh = true;
    if(xml.exists("drawMesh")) {
        bDrawMesh = (bool) xml.getValue("drawMesh", 1);
    }
    tracker.setup();
    xml.setToParent();
    
    xml.setTo("osc");
    host = xml.getValue("host", "localhost");
    port = xml.getValue("port", 8338);
    osc.setup(host, port);
    xml.setToParent();
    
    xml.clear();
}



//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetVerticalSync(true);
    loadSettings();
    
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
    
    cp1 = cp2 = cp3 = cp4 = 1.0;
    rp1 = rp2 = rp3 = rp4 = 1.0;
    
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
    showGui = true;
    gui.setup();
    gui.add(thresholdChange.setup("threshold", 2, 0, 2));
    
    // set up videoGrabber
    glm::ivec2 vidSize = glm::ivec2(vidGrabber.getWidth(),vidGrabber.getHeight());
    //cout << vidSize.x << "," << vidSize.y << endl;

    videoSource = &vidGrabber;
    colorImg.allocate(vidSize.x, vidSize.y);
    grayImage.allocate(vidSize.x, vidSize.y);
    // grayBg.allocate(vidSize.x, vidSize.y);
    // grayDiff.allocate(vidSize.x, vidSize.y);
    
    bLearnBackground = true;
    threshold = thresholdChange;
    
    // set up fbo
    pattern.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    pattern.begin();
    ofClear(255,255,255,0);
    pattern.end();
    
    // set up shader
    if(ofIsGLProgrammableRenderer()){
        try {
            wireFrameShader.load("faceShader/wireFrameShader");
            fillShader.load("faceShader/fillShader");
        } catch (std::exception e) {
            ofLog() << e.what() << endl;
        }
    }
    
    ofSetBackgroundAuto(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // update threshold
    threshold = thresholdChange;
    
    // update video info
    videoSource->update();
    
    if(videoSource->isFrameNew()){
        tracker.update(toCv(*videoSource));
        sendFaceOsc(tracker);
        rotationMatrix = tracker.getRotationMatrix();

        colorImg.setFromPixels(videoSource->getPixels());
        grayImage = colorImg;
        
        // update font size
        glm::ivec2 leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
        glm::ivec2 rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
        glm::ivec2 faceCenter = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE).getCentroid2D();
        glm::ivec2 mouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH).getCentroid2D();
        double faceWidth = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE).getBoundingBox().getWidth();
        double faceHeight = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE).getBoundingBox().getHeight();
        cp1 = leftEye.x - (faceCenter.x - faceWidth/2);
        cp2 = faceCenter.x - leftEye.x;
        cp3 = rightEye.x - faceCenter.x;
        cp4 = faceCenter.x + faceWidth/2 - rightEye.x;
        rp1 = (leftEye.y + rightEye.y)/2 - (faceCenter.y - faceHeight/2);
        rp2 = faceCenter.y - (leftEye.y + rightEye.y)/2;
        rp3 = mouth.y - faceCenter.y;
        rp4 = faceCenter.y + faceHeight/2 - mouth.y;
        
        
    }else{
        // grayImage.set(220);
    }
    
    // update text
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->update(cp1, cp2, cp3, cp4, rp1, rp2, rp3, rp4);
    }
    
    // update drawings in fbo
    updateFBO();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    ofBackground(10);
    
    // draw grid
    // drawGrid();
    
    // draw face badsed on different shaders
    ofMesh face = tracker.getImageMesh();;
    ofMesh oneTime = face;
    ofMesh twoTime = face;
    
    glm::vec3 centroid = face.getCentroid();
    
    for( int i = 0; i < twoTime.getNumVertices(); i++ ) {
        oneTime.getVertices()[i] = (oneTime.getVertices()[i]-centroid) * 0.4;
        twoTime.getVertices()[i] = (twoTime.getVertices()[i]-centroid) * 3;
    }
    
    // draw the face based on shader
    fillShader.begin();
    /*
    r += rStep;
    b += bStep;
    if(r > 1 || r < 0){
        rStep = -1 * rStep;
    }
    if(b > 1 || b < 0){
        bStep = -1 * bStep;
    }
    fillShader.setUniform1f("r", r);
    fillShader.setUniform1f("b", r);
    // draw the small mesh in wireframe
    for(int i = 0; i < grids.size(); i ++ ){
        ofPushView();
        ofTranslate(grids[i]->pos.x + moduleWidth/2, grids[i]->pos.y + moduleHeight/2);
        oneTime.drawWireframe();
        ofPopView();
    }
    fillShader.end();
    */
    // draw the small mesh in wireframe
    ofPushView();
    ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
    twoTime.drawWireframe();
    ofPopView();
    fillShader.end();
    
    // draw FBO
    pattern.draw(0,0);

    // draw text
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->draw();
    }
    
    // draw GUI
    if(showGui){
       gui.draw();
    }

}

//--------------------------------------------------------------
int ofApp::getIndex(int _row, int _col){
    int index = _col + _row * cols;
    return index;
}

//--------------------------------------------------------------
void ofApp::updateFBO(){
    
    // draw the image based on FBO
    pattern.begin();
    
    ofClear(255,255,255,0);
    // draw grayBg image
    // videoSource->draw(0,0);
    
    // draw eys mouse and nose
    checkLeftEye = drawFacePart(ofxFaceTracker::LEFT_EYE, checkLeftEye, 2);
    checkRightEye = drawFacePart(ofxFaceTracker::RIGHT_EYE, checkRightEye, 1);
    checkNose = drawFacePart(ofxFaceTracker::NOSE_BRIDGE, checkNose, 1);
    checkMouse = drawFacePart(ofxFaceTracker::OUTER_MOUTH, checkMouse, 2);
    
    pattern.end();
    
}

//--------------------------------------------------------------
void ofApp::drawGrid(){
     for(int i = 0; i < rows; i ++ ){
         for(int j = 0; j < cols; j ++ ){
             int index = j + i * cols;
             grids[index]->draw();
         }
     }
}

//--------------------------------------------------------------
int ofApp::drawFacePart(ofxFaceTracker::Feature feature, int checkSet, int scale){
    ofImage featureImage;
    glm::ivec2 featurePos = tracker.getImageFeature(feature).getCentroid2D();
    featureImage.setFromPixels(grayImage.getPixels());
    featureImage.crop(featurePos.x - 30, featurePos.y - 30, 60, 60);
    featureImage.resize(floor(moduleWidth) * scale, floor(moduleHeight) * scale);
    if(checkSet == 0){
        int row = floor(ofRandom(2, rows-3));
        int col = floor(ofRandom(2, cols-2));
        int index = getIndex(row, col);
        if(!grids[index]->solid){
            grids[index]->solid = true;
            featureImage.draw(grids[index]->pos.x, grids[index]->pos.y);
            return index;
        }else{
            return 0;
        }
    }else{
        featureImage.draw(grids[checkSet]->pos.x, grids[checkSet]->pos.y);
        return checkSet;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'b':
            bLearnBackground = true;
            break;
        case 'g':
            showGui = !showGui;
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
