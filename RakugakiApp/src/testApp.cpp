#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending();
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    ofSetCircleResolution(80);
    ofSetLogLevel(OF_LOG_NOTICE);
    ofBackground(255);
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
    
    //OSC
    receiver.setup(42414);
    
    getAndSetRakugaki("rakugaki_test");
    
    bDebugView = true;
    mMode = TEST_MODE;
    for (int i = 0; i < 9; i++) bFunc[i] = 0;
    bFunc[0] = true;
    
    // TITLE
    mFont.loadFont("ComicSansMS.ttf", 80);
    
    mTargetID = 0;
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
        
        case PACHINCO:
            updatePachinco();
            break;
            
        case DOREMI:
            updateDoremi();
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
    ofSetColor(255);

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
            
        case PACHINCO:
            drawPachinco();
            break;
            
        case DOREMI:
            drawDoremi();
            break;
    }
    
    //----------
    // draw lines
    //----------
    ofSetColor(0);
	ofNoFill();
	for (int i=0; i<mPLines.size(); i++) {
		mPLines[i].draw();
	}
	for (int i=0; i<mPPolyLines.size(); i++) {
		mPPolyLines[i].draw();
	}
    
    //----------
    // debug text
    //----------
    if (bDebugView) {
        ofSetColor(0);
        stringstream s;
        s << "fps: " << ofToString(ofGetFrameRate(), 2) << endl;
        s << "window size: " << ofGetWidth() << " " << ofGetHeight() << endl;
        s << "physic circles: " << mPsCircles.size() << endl;
        s << "mode id: " << mMode << endl;
        s << "rakufaki image size: " << mRakugakis.size() << endl;
        s << "finded contours: " << mContour.nBlobs << endl;
        s << "kinect angle: " << mKinectAngle << endl;
        s << "clip depth: " << mClipDepth << endl;
        s << "target id: " << mTargetID << endl;
        for (int i = 0; i < bFunc.size(); i++) s << "func " << i << ": " << bFunc[i] << endl;
        ofDrawBitmapString(s.str(), 10, 35);
        gui.draw();
        
        //kinect image
//        mDepthImage.draw(0, 0);
        mContour.draw();
//        for (int i = 0; i < mKinectPsCircles.size(); i++) {
//            ofFill();
//            ofSetColor(0, 0, 255);
//            mKinectPsCircles[i].draw();
//        }
        for (int i = 0; i < mKinectPsRect.size(); i++) {
            ofFill();
            ofSetColor(0, 0, 255, 80);
            mKinectPsRect[i].draw();
        }
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
    clearPolyLines();
}

void testApp::setupSnow()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 1);
    mBox2d.createGround();
	mBox2d.setFPS(24.0);
    gui.loadSettings(getGuiFileName());
    clearPolyLines();
}

void testApp::setupJump()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 3);
//    mBox2d.createGround();
	mBox2d.setFPS(24.0);
    gui.loadSettings(getGuiFileName());
    clearPolyLines();
}

void testApp::setupPachinco()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 1);
    mBox2d.createGround();
	mBox2d.setFPS(24.0);
    gui.loadSettings(getGuiFileName());
    clearPolyLines();
}

