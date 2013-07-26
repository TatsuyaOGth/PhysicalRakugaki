#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetVerticalSync(true);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofBackground(24, 80, 65);
    ofSetLogLevel(OF_LOG_VERBOSE);
//    ofSetWindowShape(1024, 768);
    
    if (xml.loadFile("def_settings.xml")) {
        mKinectAngle = xml.getValue("kinect_angle", 0);
        mClipDepth = 0;
    }
    
    //----------
    // GUI setup
    //----------
    gui.setup("Controller", 0, 0, 300, 240);
    gui.setXMLFilename("gui_setting_" + ofToString(TEST_MODE));
    gui.setMinimized(true);
    gui.addPanel("settings", 1);
    gui.addSlider(GUI_SLIDER_01, GUI_SLIDER_01, 0.5, 0.0, 1.0, false);
    gui.addSlider(GUI_SLIDER_02, GUI_SLIDER_02, 0.5, 0.0, 1.0, false);
    gui.addSlider(GUI_SLIDER_03, GUI_SLIDER_03, 0.5, 0.0, 1.0, false);
    
    //----------
    // Box2d setup
    //----------
	mBox2d.init();
	mBox2d.setGravity(0, 5);
	mBox2d.createGround();
	mBox2d.setFPS(30.0);
    //mBox2d.createBounds();
    //mBox2d.registerGrabbing();
    
    //----------
    // Kinect setup
    //----------
#ifdef ENABLE_KINECT
	kinect.setRegistration(true);
	kinect.init(false, false);
    kinect.open();
    kinect.setCameraTiltAngle(mKinectAngle);
    mDepthImage.allocate(kinect.width, kinect.height);
#endif
    
    getAndSetRakugaki("rakugaki_test");
    
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
            
        case JUMP:
            updateJump();
            break;
    }
    mBox2d.update();
    gui.update();
#ifdef ENABLE_KINECT
    kinect.update();
#endif
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
            
        case JUMP:
            drawJump();
            break;
    }
    
    //----------
    // debug text
    //----------
    if (bDebugView) {
        ofSetColor(255);
        stringstream s;
        s << "fps: " << ofToString(ofGetFrameRate(), 2) << endl;
        s << "window size: " << ofGetWidth() << " " << ofGetHeight() << endl;
        s << "physic circles: " << mPsCircles.size() << endl;
        s << "mode id: " << mMode << endl;
        s << "rakufaki image size: " << mRakugakis.size() << endl;
        s << "finded contours: " << mContour.nBlobs << endl;
        s << "kinect angle: " << mKinectAngle << endl;
        s << "clip depth: " << mClipDepth << endl;
        for (int i = 0; i < bFunc.size(); i++) s << "func " << i << ": " << bFunc[i] << endl;
        ofDrawBitmapString(s.str(), 10, 35);
        gui.draw();
        
        //kinect image
        mDepthImage.draw(0, 0);
        mContour.draw();
    }
    getKinectContoursPts();
}

//--------------------------------------------------------------
void testApp::exit(){
#ifdef ENABLE_KINECT
    kinect.setCameraTiltAngle(0);
#endif
    xml.saveFile();
}

//============================================================== setup

void testApp::setupRain()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 10);
//	mBox2d.createGround();
	mBox2d.setFPS(24.0);
    gui.loadSettings(getGuiFileName());
}

void testApp::setupSnow()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 1);
    mBox2d.createGround();
	mBox2d.setFPS(24.0);
    gui.loadSettings(getGuiFileName());
}

void testApp::setupJump()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 3);
//    mBox2d.createGround();
	mBox2d.setFPS(24.0);
    gui.loadSettings(getGuiFileName());
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
        c.circle.setVelocity(ofRandom(-1, 1), 0);
		mPsCircles.push_back(c);
	}
}

