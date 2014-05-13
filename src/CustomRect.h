//
//  CustomRect.h
//  Aura
//
//  Created by SystemTOGA on 5/1/14.
//
//
//#include "ofxVectorMath.h"
#include "ofxBox2d.h"

class CustomRect : public ofxBox2dRect {
		
public:
		CustomRect();
		void draw();
		
		void setImage(ofImage img);
		ofImage getImage();
		ofImage img;
		
		void setBirthTime(float elapsedTime);
		float getBirthTime();
		float birthTime;
};