#include "ofApp.h"

#define DEFAULT_X 640
#define DEFAULT_Y 480

//--------------------------------------------------------------
void ofApp::setup(){
    //window設定
    ofBackground(0,0,0); //背景色
    ofSetWindowShape(DEFAULT_X, DEFAULT_Y);
    ofSetFrameRate(30); //fps30なら640×480,15なら1024×768
    
    //set ofxOpenNI
    kinect.setup();
    kinect.setRegister(true);
    kinect.setMirror(true);     //反転
    kinect.addDepthGenerator(); //深度センサー
    kinect.addImageGenerator(); //カメラ
    kinect.addUserGenerator();  //人認識
    kinect.setMaxNumUsers(2);   //認識できる最大人数
    
    //depth画像のズレ補正
    kinect.getDepthGenerator().GetAlternativeViewPointCap().SetViewPoint(kinect.getImageGenerator());
    
    hide = true;
    
    //得点設定
    font.load("ヒラギノ角ゴシック W9.ttc", 32);
    point1 = 0;
    point2 = 0;
    timer = 60.0;
    battle = false;
    result = false;
    
    //サウンド設定
    add.load("add_score.mp3");
    more_add.load("cancel3.mp3");
    start.load("game_start.mp3");
    end.load("time_is_up.mp3");
    
    //GUI設定
    /*
     gui.setup();
     gui.add(scaleSlider1.setup("R", 0, 1, 255));
     gui.add(scaleSlider2.setup("G", 0, 1, 255));
     gui.add(scaleSlider3.setup("B", 0, 1, 255));
     */
    
    //画面サイズ設定
    scale = 1.0;
    offsetX = offsetY = 0.0;
    
    //start kinect
    kinect.start();
}

