#pragma once

#include "ofMain.h"
#include "vectorField.h"
#include "ofxOpenCv.h"
#include "ofxSimpleGuiToo.h"
#include "ofxBox2d.h"
#include "CustomCircle.h"
#include "CustomRect.h"
#include "ofxBlackMagic.h"

class RateTimer {
protected:
		float lastTick, averagePeriod, smoothing;
		bool secondTick;
public:
		RateTimer() :
		smoothing(.9) {
				reset();
		}
		void reset() {
				lastTick = 0, averagePeriod = 0, secondTick = false;
		}
		void setSmoothing(float smoothing) {
				this->smoothing = smoothing;
		}
		float getFramerate() {
				return averagePeriod == 0 ? 0 : 1 / averagePeriod;
		}
		void tick() {
				float curTick = ofGetElapsedTimef();
				if(lastTick == 0) {
						secondTick = true;
				} else {
						float curDiff = curTick - lastTick;;
						if(secondTick) {
								averagePeriod = curDiff;
								secondTick = false;
						} else {
								averagePeriod = ofLerp(curDiff, averagePeriod, smoothing);
						}
				}
				lastTick = curTick;
		}
};


class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		//カメラ入力を準備
    ofVideoGrabber vidGrabber;
		
    ofxCvColorImage colorImg; //オリジナルのカラー映像
    ofxCvGrayscaleImage grayImage; //グレースケールに変換後
    ofxCvGrayscaleImage grayBg; //キャプチャーした背景画像
    ofxCvGrayscaleImage grayDiff; //現在の画像と背景との差分
    ofxCvGrayscaleImage grayDiffSmall; //縮小した差分イメージ
    ofxCvContourFinder contourFinder; //輪郭抽出のためのクラス
		
    bool bLearnBakground; //背景画像を登録したか否か
    bool showCvAnalysis; //解析結果を表示するか
    int threshold; //2値化の際の敷居値
    bool liveVideo; //カメラ入力を使用するか
    bool drawColorImg; //ビデオ表示のon/off
    bool drawVectorFirld; //VectorField表示のon/off
    bool bForceInward; //重力の向き
		
		//GUI
    ofxSimpleGuiToo gui;
		
    //ベクトル場
    vectorField VF;
		
    //box2d
    ofxBox2d box2d;
    float gravity; //重力
    float force; //押し出す力
    float vectorThreshold; //力を適用する閾値
		ofxBox2dEdge box2dEdge;

		
    //ofxBox2dCircleを継承したクラス
//    list <CustomCircle *> particles;
//    int particleNum;
		
//		vector <ofPtr<CustomRect> > rects;
		vector <ofPtr<CustomCircle> > circles;
		vector <ofPtr<CustomCircle> > circleLogos;
		ofPtr<CustomCircle> specialCircle;

		
//		vector<ofImage> customRectImageVector;
		vector<ofImage> customCircleImageVector;
		ofImage specialCircleImage;
		
		// camera
		bool useBlackMagic;
		ofxBlackMagic blackMagic;
		RateTimer timer;
		
//		int photoNumberRect;
		int photoNumberCircleLogos;
	  int circleNumberLimit;
		int circleLogoNumberLimit;
		
		int circleLifeTime;
		int circleLogosLifeTime;
//		int rectNumberLimit;
		
};
