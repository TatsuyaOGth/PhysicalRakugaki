#pragma once

#include "ofMain.h"
#include "ofxOgsn.h"
#include "ofxBox2d.h"
#include "ofxTrueTypeFontUC.h"
#include "ofxXmlSettings.h"
#include "ofxControlPanel.h"
#include "ofxKinect.h"

//#define ENABLE_KINECT


//Control Panel Propaty
static const string GUI_SLIDER_01 = "slider_01";
static const string GUI_SLIDER_02 = "slider_02";
static const string GUI_SLIDER_03 = "slider_03";

/**
 MODE
 */
enum mode {
    TEST_MODE = 0,
    TITLE,
    RAIN_DROP,
    SNOW_FALL,
    JUMP,
    PACHINCO
};

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    void exit();
    
    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
        
    //=== Each Update ===
    void setupRain();
    void setupSnow();
    void setupJump();
    void setupPachinco();
    
    //=== Each Update ===
    void updateTitle();
    void updateRain();
    void updateSnow();
    void updateJump();
    void updatePachinco();
    
    //=== Each Draw ===
    void drawTitle();
    void drawRain();
    void drawSnow();
    void drawJump();
    void drawPachinco();
    
private:
    
    void getAndSetRakugaki(const string& path);
    string getGuiFileName();
    void getKinectContoursPts();
    void clearPolyLines();
    
    ofxBox2d mBox2d;
    ofxControlPanel gui;
    ofxXmlSettings xml;
    ofxKinect kinect;
    ofxOgsn og;
    
    int mKinectAngle;
    int mClipDepth;
    
    typedef struct _psCircle {
        unsigned int ID = 0;
        bool touch = false;
        int life = 255;
        ofxBox2dCircle circle;
    } psCircle;
    vector<psCircle> mPsCircles;
    
    ofImage mDepthImage;
    vector<ofPoint> mContPts;
    vector<ofPolyline> mPLines;
	vector<ofxBox2dPolygon>	mPPolyLines;
    
    //image
    vector<ofImage> mRakugakis;
    ofDirectory mDir;
    
    //view mode
    bool bDebugView;
    mode mMode;
    bitset<9> bFunc;
    
    // TITLE
    ofxTrueTypeFontUC mFont;
    
    //target mode
    int mTargetID;

};
