#include "CustomCircle.h"

//ofxBox2dCircleを継承した、オリジナルの円を描く
void CustomCircle::draw()
{
    float radius = getRadius(); //半径を取得
    glPushMatrix(); //座標を変更
    glTranslatef(getPosition().x, getPosition().y, 0); //物体の位置に座標を移動
    //円を描く
//    ofFill();
//    ofSetColor(127, 255, 255, 11);
//    ofCircle(0, 0, radius*2.0);
//    ofSetColor(31, 127, 255, 127);
//    ofCircle(0, 0, radius);
//    ofSetColor(255, 255, 255, 63);
//    ofCircle(0, 0, radius*0.5);

    ofFill();
		ofColor color2;
		color2.setHsb(color.getHue(), color.getSaturation(), color.getBrightness()*1.5);
    ofSetColor(color2.r, color2.g, color2.b, 11);
    ofCircle(0, 0, radius*2.0);
    ofSetColor(color);
    ofCircle(0, 0, radius);
    ofSetColor(255, 255, 255, 63);
    ofCircle(0, 0, radius*0.5);    
		glPopMatrix(); //座標を元に戻す
}

void CustomCircle::setCircleMainColor(ofColor col)
{
		color = col;
}

void CustomCircle::setBirthTime(float elapsedTime)
{
		birthTime = elapsedTime;
}

float CustomCircle::getBirthTime(){
		return birthTime;
}