void testApp::updateJump()
{
    if((int)ofRandom(0, 10) == 0) {
        int jumpV = (int)(gui.getValueF(GUI_SLIDER_01) * 15);
        float xV = gui.getValueF(GUI_SLIDER_02) * 5;
        
		psCircle c;
        c.ID = (int)ofRandom(mRakugakis.size());
		c.circle.setPhysics(1, 0.8, 0.2);
		c.circle.setup(mBox2d.getWorld(), ofRandom(ofGetWidth()), ofGetHeight(), ofRandom(20, 30));
        c.circle.setVelocity(ofRandom(-xV, xV), -jumpV); //pop up
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
        //draw rakugaki
        if (mRakugakis.size()) {
            float size = (it->circle.getRadius() * 3);
            float x = it->circle.getPosition().x - (size/2);
            float y = it->circle.getPosition().y - (size/2);
            mRakugakis[it->ID].draw(x, y, size, size);
        }
        if (bDebugView) {
            ofNoFill();
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
        //draw rakugaki
        if (mRakugakis.size()) {
            float size = (it->circle.getRadius() * 3);
            float x = it->circle.getPosition().x - (size/2);
            float y = it->circle.getPosition().y - (size/2);
            mRakugakis[it->ID].draw(x, y, size, size);
        }
        if (bDebugView) {
            ofNoFill();
            ofSetColor(255, 255, 255, (*it).life);
            (*it).circle.draw();
        }
        
        //delete
        if ((*it).circle.getPosition().y + (*it).circle.getRadius() >= ofGetHeight()-10) (*it).touch = true;
        if ((*it).touch) {
            (*it).circle.setRadius((*it).circle.getRadius() - 0.1);
            it->life -= 1;
        }
        if ((*it).circle.getRadius() <= 0) {
            it = mPsCircles.erase(it);
        } else {
            it++;
        }
    }
}

void testApp::drawJump()
{
    vector<psCircle>::iterator it = mPsCircles.begin();
    while (it != mPsCircles.end()) {
        //draw rakugaki
        if (mRakugakis.size()) {
            float size = (it->circle.getRadius() * 3);
            float x = it->circle.getPosition().x - (size/2);
            float y = it->circle.getPosition().y - (size/2);
            mRakugakis[it->ID].draw(x, y, size, size);
        }
        if (bDebugView) {
            ofNoFill();
            ofSetColor(255, 255, 255);
            (*it).circle.draw();
        }
        //delete
        if ((*it).circle.getPosition().y > ofGetHeight() + (*it).circle.getRadius()) {
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
            mClipDepth--;
            if (mClipDepth < 0) mClipDepth = 0;
            break;
        case OF_KEY_RIGHT:
            mClipDepth++;
            if (mClipDepth > 255) mClipDepth = 255;
            break;
        case OF_KEY_UP:
#ifdef ENABLE_KINECT
            mKinectAngle++;
            if (mKinectAngle > 30) mKinectAngle = 30;
            kinect.setCameraTiltAngle(mKinectAngle);
            xml.setValue("kinect_angle", mKinectAngle);
#endif
            break;
        case OF_KEY_DOWN:
#ifdef ENABLE_KINECT
            mKinectAngle--;
            if (mKinectAngle < -30) mKinectAngle = -30;
            kinect.setCameraTiltAngle(mKinectAngle);
            xml.setValue("kinect_angle", mKinectAngle);
#endif
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
            
        case 'r':
            getAndSetRakugaki("rakugaki_test");
            break;
            
        //1~9,0
        case '1': mMode = TEST_MODE; break;
        case '2': mMode = TITLE; break;
        case '3': mMode = RAIN_DROP; setupRain(); break;
        case '4': mMode = SNOW_FALL; setupSnow(); break;
        case '5': mMode = JUMP; setupJump(); break;
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
    
    // set gui file name
    gui.setXMLFilename(getGuiFileName());
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    gui.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    gui.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    gui.mouseReleased();
    gui.saveSettings(); //audo save
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
            
        case JUMP:
            setupJump();
            break;
    }
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void testApp::getAndSetRakugaki(const string &path)
{
    if (mDir.listDir(path) != 0) {
        mRakugakis.clear();
        for (int i = 0; i < mDir.size(); i++) {
            ofImage tImg;
            if (tImg.loadImage(mDir.getPath(i))) {
                mRakugakis.push_back(tImg);
                cout << "load rakugaki : " << mDir.getName(i) << endl;
            } else {
                cout << "faild load rakugaki : " << mDir.getName(i) << endl;
            }
        }
        cout << "set rakugaki images " << mRakugakis.size() << endl;
    } else {
        cout << "faild load rakufaki images" << endl;
    }
}

string testApp::getGuiFileName()
{
    return "gui_settings_" + ofToString(mMode) + ".xml";
}

void testApp::getKinectContoursPts()
{
#ifdef ENABLE_KINECT
    if (kinect.isFrameNew()) {
        int size = kinect.width * kinect.height;
        mDepthImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        unsigned char* px = new unsigned char[size];
        for (int i = 0; i < size; i++) {
            if (mDepthImage.getPixels()[i] < mClipDepth) {
                px[i] = 0;
            } else {
                px[i] = mDepthImage.getPixels()[i];
            }
        }
        mDepthImage.setFromPixels(px, kinect.width, kinect.height);
        mContour.findContours(mDepthImage, 10, mDepthImage.width * mDepthImage.height, 20, false);        
        delete px;
    }
#endif
    return ;
}
