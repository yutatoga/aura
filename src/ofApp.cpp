#include "ofApp.h"

static bool removeShapeOffScreen(ofPtr<ofxBox2dBaseShape> shape) {
    if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(shape.get()->getPosition())) {
        return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::setup(){
		//画面の基本設定
		ofSetFullscreen(true);
    ofBackground(0, 0, 0);
    ofSetFrameRate(60);
		
		//カメラから映像を取り込んで表示
		useBlackMagic = false;
		ofVec2f imageSize;
		if (useBlackMagic) {
				imageSize.set(1920, 1080);
				blackMagic.setup(imageSize.x, imageSize.y, 59.94);
		}else{
				vidGrabber.setVerbose(true);
				imageSize.set(320, 240);
				vidGrabber.initGrabber(imageSize.x,imageSize.y);
		}
		
		//使用する画像の領域を確保
		colorImg.allocate(imageSize.x, imageSize.y);
		grayImage.allocate(imageSize.x, imageSize.y);
		grayBg.allocate(imageSize.x, imageSize.y);
		grayDiff.allocate(imageSize.x, imageSize.y);
		grayDiffSmall.allocate(60, 40);
		
		//変数の初期化
    bLearnBakground = true;
    showCvAnalysis = false;
		
		//ベクトル場の初期化
    VF.setupField(60, 40, ofGetWidth(), ofGetHeight());
		
		//GUIの初期設定
    gui.setup();
    gui.config->gridSize.x = 250;
    gui.addContent("grayImage", grayImage);
    gui.addContent("grayDiff", grayDiff);
    gui.addContent("grayDiffSmall", grayDiffSmall);
    gui.addFPSCounter();
    gui.addSlider("threshold", threshold, 0, 400);
    gui.addSlider("gravity", gravity, 0.0, 1.0);
    gui.addSlider("force", force, 0.0, 20.0);
    gui.addSlider("vector threshold", vectorThreshold, 0.0, 2.0);
    gui.addToggle("use live video", liveVideo);
    gui.addToggle("draw video", drawColorImg);
    gui.addToggle("draw vector field", drawVectorFirld);
    gui.addToggle("force inward", bForceInward);
    gui.loadFromXML();
		
		// Box2D初期設定
    box2d.init();
    box2d.setGravity(0, gravity);
		// box2d.createBounds();
		// box2d.createGround();
		float edgeHeight = 0.2;
		ofPolyline polyLineEdge;
		polyLineEdge.addVertex(0, ofGetHeight()*edgeHeight);
		polyLineEdge.addVertex(0, ofGetHeight());
		polyLineEdge.addVertex(ofGetWidth(), ofGetHeight());
		polyLineEdge.addVertex(ofGetWidth(), ofGetHeight()*edgeHeight);
		box2dEdge.addVertexes(polyLineEdge);
		box2dEdge.setPhysics(0.0, 0.5, 0.5);
		box2dEdge.create(box2d.getWorld());
		box2d.setFPS(8);
		box2d.registerGrabbing();
		
		//パーティクル生成
		//    particleNum = 10;
		//    for (int i = 0; i < particleNum; i++) {
		//        CustomCircle* p = new CustomCircle();
		//        p->setPhysics(1.0, 0.0, 0.2);
		//				p->setup(box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), ofRandom(7, 14));
		////        p->disableCollistion();
		//        particles.push_back(p);
		//    }
		
		//		ofURLFileLoader loader;
		//		loader.get("https://yt3.ggpht.com/-3js912icS0E/AAAAAAAAAAI/AAAAAAAAAAA/54i1xPNqEmM/s48-c-k-no/photo.jpg");
		//		loader.saveTo(<#string url#>, <#string path#>);
		//		imageTest.loadImage(loader);
		
		// custom rect
		//		photoNumberRect = 17;
		//		for (int i = 0; i<photoNumberRect; i++) {
		//				ofImage img;
		//				img.loadImage(ofToString(i)+".jpg");
		//				customRectImageVector.push_back(img);
		//		}
		
		// custom circle
		photoNumberCircleLogos = 20;
		for (int i = 0; i<photoNumberCircleLogos; i++) {
				ofImage img;
				img.loadImage("circleLogo/"+ofToString(i)+".png");
				customCircleImageVector.push_back(img);
		}
		
		//special circle
		specialCircleImage.loadImage("special.png");
		specialCircle = ofPtr<CustomCircle>(new CustomCircle);
		specialCircle->setCircleMainColor(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 127));
		specialCircle->setBirthTime(ofGetElapsedTimef());
		specialCircle->setPhysics(0.2, 0.0, 3.9);
		specialCircle->setImage(specialCircleImage);
		specialCircle->setup(box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight(), 0), ofRandom(30, 50));
		
		//初期追加
		//		for (int i=0; i<1000; i++) {
		//				circles.push_back(ofPtr<CustomCircle>(new CustomCircle));
		//				circles.back().get()->setCircleMainColor(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 127));
		//				circles.back().get()->setBirthTime(ofGetElapsedTimef());
		//				circles.back().get()->setPhysics(1.0, 0.0, 3.9);
		//				circles.back().get()->setup(box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight(), 0), ofRandom(3, 20));
		//		}
		
		// 寿命
		circleLifeTime = 45;
		circleLogosLifeTime = 45;
		
		// 上限数
		circleNumberLimit = 5000;
		circleLogoNumberLimit = 20;
}