void testApp::setupDoremi()
{
    mPsCircles.clear();
    mBox2d.init();
    mBox2d.setGravity(0, 4);
//    mBox2d.createGround();
	mBox2d.setFPS(24.0);
    gui.loadSettings(getGuiFileName());
    clearPolyLines();
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

void testApp::updatePachinco()
{
    if (bFunc[0]) {
        int p = (int)(gui.getValueF(GUI_SLIDER_01) * 30) + 3;
        if((int)ofRandom(0, p) == 0) {
            psCircle c;
            c.ID = (int)ofRandom(mRakugakis.size());
            c.circle.setPhysics(1, 0.2, 0.8);
            c.circle.setup(mBox2d.getWorld(), ofRandom(ofGetWidth()), -100, 20);
            c.circle.setVelocity(ofRandom(-1, 1), 0);
            mPsCircles.push_back(c);
        }
    }
}

void testApp::updateDoremi()
{
    while (receiver.hasWaitingMessages()) {
        ofxOscMessage m;
        receiver.getNextMessage(&m);
        if (m.getAddress() == "/play") {
            int jumpV = (int)(gui.getValueF(GUI_SLIDER_01) * 40);
            float xV = gui.getValueF(GUI_SLIDER_02) * 5;
            psCircle c;
            c.ID = m.getArgAsInt32(0);
            c.size = m.getArgAsFloat(1);
            c.circle.setPhysics(1, 0.8, 0.2);
            c.circle.setup(mBox2d.getWorld(), ofRandom(ofGetWidth()), ofGetHeight(), 100 * m.getArgAsFloat(1));
            c.circle.setVelocity(ofRandom(-xV, xV), -(jumpV * m.getArgAsFloat(2)));
            mPsCircles.push_back(c);
        }
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
            float w = mRakugakis[it->ID].width * gui.getValueF(GUI_SLIDER_03);
            float h = mRakugakis[it->ID].height * gui.getValueF(GUI_SLIDER_03);
            float x = it->circle.getPosition().x - (w/2);
            float y = it->circle.getPosition().y - (h/2);
            ofSetColor(255);
            mRakugakis[it->ID].draw(x, y, w, h);
        }
        if (bDebugView) {
            ofNoFill();
            ofSetColor(0, 255, 0);
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
            float w = (mRakugakis[it->ID].width * gui.getValueF(GUI_SLIDER_03)) * ((float)it->life/255);
            float h = (mRakugakis[it->ID].height * gui.getValueF(GUI_SLIDER_03)) * ((float)it->life/255);
            float x = it->circle.getPosition().x - (w/2);
            float y = it->circle.getPosition().y - (h/2);
            ofSetColor(255, it->life);
            mRakugakis[it->ID].draw(x, y, w, h);
        }
        if (bDebugView) {
            ofNoFill();
            ofSetColor(0, 255, 0, (*it).life);
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
            ofPushMatrix();
            float w = mRakugakis[it->ID].width * gui.getValueF(GUI_SLIDER_03);
            float h = mRakugakis[it->ID].height * gui.getValueF(GUI_SLIDER_03);
            float x = it->circle.getPosition().x - (w/2);
            float y = it->circle.getPosition().y - (h/2);
            ofTranslate(x + (w/2), y + (h/2));
            ofRotate(it->circle.getRotation());
            ofSetColor(255);
            mRakugakis[it->ID].draw(-(w/2), -(h/2), w, h);
            ofPopMatrix();
        }
        if (bDebugView) {
            ofNoFill();
            ofSetColor(0, 255, 0);
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

void testApp::drawPachinco()
{
    vector<psCircle>::iterator it = mPsCircles.begin();
    while (it != mPsCircles.end()) {
        //draw rakugaki
        if (mRakugakis.size()) {
            ofPushMatrix();
            float w = (mRakugakis[it->ID].width * gui.getValueF(GUI_SLIDER_03)) * ((float)it->life/255);
            float h = (mRakugakis[it->ID].height * gui.getValueF(GUI_SLIDER_03)) * ((float)it->life/255);
            float x = it->circle.getPosition().x - (w/2);
            float y = it->circle.getPosition().y - (h/2);
            ofTranslate(x + (w/2), y + (h/2));
            ofRotate(it->circle.getRotation());
            ofSetColor(255, it->life);
            mRakugakis[it->ID].draw(-(w/2), -(h/2), w, h);
            ofPopMatrix();
        }
        if (bDebugView) {
            ofNoFill();
            ofSetColor(0, 255, 0);
            (*it).circle.draw();
        }
        //delete
        if ((*it).circle.getPosition().y + (*it).circle.getRadius() >= ofGetHeight()-10) (*it).touch = true;
        if ((*it).touch) {
            (*it).circle.setRadius((*it).circle.getRadius() - 0.1);
            it->life -= 3;
        }
        if ((*it).circle.getRadius() <= 0) {
            it = mPsCircles.erase(it);
        } else {
            it++;
        }
    }
    // draw target image
    if (mRakugakis.size()) {
        float x = ofGetWidth() - 60;
        float y = 60;
        float r = 55;
        ofSetColor(80, 127);
        ofFill();
        ofCircle(x, y, r);
        ofSetColor(255);
        mRakugakis[mTargetID].draw(x-r, y-r, r*2, r*2);
    }
}

void testApp::drawDoremi()
{
    vector<psCircle>::iterator it = mPsCircles.begin();
    while (it != mPsCircles.end()) {
        //draw rakugaki
        if (mRakugakis.size()) {
            ofPushMatrix();
            float w = mRakugakis[it->ID].width * gui.getValueF(GUI_SLIDER_03);
            float h = mRakugakis[it->ID].height * gui.getValueF(GUI_SLIDER_03);
            w *= it->size; h *= it->size;
            float x = it->circle.getPosition().x - (w/2);
            float y = it->circle.getPosition().y - (h/2);
            ofTranslate(x + (w/2), y + (h/2));
            ofRotate(it->circle.getRotation());
            ofSetColor(255, it->life);
            mRakugakis[it->ID].draw(-(w/2), -(h/2), w, h);
            ofPopMatrix();
        }
        if (bDebugView) {
            ofNoFill();
            ofSetColor(0, 255, 0);
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
            
        case 'c':
            clearPolyLines();
            break;
            
        case 'z':
            if (mRakugakis.size())
                mTargetID = (mTargetID - 1) % mRakugakis.size();
            break;
            
        case 'x':
            if (mRakugakis.size())
                mTargetID = (mTargetID + 1) % mRakugakis.size();
            break;
            
        //1~9,0
        case '1': mMode = TEST_MODE; break;
        case '2': mMode = TITLE; break;
        case '3': mMode = RAIN_DROP; setupRain(); break;
        case '4': mMode = SNOW_FALL; setupSnow(); break;
        case '5': mMode = JUMP; setupJump(); break;
        case '6': mMode = PACHINCO; setupPachinco(); break;
        case '7': mMode = DOREMI; setupDoremi(); break;
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
    if (bDebugView) {
        gui.mouseDragged(x, y, button);
    } else {
        mPLines.back().addVertex(x, y);
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (bDebugView) {
        gui.mousePressed(x, y, button);
    } else {
        mPLines.push_back(ofPolyline());
        mPLines.back().addVertex(x, y);
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if (bDebugView) {
        gui.mouseReleased();
        gui.saveSettings();
    } else {
        
        ofxBox2dPolygon poly;
        mPLines.back().simplify();
        
        for (int i=0; i<mPLines.back().size(); i++) {
            poly.addVertex(mPLines.back()[i]);
        }
        
        //poly.setPhysics(1, .2, 1);  // uncomment this to see it fall!
        poly.create(mBox2d.getWorld());
        mPPolyLines.push_back(poly);
    }
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
            
        case PACHINCO:
            setupPachinco();
            break;
            
        case DOREMI:
            setupDoremi();
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
    mPsCircles.clear();
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
        delete px;

        mContour.findContours(mDepthImage, 10, mDepthImage.width * mDepthImage.height, 20, false);
//        mPKinectPolyLines.clear();
//        for (int i = 0; i < mContour.blobs.size(); i++) {
//            ofPolyline poly;
//            for (int j = 0; j < mContour.blobs[i].pts.size(); j++) {
//                poly.addVertex(mContour.blobs[i].pts[j]);
//            }
//            ofxBox2dPolygon psPoly;
//            poly.simplify();
//            for (int k = 0; k < poly.size(); k++) {
//                psPoly.addVertex(poly[k]);
//            }
//            psPoly.create(mBox2d.getWorld());
//            mPKinectPolyLines.push_back(psPoly);
//        }
        
        mKinectPsRect.clear();
        for (int i = 0; i < mContour.blobs.size(); i++) {
//            ofxBox2dCircle c;
//            c.setup(mBox2d.getWorld(),
//                    mContour.blobs[i].centroid.x,
//                    mContour.blobs[i].centroid.y,
//                    min(mContour.blobs[i].boundingRect.width/2, mContour.blobs[i].boundingRect.height/2));
//            mKinectPsCircles.push_back(c);
//            ofxBox2dRect r;
//            r.setup(mBox2d.getWorld(),
//                    mContour.blobs[i].boundingRect.x + (mContour.blobs[i].boundingRect.width/2),
//                    mContour.blobs[i].boundingRect.y + (mContour.blobs[i].boundingRect.height/2),
//                    mContour.blobs[i].boundingRect.width/2,
//                    mContour.blobs[i].boundingRect.height/2);
//            mKinectPsRect.push_back(r);
        }
    }
#endif
    return ;
}

void testApp::clearPolyLines()
{
    mPLines.clear();
    for (int i=0; i<mPPolyLines.size(); i++) {
        mPPolyLines[i].destroy();
    }
}
