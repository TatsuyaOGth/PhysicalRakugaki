#pragma once

#include "ofMain.h"
#include "ofxOgsn.h"
#include "ofxBox2d.h"
#include "ofxTrueTypeFontUC.h"

class testApp : public ofBaseApp{
public:
    void setup();
    void update();
    void draw();
    
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
    
    //=== Each Update ===
    void updateTitle();
    void updateRain();
    void updateSnow();
    
    //=== Each Draw ===
    void drawTitle();
    void drawRain();
    void drawSnow();
    
private:
    
    //Box2D
    ofxBox2d mBox2d;
    
    typedef struct _psCircle {
        unsigned int ID = 0;
        bool touch = false;
        int life = 255;
        ofxBox2dCircle circle;
        int minusLifeTime(){ life--; return life; }
    } psCircle;
    vector<psCircle> mPsCircles;
    
    vector<ofPolyline> mPLines;
	vector<ofxBox2dPolygon>	mPPolyLines;
    
    //image
    vector<ofImage> mRakugakis;
    
    //view mode
    enum mode {
        TEST_MODE = 0,
        TITLE,
        RAIN_DROP,
        SNOW_FALL
    };
    bool bDebugView;
    mode mMode;
    bitset<9> bFunc;
    
    // TITLE
    ofxTrueTypeFontUC mFont;

};
