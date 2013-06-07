//
// you must add ofxBox2d
// https://github.com/vanderlin/ofxBox2d
//
//

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofBackground(24, 80, 65);
    
    // Box2d
	mBox2d.init();
	mBox2d.setGravity(0, 5);
	mBox2d.createGround();
	mBox2d.setFPS(30.0);
//    mBox2d.createBounds();
//    mBox2d.registerGrabbing();
    
    
    mImg.loadImage("test.png");
    
    bDebugMode = true;
}

//--------------------------------------------------------------
void testApp::update(){
    
    // add some circles every so often
    // 自動的に円を追加（上から）
	if((int)ofRandom(0, 10) == 0) {
		ofxBox2dCircle c;
		c.setPhysics(1, 0.5, 0.1);
		c.setup(mBox2d.getWorld(), ofRandom(50, 200), -20, ofRandom(10, 30));
		mPCircles.push_back(c);
	}
    
    mBox2d.update();

    if (bForcesMode) {
        ofVec2f mouse(ofGetMouseX(), ofGetMouseY());
        float minDis = ofGetMousePressed() ? 300 : 10;
        for(int i = 0; i < mPCircles.size(); i++) {
            float dis = mouse.distance(mPCircles[i].getPosition());
            if (dis < minDis) mPCircles[i].addRepulsionForce(mouse, 2);
            else              mPCircles[i].addAttractionPoint(mouse, 4.0);
        }
    }

    
}

//--------------------------------------------------------------
void testApp::draw(){

    //----------
    // 円の描画
    //----------
    for (int i = 0; i < mPCircles.size(); i++) {
        ofPoint pos = mPCircles[i].getPosition();
        float pr = mPCircles[i].getRadius() * 2;
        mImg.draw(pos.x - pr, pos.y - pr, pr*2, pr*2);
        
        if (bDebugMode) {
            ofSetColor(255, 255, 255);
            ofNoFill();
            mPCircles[i].draw();
        }
    }
    
    //----------
    // 手書き線の描画
    //----------
    ofSetColor(255);
	ofNoFill();
	for (int i = 0; i < mPLines.size(); i++) {
		mPLines[i].draw();
	}
	for (int i = 0; i < mPPolyLines.size(); i++) {
		mPPolyLines[i].draw();
	}
    
    
    //----------
    // debug text
    //----------
    if (bDebugMode) {
        
        ofPushStyle();
        ofSetColor(255);
        stringstream s;
        s << "FPS: " << ofToString(ofGetFrameRate(), 2) << endl;
        s << "physic circles: " << mPCircles.size() << endl;
        ofDrawBitmapString(s.str(), 10, 20);
        ofPopStyle();
    }
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    switch (key) {
        case OF_KEY_LEFT:
            break;
        case OF_KEY_RIGHT:
            break;
        case OF_KEY_UP:
            break;
        case OF_KEY_DOWN:
            break;
        case OF_KEY_BACKSPACE:
        case OF_KEY_DEL:
            break;
        case 'f':
            ofToggleFullscreen();
            break;
            
        case 'c':
            mPLines.clear();
            for (int i=0; i<mPPolyLines.size(); i++)
                mPPolyLines[i].destroy();
            break;
            
        case 'C':
            mPCircles.clear();
            break;
            
        case '1':
            bForcesMode = !bForcesMode;
            break;
            
        case ' ': bDebugMode = !bDebugMode; break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
	mPLines.back().addVertex(x, y);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
	mPLines.push_back(ofPolyline());
	mPLines.back().addVertex(x, y);
    
    // 円を１個追加
//    float r = ofRandom(20, 40);
//    ofxBox2dCircle circle;
//    circle.setPhysics(3.0, 0.53, 0.1);
//    circle.setup(mBox2d.getWorld(), mouseX-r/2, mouseY-r/2, r);
//    mPCircles.push_back(circle);
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
	ofxBox2dPolygon poly;
	mPLines.back().simplify();
	
	for (int i=0; i<mPLines.back().size(); i++) {
		poly.addVertex(mPLines.back()[i]);
	}
	
	//poly.setPhysics(1, .2, 1);  // uncomment this to see it fall!
	poly.create(mBox2d.getWorld());
	mPPolyLines.push_back(poly);
	
	//lines.clear();
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