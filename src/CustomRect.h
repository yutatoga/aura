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
		
private:
		ofImage img;
		
public:
		CustomRect();
		void draw();
		void setImage(ofImage img);
		ofImage getImage();
};