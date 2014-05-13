//#include "ofxVectorMath.h"
#include "ofxBox2d.h"

//ofxBox2dCircleを継承したクラスCustomCircleを定義
class CustomCircle : public ofxBox2dCircle {
public:
    void draw(); //円を描画する
		void setCircleMainColor(ofColor col);
		ofColor color;
		
		void setImage(ofImage img);
		ofImage getImage();
		ofImage image;
		
		void setBirthTime(float elapsedTime);
		float getBirthTime();
		float birthTime;
		
		//あたり
		void setSpecial(bool isSpecial);
		bool getSpecial();
		bool special;
};