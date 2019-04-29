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
    
    ofSetFrameRate(60);
    
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
    /*
    showGui = true;
    gui.setup();
    gui.add(brightness.setup("brightness", 0, 0, 10));
    gui.add(contrast.setup("copntrast", 0, 0, 10));
     */
    
    // set up videoGrabber
    glm::ivec2 vidSize = glm::ivec2(vidGrabber.getWidth(),vidGrabber.getHeight());
    //cout << vidSize.x << "," << vidSize.y << endl;

    videoSource = &vidGrabber;
    colorImg.allocate(vidSize.x, vidSize.y);
    grayImage.allocate(vidSize.x, vidSize.y);
    grayScale.allocate(vidSize.x, vidSize.y, OF_IMAGE_GRAYSCALE);
    // grayBg.allocate(vidSize.x, vidSize.y);
    // grayDiff.allocate(vidSize.x, vidSize.y);
    
    bLearnBackground = true;
    
    // set up fbo
    pattern.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA);
    pattern.begin();
    ofClear(255,255,255,0);
    pattern.end();
    
    // set up shader
    if(ofIsGLProgrammableRenderer()){
        try {
            formShader.load("shaders/formShader");
            fillShader.load("shaders/fillShader");
        } catch (std::exception e) {
            ofLog() << e.what() << endl;
        }
    }
    
    // set up the font
    font.load("fonts/Exo-Medium.ttf", 20, true, true, true, 0);
    
    // set up the shape original pos
    shapeX = ofRandom(0, ofGetWidth());
    shapeY = ofRandom(0, ofGetHeight());
    
    // set up pdf rendering
    pdfRendering = false;
    oneShot = false;
    
    ofSetBackgroundAuto(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // update video info
    videoSource->update();
    
    if(videoSource->isFrameNew()){
        tracker.update(toCv(*videoSource));
        sendFaceOsc(tracker);
        rotationMatrix = tracker.getRotationMatrix();

        colorImg.setFromPixels(videoSource->getPixels());
        grayImage = colorImg;
        grayImage.brightnessContrast(0.2, 0.7);
        grayScale.setFromPixels(grayImage.getPixels());
        
        // update font size
        if(tracker.getFound()){
            glm::ivec2 leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
            glm::ivec2 rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
            glm::ivec2 faceCenter = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE).getCentroid2D();
            glm::ivec2 mouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH).getCentroid2D();
            float faceWidth = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE).getBoundingBox().getWidth();
            float faceHeight = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE).getBoundingBox().getHeight();
            cp1 = leftEye.x - (faceCenter.x - faceWidth/2);
            cp2 = faceCenter.x - leftEye.x;
            cp3 = rightEye.x - faceCenter.x;
            cp4 = faceCenter.x + faceWidth/2 - rightEye.x;
            rp1 = (leftEye.y + rightEye.y)/2 - (faceCenter.y - faceHeight/2);
            rp2 = faceCenter.y - (leftEye.y + rightEye.y)/2;
            rp3 = mouth.y - faceCenter.y;
            rp4 = faceCenter.y + faceHeight/2 - mouth.y;
        }
    }
    // ofLog() << checkLeftEye << " " << checkRightEye << " " << checkMouse << " " << checkNose << endl;
    
    // update text
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->update(cp1, cp2, cp3, cp4, rp1, rp2, rp3, rp4);
    }
    
    // update drawings in fbo
    updateFBO();
    
    // update the shape position
    shapeX += ofMap(ofNoise(xOffset), 0, 1, -1, 1) * 1;
    shapeY += ofMap(ofNoise(yOffset), 0, 1, -1, 1) * 1;
    xOffset += 0.1;
    yOffset += 0.1;
    if(shapeX > ofGetWidth()){
        shapeX = 0;
    }
    if(shapeY > ofGetHeight()){
        shapeY = 0;
    }
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(oneShot){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    ofBackground(10);
    
    // draw face badsed on different shaders
    if(tracker.getFound()){
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
        
        ofPushView();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        twoTime.drawWireframe();
        ofPopView();
        
        fillShader.end();
        
    } else {
        formShader.begin();
        formShader.setUniform1f("time", ofGetElapsedTimef());
        
        ofPushView();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofSpherePrimitive sphere;
        sphere.set(ofGetWidth()/3, 6);
        sphere.drawWireframe();
        ofPopView();
        
        formShader.end();
    }
    
    // draw FBO
    pattern.draw(0,0);

    // draw text
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->draw();
    }
    
    // draw grid
    // drawGrid();
    
    // draw GUI
    /*
    if(showGui){
       gui.draw();
    }
    */
    
    if( oneShot ){
        ofEndSaveScreenAsPDF();
        oneShot = false;
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
    // videoSource->draw(0,0);
    
    // draw eys mouse and nose
    if(tracker.getFound()){
        checkLeftEye = drawFacePart(ofxFaceTracker::LEFT_EYE, checkLeftEye, 2.0f);
        checkRightEye = drawFacePart(ofxFaceTracker::RIGHT_EYE, checkRightEye, 1.0f);
        checkNose = drawFacePart(ofxFaceTracker::NOSE_BASE, checkNose, 1.0f);
        checkMouse = drawFacePart(ofxFaceTracker::OUTER_MOUTH, checkMouse, 2.0f);
    }
    
    // draw shapes
    // drawShapes();
    
    // draw date and time
    drawDate();
    
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
int ofApp::drawFacePart(const ofxFaceTracker::Feature &feature, int checkSet, float scale){
    ofImage featureImage = grayScale;
    ofVec2f featurePos = tracker.getImageFeature(feature).getCentroid2D();
    featureImage.crop(floor(featurePos.x - 30), floor(featurePos.y - 30), 60, 60);
    float w = (moduleWidth + gutter) * (scale - 1.0f) + moduleWidth;
    float h = (moduleHeight + gutter) * (scale - 1.0f) + moduleHeight;
    featureImage.resize(w, h);
    // featureImage.resize(floor(moduleWidth) * scale, floor(moduleHeight) * scale);
    if(checkSet == 0){
        int row = floor(ofRandom(2, rows-2));
        int col = floor(ofRandom(2, cols-2));
        int index = getIndex(row, col);
        bool set = true;
        // see if all to-be-occupied grid are not solid, if solid, redo the location seeking
        for(int i = 0; i < floor(scale); i ++ ){
            for(int j = 0; j < floor(scale); j ++ ){
                if(grids[index + i + j * cols]->solid){
                    set = false;
                    break;
                }
            }
        }
        
        if(set){
            for(int i = 0; i < floor(scale); i ++ ){
                for(int j = 0; j < floor(scale); j ++ ){
                    grids[index + i + j * cols]->solid = true;
                }
            }
            // featureImage.draw(grids[index]->pos.x, grids[index]->pos.y);
            for(int i = 0; i < floor(featureImage.getWidth()); i += 5){
                for(int j = 0; j < floor(featureImage.getHeight()); j += 5){
                    int tempIndex = i + j * floor(featureImage.getWidth());
                    ofColor tempC = featureImage.getColor(tempIndex);
                    float tempR = ofMap(tempC[0], 0, 255, 0, 3.5);
                    ofSetColor(tempC);
                    ofDrawEllipse(grids[index]->pos.x + i, grids[index]->pos.y + j, tempR, tempR);
                }
            }
            return index;
        }else{
            return 0;
        }
    }else{
        // featureImage.draw(grids[checkSet]->pos.x, grids[checkSet]->pos.y);
        for(int i = 0; i < floor(featureImage.getWidth()); i += 5){
            for(int j = 0; j < floor(featureImage.getHeight()); j += 5){
                int tempIndex = i + j * floor(featureImage.getWidth());
                ofColor tempC = featureImage.getColor(tempIndex);
                float tempR = ofMap(tempC[0], 0, 255, 0, 3.5);
                ofSetColor(tempC);
                ofDrawEllipse(grids[checkSet]->pos.x + i, grids[checkSet]->pos.y + j, tempR, tempR);
            }
        }
        return checkSet;
    }
}

//--------------------------------------------------------------
void ofApp::drawDate(){
    String year = ofToString(ofGetYear(),4);
    String month = ofToString(ofGetMonth(),2);
    String day = ofToString(ofGetDay(),2);
    String hour = ofToString(ofGetHours(),2);
    String minute = ofToString(ofGetMinutes(),2);
    String second = ofToString(ofGetSeconds(),2);
    String currentDate = " current time: " + year + "/" + month + "/" + day + " " + hour + ":" + minute + ":" + second + ". ";
    float stringWidth = font.getStringBoundingBox(currentDate, 0, 0).getWidth();
    ofFill();
    ofSetColor(200);
    
    
    ofPushView();
    ofRotateDeg(90);
    for(int i = 0; i < 5; i ++ ){
       font.drawStringAsShapes(currentDate, fontOffset - stringWidth * 2 + stringWidth * i, 0);
    }
    ofPopView();
    
    ofPushView();
    ofTranslate(ofGetWidth(), ofGetHeight());
    ofRotateDeg(-90);
    for(int i = 0; i < 5; i ++ ){
        font.drawStringAsShapes(currentDate, fontOffset - stringWidth * 2 + stringWidth * i, 0);
    }
    ofPopView();
    
    fontOffset ++;
    if(fontOffset > stringWidth * 2){
        fontOffset = 0;
    }
}

//--------------------------------------------------------------
void ofApp::drawShapes(){
    ofSetColor(250,250,250,220);
    ofDrawEllipse(shapeX, shapeY, 150,150);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'g':
            showGui = !showGui;
            break;
        case 'r':
            pdfRendering = !pdfRendering;
            if( pdfRendering ){
                ofSetFrameRate(12);  // so it doesn't generate tons of pages
                ofBeginSaveScreenAsPDF("recording-"+ofGetTimestampString()+".pdf", true);
            }else{
                ofSetFrameRate(60);
                ofEndSaveScreenAsPDF();
            }
            break;
        case 's':
            if( !pdfRendering ){
                oneShot = true;
            }
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