//--------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    //プレイヤーの頭の座標(headpos1, headpos2)を取得
    if (kinect.getNumTrackedUsers() == 1){
        headpos1 = kinect.getTrackedUser(0).getJoint(Joint::JOINT_HEAD).getProjectivePosition();
    }
    else if(kinect.getNumTrackedUsers() == 2){
        headpos1 = kinect.getTrackedUser(0).getJoint(Joint::JOINT_HEAD).getProjectivePosition();
        headpos2 = kinect.getTrackedUser(1).getJoint(Joint::JOINT_HEAD).getProjectivePosition();
    }
    
    //文字の幅を取得
    cw1 = font.stringWidth(ofToString(point1))/2;
    if(point1 >= 10)
        cw1 *= 1.5;
    else if(point1 >= 100)
        cw1 *= 2.0;
    
    cw2 = font.stringWidth(ofToString(point2))/2;
    if(point2 >= 10)
        cw2 *= 1.5;
    else if(point2 >= 100)
        cw2 *= 2.0;
    
    cwwin = font.stringWidth("WIN!")/2;
    cwlose = font.stringWidth("LOSE...")/2;
    //文字の高さを取得
    ch = font.stringHeight("0")/2;
    
    if(battle && timer > 0 && !result)
        timer -= 1.0/10.0;
    
    if(battle && timer <= 0){
        end.play();
        battle = !battle;
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofScale(scale, scale);
    fontscale1 = ofMap(headpos1.z, 500, 4500, 2, 0.5);
    fontscale2 = ofMap(headpos2.z, 500, 4500, 2, 0.5);
    
    if(result){
        //プレイヤー1が勝利の場合
        if(point1 > point2){
            /*結果発表の音*/
            //勝者表示
            ofPushMatrix();
            ofScale(fontscale1, fontscale1);
            ofSetColor(255, 0, 0);
            font.drawString("WIN!!", (headpos1.x-cwwin)/fontscale1, (headpos1.y-ch)/fontscale1);
            ofPopMatrix();
            
            //敗者表示
            ofPushMatrix();
            ofScale(fontscale2, fontscale2);
            ofSetColor(0, 0, 255);
            font.drawString("LOSE...", (headpos2.x-cwlose)/fontscale2, (headpos2.y-ch)/fontscale2);
            ofPopMatrix();
        }
        
        //プレイヤー2が勝利の場合
        else if(point1 < point2){
            /*結果発表の音*/
            //勝者表示
            ofPushMatrix();
            ofScale(fontscale2, fontscale2);
            ofSetColor(255, 0, 0);
            font.drawString("WIN!!", (headpos2.x-cwwin)/fontscale2, (headpos2.y-ch)/fontscale2);
            ofPopMatrix();
            
            //敗者表示
            ofPushMatrix();
            ofScale(fontscale1, fontscale1);
            ofSetColor(0, 0, 255);
            font.drawString("LOSE...", (headpos1.x-cwlose)/fontscale1, (headpos1.y-ch)/fontscale1);
            ofPopMatrix();
        }
        
        //引き分けの場合
        else{
            /*引き分けの音*/
            ofSetColor(255, 255, 255);
            font.drawString("DRAW!", ofGetWidth()/2, 100);
        }
    }
    else{
        //カラー画像を描画
        if(hide){
            ofSetColor(255, 255, 255);
            kinect.drawImage(offsetX, 0, ofGetWidth(), ofGetHeight());
        }
        
        //スケルトンを描画
        ofSetColor(255, 255, 255);
        kinect.drawSkeletons(offsetX, 0, ofGetWidth(), ofGetHeight());
        
        if(kinect.getNumTrackedUsers() == 1){ //1人だけが認識されているとき
            //1人目の得点表示
            ofPushMatrix();
            ofSetColor(255, 255, 255); //文字色
            ofScale(fontscale1*scale, fontscale1*scale);
            font.drawString(ofToString(point1), (headpos1.x-cw1)/fontscale1, (headpos1.y-ch)/fontscale1);
            ofPopMatrix();
        }
        else if(kinect.getNumTrackedUsers() == 2){ //2人共認識されているとき
            //1人目の得点表示
            ofPushMatrix();
            ofSetColor(255, 255, 255); //文字色
            ofScale(fontscale1*scale, fontscale1*scale);
            font.drawString(ofToString(point1), (headpos1.x-cw1)/fontscale1, (headpos1.y-ch)/fontscale1);
            ofPopMatrix();
            
            //2人目の得点表示
            ofPushMatrix();
            ofSetColor(255,255,255); //文字色
            ofScale(fontscale2*scale, fontscale2*scale);
            font.drawString(ofToString(point2), (headpos2.x-cw2)/fontscale2, (headpos2.y-ch)/fontscale2);
            ofPopMatrix();
        }
        
        //gui.draw();
        
        //残り時間が1桁になるまでは白、1桁で黄、0で赤
        if(timer > 10){
            ofSetColor(255, 255, 255);
            font.drawString(ofToString(int(timer)), ofGetWidth()/2, 100);
        }
        else if(timer <= 10 && timer >= 0){
            ofSetColor(255, 255, 0);
            font.drawString(ofToString(int(timer)), ofGetWidth()/2, 100);
        }
        else{
            ofSetColor(255, 0, 0);
            font.drawString("0", ofGetWidth()/2, 100);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case('s'):
            battle = true;
            start.play();
            break;
        case('q'):
            kinect.stop();
            break;
        case('h'):
            hide = !hide;
            break;
        case('r'):
            point1 = point2 = 0;
            timer = 60.0;
            battle = false;
            result = false;
            //リセットの時に音を入れたい
            break;
        case('f'):
            ofToggleFullscreen();
            break;
        case(' '):
            int tem;
            tem = point1;
            point1 = point2;
            point2 = tem;
            break;
        case(OF_KEY_RETURN):
            result = true;
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
    if(x > 0 && x < ofGetWidth()/2 && y > 0 && y < ofGetHeight()/2){
        point1 += 20;
        more_add.play();
    }
    else if(x > 0 && x < ofGetWidth()/2 && y >= ofGetHeight()/2 && y < ofGetHeight()){
        point1 += 10;
        add.play();
    }
    else if(x >= ofGetWidth()/2 && x < ofGetWidth() && y > 0 && y < ofGetHeight()/2){
        point2 += 20;
        more_add.play();
    }
    else if(x >= ofGetWidth()/2 && x < ofGetWidth() && y > ofGetHeight()/2 && y < ofGetHeight()){
        point2 += 10;
        add.play();
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //scale = MIN(DEFAULT_X/ofGetWidth(), DEFAULT_Y/ofGetHeight());
    //offsetX = (ofGetWidth()-DEFAULT_X*scale)/2;
    //offsetY = (ofGetHeight()-480*scale)/2;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
