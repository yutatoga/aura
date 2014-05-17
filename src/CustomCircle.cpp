#include "CustomCircle.h"

//ofxBox2dCircleを継承した、オリジナルの円を描く
void CustomCircle::draw()
{
    float radius = getRadius(); //半径を取得
    glPushMatrix(); //座標を変更
    glTranslatef(getPosition().x, getPosition().y, 0); //物体の位置に座標を移動
		ofRotate(getRotation(), 0, 0, 1); //回転
    //円を描く
//    ofFill();
//    ofSetColor(127, 255, 255, 11);
//    ofCircle(0, 0, radius*2.0);
//    ofSetColor(31, 127, 255, 127);
//    ofCircle(0, 0, radius);
//    ofSetColor(255, 255, 255, 63);
//    ofCircle(0, 0, radius*0.5);
		
		// 背景の円
		ofPushStyle();
		ofFill();
		ofColor color2;
		color2.setHsb(color.getHue(), color.getSaturation(), color.getBrightness()*1.5);
		ofSetColor(color2.r, color2.g, color2.b, 11);
		ofCircle(0, 0, radius*2.0);
		ofSetColor(color);
		ofCircle(0, 0, radius);
		ofSetColor(255, 255, 255, 63);
		ofCircle(0, 0, radius*0.5);
		ofPopStyle();
		
		//画像
		if(image.isAllocated()){
				ofPushStyle();
				ofSetColor(255, 255, 255, 255);
				image.draw(-getRadius(), -getRadius(), 2*getRadius(), 2*getRadius());
				ofPopStyle();
		}
		glPopMatrix(); //座標を元に戻す
}

void CustomCircle::setCircleMainColor(ofColor col){
		color = col;
}

void CustomCircle::setBirthTime(float elapsedTime){
		birthTime = elapsedTime;
}

float CustomCircle::getBirthTime(){
		return birthTime;
}

// 画像が、真円であり、画像サイズが半径x半径であることを想定
void CustomCircle::setImage(ofImage img){
		image = img;
}

ofImage CustomCircle::getImage(){
		return image;
}
