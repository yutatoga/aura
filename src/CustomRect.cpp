//
//  CustomRect.cpp
//  Aura
//
//  Created by SystemTOGA on 5/1/14.
//
//

#include "CustomRect.h"

CustomRect::CustomRect() {
}

void CustomRect::draw() {
		ofPushStyle();
		ofPushMatrix();
		ofSetRectMode(OF_RECTMODE_CENTER);
		ofTranslate(getPosition().x, getPosition().y);
		ofRotate(getRotation());
		ofSetColor(255, 255, 255);
		img.draw(0, 0, getWidth(), getHeight());
		ofPopMatrix();
		ofPopStyle();
}

void CustomRect::setImage(ofImage image){
		img = image;
}

ofImage CustomRect::getImage(){
		return img;
}

void CustomRect::setBirthTime(float elapsedTime){
		birthTime = elapsedTime;
}

float CustomRect::getBirthTime(){
		return birthTime;
}