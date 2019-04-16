#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    combos.push_back(colorCombo1);
    combos.push_back(colorCombo2);
    combos.push_back(colorCombo3);
    combos.push_back(colorCombo4);
    combos.push_back(colorCombo5);
    backgrounds.push_back(ofColor(172,210,120));
    backgrounds.push_back(ofColor(245,63,43));
    backgrounds.push_back(ofColor(41,253,46));
    backgrounds.push_back(ofColor(255,253,56));
    backgrounds.push_back(ofColor(247,168,83));
    fonts.push_back(ofColor(251,13,27));
    fonts.push_back(ofColor(11,97,112));
    fonts.push_back(ofColor(155,14,22));
    fonts.push_back(ofColor(255,56,201));
    fonts.push_back(ofColor(56,81,237));
    ofSetVerticalSync(true);
    
    rows = 24;
    cols = 16;
    
    // set up event listener for the buttons
    vertical.addListener(this, &ofApp::checkOrientation);
    showGrid.addListener(this, &ofApp::displayGrid);
    showImage.addListener(this, &ofApp::displayImage);
    loadImage.addListener(this, &ofApp::uploadImage);
    saveAsPDF.addListener(this, &ofApp::saveFrame);
    colorCombo1.addListener(this, &ofApp::changeColor1);
    colorCombo2.addListener(this, &ofApp::changeColor2);
    colorCombo3.addListener(this, &ofApp::changeColor3);
    colorCombo4.addListener(this, &ofApp::changeColor4);
    colorCombo5.addListener(this, &ofApp::changeColor5);
    textExample1.addListener(this, &ofApp::changeText1);
    textExample2.addListener(this, &ofApp::changeText2);
    textExample3.addListener(this, &ofApp::changeText3);
    textExample4.addListener(this, &ofApp::changeText4);
    
    // set up GUI
    gui.setup();
    gui.add(vertical.setup("vertical", gWidth, gHeight));
    gui.add(horizontal.setup("horizontal", gWidth, gHeight));
    gui.add(scale.setup("scale", 1.3, 1, 1.8));
    gui.add(loadImage.setup("load image", gWidth, gHeight));
    gui.add(showImage.setup("display image"));
    gui.add(imageCol.setup("image column", 3, 1, cols));
    gui.add(imageRow.setup("image row", 3, 1, rows));
    gui.add(imageWidth.setup("image width", 10, 1, cols));
    gui.add(imageHeight.setup("image height", 14, 1, rows));
    
    gui.add(header.setup("header text", "Poster Generator", "a", "z", gWidth, gHeight * 3));
    gui.add(headerRow.setup("header row", 19, 1, rows));
    gui.add(headerCol.setup("header column", 3, 2, cols));
    
    gui.add(text.setup("text", "Poster Generator is fantastic!", "a", "z", gWidth, gHeight * 6));
    gui.add(bodyRow.setup("body row", 20, 1, rows));
    gui.add(bodyCol.setup("body column", 3, 1, cols));
    
    gui.add(backgroundColor.setup("background color", (255,255,255), (0,0,0), (255,255,255)));
    gui.add(fontColor.setup("font color", (30,30,30), (0,0,0), (255,255,255)));
    gui.add(colorCombo1.setup("colorCombo1"));
    gui.add(colorCombo2.setup("colorCombo2"));
    gui.add(colorCombo3.setup("colorCombo3"));
    gui.add(colorCombo4.setup("colorCombo4"));
    gui.add(colorCombo5.setup("colorCombo5"));
    gui.add(textExample1.setup("InspirationalText1"));
    gui.add(textExample2.setup("InspirationalText2"));
    gui.add(textExample3.setup("InspirationalText3"));
    gui.add(textExample4.setup("InspirationalText4"));
    gui.add(showGrid.setup("show grid"));
    gui.add(saveAsPDF.setup("save as PDF"));
    
    // set up poster size
    pWidth = size2 * scale;
    pHeight = size1 * scale;
    
    // set up poster grid
    origin.x = gWidth + 20;
    origin.y = 20;
    
    moduleWidth = pWidth/(float)cols;
    moduleHeight = pHeight/(float)rows;
    
    for(int i = 0; i < rows; i ++ ){
        for(int j = 0; j < cols; j ++ ){
            ofVec2f temp;
            temp.x = j * moduleWidth;
            temp.y = i * moduleHeight;
            modules.push_back(temp);
        }
    }
    
    //img.load("building.jpg");
    
    HelveticaBold.load("fonts/Helvetica-Bold.ttf", 40, true, true, true, 0, 0);
    HelveticaLight.load("fonts/Helvetica-light.ttf", 20, true, true, true, 0, 0);
    
    // allocate FBO
    fbo.allocate(pWidth, pHeight);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetBackgroundColor(230, 230, 230);
    
    if(savePDF){
        ofBeginSaveScreenAsPDF("poster-" + ofToString(counter) + ".pdf", false);
        savePDF = !savePDF;
    }

    // draw the poster in FBO so as to export it individually
    fbo.begin();
    generatePoster();
    fbo.end();
    
    fbo.draw(origin.x, origin.y);
    
    ofEndSaveScreenAsPDF();
    
    gui.draw();
    
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
    HelveticaBold.drawStringAsShapes(header, modules[headerIndex].x, modules[headerIndex].y);
    
    // poster body text
    int bodyIndex = getIndex(bodyRow, bodyCol);
    ofSetColor(fontColor);
    HelveticaLight.drawStringAsShapes(text, modules[bodyIndex].x, modules[bodyIndex].y);
    
    // poster grid
    if(show){
        for(int i = 0; i < modules.size(); i ++ ){
            ofNoFill();
            ofSetColor(30, 30, 30);
            ofDrawRectangle(modules[i].x, modules[i].y, moduleWidth, moduleHeight);
        }
    }
}

void ofApp::checkOrientation(){
    // will do this function in coming versions
    verticalDir = !verticalDir;
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
    savePDF = !savePDF;
    counter ++;
}

void ofApp::changeColor1(){
    backgroundColor = backgrounds[0];
    fontColor = fonts[0];
}
void ofApp::changeColor2(){
    backgroundColor = backgrounds[1];
    fontColor = fonts[1];
}
void ofApp::changeColor3(){
    backgroundColor = backgrounds[2];
    fontColor = fonts[2];
}
void ofApp::changeColor4(){
    backgroundColor = backgrounds[3];
    fontColor = fonts[3];
}
void ofApp::changeColor5(){
    backgroundColor = backgrounds[4];
    fontColor = fonts[4];
}
void ofApp::changeText1(){
    header = "Gucci Gang";
    text = "Yes, I like the color BLACK.";
}
void ofApp::changeText2(){
    header = "Send Nudes";
    text = "404 page not found";
}
void ofApp::changeText3(){
    header = "Aesthetic";
    text = "You are not even close";
}
void ofApp::changeText4(){
    header = "EmotionalBoys2001";
    text = "Gorgeous hair is the best revenge.";
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
