//#include "ofxVectorMath.h"
#include "ofxBox2d.h"

//ofxBox2dCircleを継承したクラスCustomCircleを定義
class CustomCircle : public ofxBox2dCircle {
public:
    void draw(); //円を描画する
		void setCircleMainColor(ofColor col);
		void setBirthTime(float elapsedTime);
		float getBirthTime();
		float birthTime;
		ofColor color;
};