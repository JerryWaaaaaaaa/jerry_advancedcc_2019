#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    
    rows = 24;
    cols = 16;
    
    // set up event listener for the buttons
    showGrid.addListener(this, &ofApp::displayGrid);
    showImage.addListener(this, &ofApp::displayImage);
    loadImage.addListener(this, &ofApp::uploadImage);
    save.addListener(this, &ofApp::saveFrame);
    
    // set up GUI
    gui.setup();
    gui.add(vertical.setup("vertical", true, gWidth, gHeight));
    gui.add(horizontal.setup("horizontal", false, gWidth, gHeight));
    gui.add(scale.setup("scale", 1.3, 1, 1.8));
    gui.add(loadImage.setup("load image", gWidth, gHeight));
    gui.add(showImage.setup("display image"));
    gui.add(imageCol.setup("image column", 3, 1, cols));
    gui.add(imageRow.setup("image row", 3, 1, rows));
    gui.add(imageWidth.setup("image width", 10, 1, cols));
    gui.add(imageHeight.setup("image height", 12, 1, rows));
    
    gui.add(header.setup("header text", "Poster Generator", "a", "z", gWidth, gHeight * 3));
    gui.add(headerRow.setup("header row", 3, 1, rows));
    gui.add(headerCol.setup("header column", 3, 2, cols));
    
    gui.add(text.setup("text", "Poster Generator is fantastic!", "a", "z", gWidth, gHeight * 6));
    gui.add(bodyRow.setup("body row", 10, 1, rows));
    gui.add(bodyCol.setup("body column", 3, 1, cols));
    
    gui.add(backgroundColor.setup("background color", (255,255,255), (0,0,0), (255,255,255)));
    gui.add(fontColor.setup("font color", (30,30,30), (0,0,0), (255,255,255)));
    
    gui.add(showGrid.setup("show grid"));
    gui.add(save.setup("save as PDF"));
    
    // set up poster size
    pWidth = size2 * scale;
    pHeight = size1 * scale;
    
    // set up poster grid
    origin.x = gWidth + 20;
    origin.y = 20;
    
    moduleWidth = pWidth/(float)cols;
    moduleHeight = pHeight/(float)rows;
    
    // allocate FBO
    fbo.allocate(pWidth, pHeight);
    
    for(int i = 0; i < rows; i ++ ){
        for(int j = 0; j < cols; j ++ ){
            ofVec2f temp;
            temp.x = j * moduleWidth;
            temp.y = i * moduleHeight;
            modules.push_back(temp);
        }
    }
    
    //img.load("building.jpg");
    
    HelveticaBold.load("fonts/Helvetica-Bold.ttf", 40);
    HelveticaLight.load("fonts/Helvetica-light.ttf", 20);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(230, 230, 230);
    
    gui.draw();

    // draw the poster in FBO so as to export it individually
    fbo.begin();
    generatePoster();
    fbo.end();
    
    fbo.draw(origin.x, origin.y);
    
}

void ofApp::generatePoster(){
    // poster background
    ofFill();
    ofSetColor(backgroundColor);
    ofDrawRectangle(modules[0].x, modules[0].y, moduleWidth * cols, moduleHeight * rows);
    
    // poster image
    ofSetColor(255, 255, 255);
    int picIndex = getIndex(imageRow, imageCol);
    if(showImg){
        img.draw(modules[picIndex], moduleWidth * imageWidth, moduleHeight * imageHeight);
    }
    
    // poster header
    int headerIndex = getIndex(headerRow, headerCol);
    ofSetColor(fontColor);
    HelveticaBold.drawString(header, modules[headerIndex].x, modules[headerIndex].y);
    
    // poster body text
    int bodyIndex = getIndex(bodyRow, bodyCol);
    ofSetColor(fontColor);
    HelveticaLight.drawString(text, modules[bodyIndex].x, modules[bodyIndex].y);
    
    // poster grid
    if(show){
        for(int i = 0; i < modules.size(); i ++ ){
            ofNoFill();
            ofSetColor(30, 30, 30);
            ofDrawRectangle(modules[i].x, modules[i].y, moduleWidth, moduleHeight);
        }
    }
}


int ofApp::getIndex(int row, int col){
    int index = (col - 1) + (row - 1) * cols;
    return index;
}

void ofApp::displayGrid(){
    show = !show;
}

void ofApp::displayImage(){
    showImg = !showImg;
}

void ofApp::uploadImage(){
    ofFileDialogResult result = ofSystemLoadDialog("Load image");
    string filepath = result.getPath();
    img.load (filepath);
}

void ofApp::saveFrame(){
    //ofSaveFrame();
    ofBeginSaveScreenAsPDF("poster" + ofToString(counter) + ".pdf", false);
    fbo.draw(origin.x, origin.y);
    ofEndSaveScreenAsPDF();
    counter ++;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
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
