#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofBackground(24, 80, 65);
//    ofSetWindowShape(1024, 768);
    
    //----------
    // Box2d setup
    //----------
	mBox2d.init();
	mBox2d.setGravity(0, 5);
	mBox2d.createGround();
	mBox2d.setFPS(30.0);
    //mBox2d.createBounds();
    //mBox2d.registerGrabbing();
    
    bDebugView = true;
    mMode = TEST_MODE;
    for (int i = 0; i < 9; i++) bFunc[i] = 0;
    
    // TITLE
    mFont.loadFont("ComicSansMS.ttf", 80);
}

//--------------------------------------------------------------
void testApp::update(){
    switch (mMode) {
        case TEST_MODE:
            break;
            
        case TITLE:
            break;
            
        case RAIN_DROP:
            updateRain();
            break;
            
        case SNOW_FALL:
            updateSnow();
            break;
    }
    mBox2d.update();

}

//--------------------------------------------------------------
void testApp::draw(){
    
    switch (mMode) {
        case TEST_MODE:
            break;
            
        case TITLE:
            drawTitle();
            break;
            
        case RAIN_DROP:
            drawRain();
            break;
            
        case SNOW_FALL:
            drawSnow();
            break;
    }
    
    //----------
    // debug text
    //----------
    if (bDebugView) {
        ofSetColor(255);
        stringstream s;
        s << "FPS: " << ofToString(ofGetFrameRate(), 2) << endl;
        s << "physic circles: " << mPsCircles.size() << endl;
        s << "mode: " << mMode << endl;
        for (int i = 0; i < bFunc.size(); i++) s << "func " << i << ": " << bFunc[i] << endl;
        ofDrawBitmapString(s.str(), 10, 40);
    }
}

//============================================================== setup

void testApp::setupRain()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 10);
    //	mBox2d.createGround();
	mBox2d.setFPS(24.0);
}

void testApp::setupSnow()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 1);
    mBox2d.createGround();
	mBox2d.setFPS(24.0);
}

//============================================================== update

void testApp::updateTitle()
{
}

void testApp::updateRain()
{
    if((int)ofRandom(0, 2) == 0) {
		psCircle c;
        c.ID = (int)ofRandom(mRakugakis.size());
		c.circle.setPhysics(1, 0.5, 0.1);
		c.circle.setup(mBox2d.getWorld(), ofRandom(ofGetWidth()), -100, ofRandom(3, 10));
		mPsCircles.push_back(c);
	}
}

void testApp::updateSnow()
{
    if((int)ofRandom(0, 3) == 0) {
		psCircle c;
        c.ID = (int)ofRandom(mRakugakis.size());
		c.circle.setPhysics(1, 0.2, 0.8);
		c.circle.setup(mBox2d.getWorld(), ofRandom(ofGetWidth()), -100, ofRandom(3, 10));
		mPsCircles.push_back(c);
	}
}

//============================================================== draw

void testApp::drawTitle()
{
    ofSetColor(80, 255, 120);
    mFont.drawString("RA KU GA KI", 200, 200);
}

void testApp::drawRain()
{
    vector<psCircle>::iterator it = mPsCircles.begin();
    while (it != mPsCircles.end()) {
        if (bDebugView) {
            ofSetColor(255, 255, 255);
            (*it).circle.draw();
        }
        //delete
        if ((*it).circle.getPosition().y - (*it).circle.getRadius() > ofGetHeight()) {
            it = mPsCircles.erase(it);
        } else {
            it++;
        }
    }
}

void testApp::drawSnow()
{
    vector<psCircle>::iterator it = mPsCircles.begin();
    while (it != mPsCircles.end()) {
        if (bDebugView) {
            ofSetColor(255, 255, 255, (*it).life);
            (*it).circle.draw();
        }
        if ((*it).circle.getPosition().y + (*it).circle.getRadius() >= ofGetHeight()-10) (*it).touch = true;
        if ((*it).touch) {
            (*it).circle.setRadius((*it).circle.getRadius() - 0.05);
            (*it).minusLifeTime();
        }
        //delete
        if ((*it).circle.getRadius() <= 0) {
            it = mPsCircles.erase(it);
        } else {
            it++;
        }
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
            
        case ' ':
            bDebugView = !bDebugView;
            break;
            
        //1~9,0
        case '1': mMode = TEST_MODE; break;
        case '2': mMode = TITLE; break;
        case '3': mMode = RAIN_DROP; setupRain(); break;
        case '4': mMode = SNOW_FALL; setupSnow(); break;
        case '5':  break;
        case '6':  break;
        case '7':  break;
        case '8':  break;
        case '9':  break;
        case '0':  break;
            
        //1~9 + shift(jp-key)
        case '!': bFunc[0] = !bFunc[0]; break;
        case '"': bFunc[1] = !bFunc[1]; break;
        case '#': bFunc[2] = !bFunc[2]; break;
        case '$': bFunc[3] = !bFunc[3]; break;
        case '%': bFunc[4] = !bFunc[4]; break;
        case '&': bFunc[5] = !bFunc[5]; break;
        case '\'':bFunc[6] = !bFunc[6]; break;
        case '(': bFunc[7] = !bFunc[7]; break;
        case ')': bFunc[8] = !bFunc[8]; break;
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

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
    //画面サイズが変更されたら初期化する
    switch (mMode) {
        case TEST_MODE:
            break;
            
        case TITLE:
            break;
            
        case RAIN_DROP:
            setupRain();
            break;
            
        case SNOW_FALL:
            setupSnow();
            break;
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}