//--------------------------------------------------------------
void ofApp::update(){
		// 削除
		// rects
		//		int rectLifeTime = 30;
		//		for (int i=0; i<rects.size(); i++) {
		//				if (ofGetElapsedTimef() - rects[i]->getBirthTime() > rectLifeTime) {
		//						rects.erase(rects.begin()+i);
		//				}
		//		}
		
		// circles
		
		//		ofRemove(rects, removeShapeOffScreen); // custom rect
		//		ofRemove(circles, removeShapeOffScreen); // custom circle
		for (int i=0; i<circles.size(); i++) {
				if (ofGetElapsedTimef() - circles[i]->getBirthTime() > circleLifeTime) {
						circles.erase(circles.begin()+i);
				}
				if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(circles[i]->getPosition())) {
						circles.erase(circles.begin()+i);
						
				}
		}
		// circle logos
		for (int i=0; i<circleLogos.size(); i++) {
				if (ofGetElapsedTimef() - circleLogos[i]->getBirthTime() > circleLogosLifeTime) {
						circleLogos.erase(circleLogos.begin()+i);
				}
				if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(circleLogos[i]->getPosition())) {
						circleLogos.erase(circleLogos.begin()+i);
				}
		}
		
		// special circle
		if (!ofRectangle(0, -400, ofGetWidth(), ofGetHeight()+400).inside(specialCircle->getPosition())) {
				specialCircle->setPosition(ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight()/3.0, 0));
		}
		
		box2d.setGravity(0, gravity);
    box2d.update();
		
		//追加
		// rects
		//		if (rects.size() < 30) {
		//				rects.push_back(ofPtr<CustomRect>(new CustomRect));
		//				rects.back().get()->setPhysics(0.2, 0.0, 3.9);
		//				// widthをキーにして、比率を守って、高さを決定する
		//				int photoIdRect = rand()%photoNumberRect;
		//				float logoArea = ofRandom(1000, 10000);
		//
		//				float w = sqrt(logoArea*customRectImageVector[photoIdRect].getWidth()/customRectImageVector[photoIdRect].getHeight());
		//				float h = logoArea/w;
		////				float h = w*customRectImageVector[photoId].getHeight()/customRectImageVector[photoId].getWidth();
		//				rects.back().get()->setup(box2d.getWorld(), ofGetWidth()/2.+ofRandom(-ofGetWidth()/2.*0.5, ofGetWidth()/2.*0.5), ofRandom(-ofGetHeight()/3.0, -customRectImageVector[photoIdRect].getHeight()), w, h);
		//				rects.back().get()->setImage(customRectImageVector[photoIdRect]);
		//				rects.back().get()->setBirthTime(ofGetElapsedTimef());
		//		}
		// circles
		if (circles.size() < circleNumberLimit) {
				circles.push_back(ofPtr<CustomCircle>(new CustomCircle));
				circles.back().get()->setCircleMainColor(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 127));
				circles.back().get()->setBirthTime(ofGetElapsedTimef());
				//ADJUSTME: 画像ありとなしの割合を調整
				float radius = ofRandom(3, 20);
				circles.back().get()->setPhysics(1.0, 0.0, 3.9);
				circles.back().get()->setup(box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight()/3.0, -radius), radius);
		}
		// circle logos
		if (circleLogos.size() < circleLogoNumberLimit) {
				circleLogos.push_back(ofPtr<CustomCircle>(new CustomCircle));
				circleLogos.back().get()->setCircleMainColor(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 127));
				circleLogos.back().get()->setBirthTime(ofGetElapsedTimef());
				float radius = ofRandom(30, 60);
				int photoId = rand()%photoNumberCircleLogos;
				circleLogos.back().get()->setImage(customCircleImageVector[photoId]);
				circleLogos.back().get()->setPhysics(0.2, 0.0, 3.9);
				circleLogos.back().get()->setup(box2d.getWorld(), ofRandom(0, ofGetWidth()), ofRandom(-ofGetHeight()/3.0, -radius), radius);
		}
		
		// 色を変える
		specialCircle->setCircleMainColor(ofColor(ofRandom(0, 255), ofRandom(0, 255), ofRandom(0, 255), 127));
		
		//camera
		if (useBlackMagic) {
				if(blackMagic.update()) {
						timer.tick();
				}
		}else{
				vidGrabber.update();
		}
		
		if(useBlackMagic || vidGrabber.isFrameNew()){
				//フレームが切り替わった際のみ画像を解析
				//取り込んだフレームを画像としてキャプチャ
				if (useBlackMagic) {
						colorImg.setFromPixels(blackMagic.getColorPixels());
				}else{
						colorImg.setFromPixels(vidGrabber.getPixels(), 320,240);
				}
				//ADJUSTME: 上下・左右反転()
				colorImg.mirror(false, true);
				//カラーのイメージをグレースケールに変換
				grayImage = colorImg;
				//まだ背景画像が記録されていなければ、現在のフレームを背景画像とする
				if (bLearnBakground == true){
						grayBg = grayImage;
						bLearnBakground = false;
				}
				//グレースケールのイメージと取り込んだ背景画像との差分を算出
				grayDiff.absDiff(grayBg, grayImage);
				//画像を2値化(白と黒だけに)する
				grayDiff.threshold(threshold);
				//縮小した差分イメージに画像をコピー
				grayDiffSmall.scaleIntoMe(grayDiff);
				//ぼかし
				grayDiffSmall.blur(5);
				//ベクトル場に差分イメージを適用
				VF.setFromPixels(grayDiffSmall.getPixels(), bForceInward, 0.05f);
				
				//ベクトル場に発生した力を計算し、パーティクルにかかる力を算出
				//particles
				//        for(list <CustomCircle *>::iterator it = particles.begin(); it != particles.end(); ++it){
				//            ofVec2f frc;
				//            frc = VF.getForceFromPos((*it)->getPosition().x, (*it)->getPosition().y);
				//            //設定した閾値を越えたら、VFの力を加える
				//            if (frc.length() > vectorThreshold) {
				//                (*it)->addForce(ofPoint(frc.x * force, frc.y * force), 1.0);
				//            }
				//            (*it)->update();
				//        }
				
				//同様にcustom rectに対しても
				//				for (int i = 0; i<rects.size(); i++) {
				//						ofVec2f frc;
				//						frc = VF.getForceFromPos(rects[i]->getPosition().x, rects[i]->getPosition().y);
				//						//設定した閾値を越えたら、VFの力を加える
				//            if (frc.length() > vectorThreshold) {
				//                rects[i]->addForce(ofPoint(frc.x * force, frc.y * force), 1.0);
				//            }
				//						rects[i]->update();
				//				}
				//同様にcustom circlesに対しても
				for (int i = 0; i<circles.size(); i++){
						ofVec2f frc;
						frc = VF.getForceFromPos(circles[i]->getPosition().x, circles[i]->getPosition().y);
						//設定した閾値を越えたら、VFの力を加える
						if (frc.length() > vectorThreshold) {
								circles[i]->addForce(ofPoint(frc.x * force, frc.y * force), 1.0);
						}
						circles[i]->update();
				}
				//同様にcustom circles logoに対しても
				for (int i = 0; i<circleLogos.size(); i++){
						ofVec2f frc;
						frc = VF.getForceFromPos(circleLogos[i]->getPosition().x, circleLogos[i]->getPosition().y);
						//設定した閾値を越えたら、VFの力を加える
						if (frc.length() > vectorThreshold) {
								circleLogos[i]->addForce(ofPoint(frc.x * force, frc.y * force), 1.0);
						}
						circleLogos[i]->update();
				}
				
				//同様にspecial circles logoに対しても
				ofVec2f frc;
				frc = VF.getForceFromPos(specialCircle->getPosition().x, specialCircle->getPosition().y);
				//設定した閾値を越えたら、VFの力を加える
				if (frc.length() > vectorThreshold) {
						specialCircle->addForce(ofPoint(frc.x * force, frc.y * force), 1.0);
				}
				specialCircle->update();
		}
}

