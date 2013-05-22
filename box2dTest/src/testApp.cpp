#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofBackground(24, 80, 65);
    
    mBox2d.init();
    mBox2d.setGravity(0, 10);
    mBox2d.createBounds();
    mBox2d.setFPS(30.0);
    mBox2d.registerGrabbing();
    
    mImg.loadImage("test.png");
}

//--------------------------------------------------------------
void testApp::update(){
    
    mBox2d.update();
    
}

//--------------------------------------------------------------
void testApp::draw(){

    
    for (int i = 0; i < mCircles.size(); i++) {
        ofSetColor(255, 255, 255);
//        mCircles[i].draw();
        
        ofPoint pos = mCircles[i].getPosition();
//        ofCircle(pos, mCircles[i].getRadius());
        mImg.draw(pos, mImg.getWidth()/6, mImg.getHeight()/6);
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

    float r = ofRandom(4, 20);
    ofxBox2dCircle circle;
    circle.setPhysics(3.0, 0.53, 0.1);
//    circle.setup(mBox2d.getWorld(), mouseX-r/2, mouseY-r/2, r);
    circle.setup(mBox2d.getWorld(), mouseX-mImg.getWidth()/6, mouseY-mImg.getHeight()/6, mImg.getHeight()/6);
    mCircles.push_back(circle);
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}