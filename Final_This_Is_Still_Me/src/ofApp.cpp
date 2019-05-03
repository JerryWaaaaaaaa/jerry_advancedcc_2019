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
        tracker.setTolerance(xml.getValue("tolerance", 0.1));
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
    ofEnableSmoothing();
    
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
            glm::vec2 tempPos = glm::vec2(x, y);
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
    showGui = false;
    gui.setup();
    gui.add(brightness.setup("brightness", 0.39, 0, 1));
    gui.add(contrast.setup("copntrast", 0.515, 0, 1));
    
    // set up videoGrabber
    glm::vec2 vidSize = glm::vec2(vidGrabber.getWidth(),vidGrabber.getHeight());
    //cout << vidSize.x << "," << vidSize.y << endl;

    videoSource = &vidGrabber;
    colorImg.allocate(vidSize.x, vidSize.y);
    grayImage.allocate(vidSize.x, vidSize.y);
    grayScale.allocate(vidSize.x, vidSize.y, OF_IMAGE_GRAYSCALE);
    // grayBg.allocate(vidSize.x, vidSize.y);
    // grayDiff.allocate(vidSize.x, vidSize.y);

    // set up fbo
    // IMPORTANT: using the MSAA method to have a better polyline
    pattern.allocate(ofGetWidth(), ofGetHeight(), GL_RGBA, 1);
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
    font.load("fonts/Exo-Medium.ttf", 18, true, true, true, 0.1);
    
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
        grayImage.brightnessContrast(brightness, contrast);
        grayScale.setFromPixels(grayImage.getPixels());
        
        // update font size
        if(tracker.getFound()){
            glm::vec2 leftEye = tracker.getImageFeature(ofxFaceTracker::LEFT_EYE).getCentroid2D();
            glm::vec2 rightEye = tracker.getImageFeature(ofxFaceTracker::RIGHT_EYE).getCentroid2D();
            glm::vec2 faceCenter = tracker.getImageFeature(ofxFaceTracker::FACE_OUTLINE).getCentroid2D();
            glm::vec2 mouth = tracker.getImageFeature(ofxFaceTracker::OUTER_MOUTH).getCentroid2D();
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
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    if(oneShot){
        ofBeginSaveScreenAsPDF("screenshot-"+ofGetTimestampString()+".pdf", false);
    }
    
    ofBackground(10);
    
    // draw FBO
    // pattern.draw(0,0);
    
    
    // draw ofImage based on my FBO
    ofPixels pix;
    pattern.getTexture().readToPixels(pix);
    ofImage img(pix);
    img.draw(0, 0);
    
    // draw GUI
    
    if(showGui){
       gui.draw();
    }
    
    
    if( oneShot ){
        ofEndSaveScreenAsPDF();
        oneShot = false;
    }

}

//--------------------------------------------------------------
void ofApp::updateFBO(){
    
    // draw the image based on FBO
    pattern.begin();
    
    ofClear(255,255,255,0);
    // videoSource->draw(0,0);
    
    // draw shaders
    drawShaders();
    
    // draw eys mouse and nose
    if(tracker.getFound()){
        checkLeftEye = drawFacePart(ofxFaceTracker::LEFT_EYE, checkLeftEye, 2.0f);
        checkRightEye = drawFacePart(ofxFaceTracker::RIGHT_EYE, checkRightEye, 1.0f);
        checkRightEye2 = drawFacePart(ofxFaceTracker::RIGHT_EYE, checkRightEye2, 3.0f);
        checkNose = drawFacePart(ofxFaceTracker::NOSE_BASE, checkNose, 1.0f);
        checkMouse = drawFacePart(ofxFaceTracker::OUTER_MOUTH, checkMouse, 2.0f);
    }
    
    // draw date and time
    drawDate();
    
    // draw text
    for(int i = 0; i < letters.size(); i ++ ){
        letters[i]->draw();
    }
    
    pattern.end();
    
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
        int row = floor(ofRandom(1, rows-3));
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
                float tempR = ofMap(tempC[0], 0, 255, 0.4, 3);
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
    ofSetColor(220);
    
    
    ofPushView();
    ofRotateDeg(90);
    for(int i = 0; i < 5; i ++ ){
       font.drawString(currentDate, fontOffset - stringWidth * 2 + stringWidth * i, 0);
    }
    ofPopView();
    
    ofPushView();
    ofTranslate(ofGetWidth(), ofGetHeight());
    ofRotateDeg(-90);
    for(int i = 0; i < 5; i ++ ){
        font.drawString(currentDate, fontOffset - stringWidth * 2 + stringWidth * i, 0);
    }
    ofPopView();
    
    fontOffset ++;
    if(fontOffset > stringWidth * 2){
        fontOffset = 0;
    }
}

//--------------------------------------------------------------
void ofApp::drawShaders(){
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
        // twoTime.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        twoTime.drawWireframe();
        ofPopView();
        
        fillShader.end();
        
    } else {
        
        // draw the sphere based on the shader
        formShader.begin();
        formShader.setUniform1f("time", ofGetElapsedTimef());
        

        ofPushView();
        ofTranslate(ofGetWidth()/2, ofGetHeight()/2);
        ofSpherePrimitive sphere;
        sphere.set(ofGetWidth()/3, 6);
        // sphere.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        sphere.drawWireframe();
        ofPopView();
        
        formShader.end();
        
        // draw the text hint
        drawCenteredString("Please find a better lighting for your face.", ofGetWidth()/2, ofGetHeight()*2/3);
        
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case 'g':
            showGui = !showGui;
            break;
        case 's':
            if( !pdfRendering ){
                oneShot = true;
            }
            break;
        case ' ':
            // reset the feature location
            for(int i = 0; i < grids.size(); i ++ ){
                grids[i]->solid = false;
            }
            for(int i = 0; i < letters.size(); i ++ ){
                grids[letters[i]->index]->solid = true; // the grid is occupied
            }
            checkLeftEye = 0;
            checkRightEye = 0;
            checkRightEye2 = 0;
            checkMouse = 0;
            checkNose = 0;
            break;
    }
}

//--------------------------------------------------------------
int ofApp::getIndex(int _row, int _col){
    int index = _col + _row * cols;
    return index;
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
void ofApp::drawCenteredString(string output, float x, float y){
    
    float textWidth = font.getStringBoundingBox(output, x, y).getWidth();
    float textHeight = font.getStringBoundingBox(output, x, y).getHeight();
    glm::vec2 center = glm::vec2(x - textWidth/2,y - textHeight/2);
    
    ofPushView();
    ofTranslate(center.x, center.y);
    font.drawString(output, 0, 0);
    ofPopView();
}