//--------------------------------------------------------------
void ofApp::draw(){
		glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		
    if (drawColorImg) {
				//カラーイメージを描く
        ofSetColor(255, 255, 255, 127);
        colorImg.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
		
    if (drawVectorFirld) {
				//ベクトル場を描く
        ofSetColor(255, 255, 255, 127);
        VF.draw();
    }
		
		//パーティクルを描く
		//    for(list <CustomCircle *>::iterator it = particles.begin(); it != particles.end(); ++it){
		//        (*it)->draw();
		//    }
		
		//GUIを描く
    gui.draw();
		
		//customRect
		//draw myImage
		//		ofLog(OF_LOG_NOTICE, ofToString(rects.size()));
		//		for(int i=0; i<rects.size(); i++){
		//				rects[i]->draw();
		//		}
		
		//customCircle
		for (int i=0; i<circles.size(); i++) {
				circles[i]->draw();
		}
		
		//customCircleLogos
		for (int i=0; i<circleLogos.size(); i++) {
				circleLogos[i]->draw();
		}
		
		//special circle
		specialCircle->draw();
		
		//camera
		//		blackMagic.drawColor();
		ofDrawBitmapStringHighlight(ofToString((int) timer.getFramerate()), 10, 20);
		//		ofDrawBitmapStringHighlight(ofToString((int) rects.size()), 40, 20);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
		//キー入力でモード切り替え
    switch (key){
        case 'f':
				{
						//						particles.clear();
						//						rects.clear();
						circles.clear();
						//フルスクリーンのon/off
            ofToggleFullscreen();
						VF.setupField(60, 40, ofGetWidth(), ofGetHeight());
						//						box2d.createBounds();
						box2dEdge.clear();
						float edgeHeight = 0.2;
						ofPolyline polyLineEdge;
						polyLineEdge.addVertex(0, ofGetHeight()*edgeHeight);
						polyLineEdge.addVertex(0, ofGetHeight());
						polyLineEdge.addVertex(ofGetWidth(), ofGetHeight());
						polyLineEdge.addVertex(ofGetWidth(), ofGetHeight()*edgeHeight);
						box2dEdge.addVertexes(polyLineEdge);
						box2dEdge.setPhysics(0.0, 0.5, 0.5);
						box2dEdge.create(box2d.getWorld());
						box2d.update();
						break;
				}
        case 'g':
						//Gui表示のon/off
            gui.toggleDraw();
            break;
        case 't':
						//重力を反転
            bForceInward = !bForceInward;
            break;
						
        case ' ':
						//背景画像を新規に取り込む
            bLearnBakground = true;
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
		
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
		
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
		
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
		
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
		
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
		
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
		
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
		
}
