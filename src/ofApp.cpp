#include "ofApp.h"

//-----------------------------------------------------------------------------------------------------------
void ofApp::setup(){
    //window設定
    ofBackground(0,0,0); //背景色(黒)
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
    
    //サウンド設定
    add.load("add_score.mp3");
    more_add.load("cancel3.mp3");
    start.load("game_start.mp3");
    end.load("time_is_up.mp3");
    
    //フォント設定
    font.load("mplus-1p-bold.ttf", 32); //文字フォント
    figfont.load("mplus-1p-bold.ttf", 100); //数字フォント
    
    //文字の幅を取得
    cwwin = font.stringWidth("WIN!!")/2;
    cwlose = font.stringWidth("LOSE")/2;
    cwdraw = font.stringWidth("DRAW!")/2;
    cwt = figfont.stringWidth("00")/2;
    
    //文字の高さを取得(得点)
    ch = figfont.stringHeight("0")/2;
    
    //ゲーム設定
    point1 = point2 = 0;
    timer = 60.0;
    battle = result = replace = false; //バトル中か?, 結果発表中か?, スコア入れ替え中か?
    
    //指定したポートで接続(OSC)
    receiver.setup(PORT);
    osccount = 0;
    
    //画面サイズ設定
    //scale = 1.0;
    //offsetX = offsetY = 0.0;
    fontscale_x = 1.6;
    fontscale_y = 1.6;
    
    //kinect起動
    kinect.start();
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::update(){
    kinect.update();
    //プレイヤーの頭の座標(headpos1, headpos2)を取得
    //1人だけ認識されているとき
    if (kinect.getNumTrackedUsers() == 1)
        headpos1 = kinect.getTrackedUser(0).getJoint(Joint::JOINT_HEAD).getProjectivePosition();
    //2人共認識されているとき
    else if(kinect.getNumTrackedUsers() == 2){
        headpos1 = kinect.getTrackedUser(0).getJoint(Joint::JOINT_HEAD).getProjectivePosition();
        headpos2 = kinect.getTrackedUser(1).getJoint(Joint::JOINT_HEAD).getProjectivePosition();
    }
    
    //文字の幅を取得(得点)
    cw1 = figfont.stringWidth(ofToString(point1))*0.5;
    if(point1 >= 10)
        cw1 *= 1.5;
    else if(point1 >= 100)
        cw1 *= 2.0;
    
    cw2 = figfont.stringWidth(ofToString(point1))*0.5;
    if(point2 >= 10)
        cw2 *= 1.5;
    else if(point2 >= 100)
        cw2 *= 2.0;
    
    if(battle && timer >= 0)
        timer -= 1.0/3.0;
    
    if(battle && timer <= 0){
        end.play();
        battle = false;
    }
    
    osccount++;
    
    //OSCメッセージを受信
    while(receiver.hasWaitingMessages() && osccount % 15 == 1){
        receiver.getNextMessage(&m);
        
        if(m.getAddress() == "/button/player1moreplus"){
            point1 += 20;
            more_add.play();
            m.clear();
        }
        else if(m.getAddress() == "/button/player2moreplus"){
            point2 += 20;
            more_add.play();
            m.clear();
        }
        else if(m.getAddress() == "/button/player1plus"){
            point1 += 10;
            add.play();
            m.clear();
        }
        else if(m.getAddress() == "/button/player2plus"){
            point2 += 10;
            add.play();
            m.clear();
        }
        else if(m.getAddress() == "/button/player1minus"){
            point1 -= 10;
            m.clear();
        }
        else if(m.getAddress() == "/button/player2minus"){
            point2 -= 10;
            m.clear();
        }
        else if(m.getAddress() == "/button/replace"){
            int tem;
            tem = point1;
            point1 = point2;
            point2 = tem;
            replace = !replace;
            m.clear();
        }
        else if(m.getAddress() == "/button/start"){
            battle = true;
            start.play();
            m.clear();
        }
        else if(m.getAddress() == "/button/result"){
            result = true;
            m.clear();
        }
        else if(m.getAddress() == "/button/reset"){
            point1 = point2 = 0;
            timer = 60.0;
            battle = result = replace = false;
            m.clear();
        }
        else if(m.getAddress() == "/button/fullscreen"){
            ofToggleFullscreen();
            m.clear();
        }
        osccount = 0;
    }
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::draw(){
    //ofScale(scale, scale);
    //fontscale1 = ofMap(headpos1.z, 500, 4500, 2, 0.5);
    //fontscale2 = ofMap(headpos2.z, 500, 4500, 2, 0.5);
    
    if(result){
        //プレイヤー1が勝利の場合
        if((!replace && point1 > point2) || (replace && point1 < point2)){
            /*結果発表の音*/
            //勝者表示
            //ofPushMatrix();
            //ofScale(fontscale1, fontscale1);
            ofSetColor(255,0,0);
            font.drawString("WIN!!", (headpos1.x-cwwin)*fontscale_x, (headpos1.y-ch)*fontscale_y);
            //ofPopMatrix();
            
            //敗者表示
            //ofPushMatrix();
            //ofScale(fontscale2, fontscale2);
            ofSetColor(0,0,255);
            font.drawString("LOSE", (headpos2.x-cwlose)*fontscale_x, (headpos2.y-ch)*fontscale_y);
            //ofPopMatrix();
        }
        
        //プレイヤー2が勝利の場合
        else if((!replace && point1 < point2) || (replace && point1 > point2)){
            /*結果発表の音*/
            //勝者表示
            //ofPushMatrix();
            //ofScale(fontscale2, fontscale2);
            ofSetColor(255,0,0);
            font.drawString("WIN!!", (headpos2.x-cwwin)*fontscale_x, (headpos2.y-ch)*fontscale_y);
            //ofPopMatrix();
            
            //敗者表示
            //ofPushMatrix();
            //ofScale(fontscale1, fontscale1);
            ofSetColor(0,0,255);
            font.drawString("LOSE", (headpos1.x-cwlose)*fontscale_x, (headpos1.y-ch)*fontscale_y);
            //ofPopMatrix();
        }
        
        //引き分けの場合
        else{
            /*引き分けの音*/
            ofSetColor(255,255,255);
            font.drawString("DRAW!", ofGetWidth()/2-cwdraw*2, 100);
        }
    }
    else{
        //カラー画像を描画
        ofSetColor(255,255,255);
        kinect.drawImage(offsetX, 0, ofGetWidth(), ofGetHeight());
        
        //スケルトンを描画
        /*
        ofSetColor(255,255,255);
        kinect.drawSkeletons(offsetX, 0, ofGetWidth(), ofGetHeight());
        */
        
        //1人だけが認識されているとき
        if(kinect.getNumTrackedUsers() == 1){
            //1人目の得点表示
            //ofPushMatrix();
            ofSetColor(255,255,255); //文字色
            //ofScale(fontscale1, fontscale1);
            figfont.drawString(ofToString(point1), (headpos1.x-cw1)*fontscale_x, (headpos1.y-ch)*fontscale_y);
            //ofPopMatrix();
        }
        //2人共認識されているとき
        else if(kinect.getNumTrackedUsers() == 2){
            //1人目の得点表示
            //ofPushMatrix();
            ofSetColor(255,255,255); //文字色
            //ofScale(fontscale1, fontscale1);
            figfont.drawString(ofToString(point1), (headpos1.x-cw1)*fontscale_x, (headpos1.y-ch)*fontscale_y);
            //ofPopMatrix();
            
            //2人目の得点表示
            //ofPushMatrix();
            ofSetColor(255,255,255); //文字色
            //ofScale(fontscale2, fontscale2);
            figfont.drawString(ofToString(point2), (headpos2.x-cw2)*fontscale_x, (headpos2.y-ch)*fontscale_y);
            //ofPopMatrix();
        }
        
        //残り時間が1桁になるまでは白、1桁で黄、0で赤
        if(timer > 10){
            ofSetColor(255,255,255);
            figfont.drawString(ofToString(int(timer)), ofGetWidth()/2-cwt, 150);
        }
        else if(timer <= 10 && timer >= 0){
            ofSetColor(255,255,0);
            figfont.drawString(ofToString(int(timer)), ofGetWidth()/2-cwt/2, 150);
        }
        else{
            ofSetColor(255,0,0);
            figfont.drawString("0", ofGetWidth()/2-cwt/2, 150);
        }
    }
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
        case('s'):
            battle = true;
            start.play();
            break;
        case('r'):
            point1 = point2 = 0;
            timer = 60.0;
            battle  = result = replace = false;
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
            replace = !replace;
            break;
        case(OF_KEY_RETURN):
            result = true;
            break;
    }
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//-----------------------------------------------------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    //scale = MIN(DEFAULT_X/ofGetWidth(), DEFAULT_Y/ofGetHeight());
    //offsetX = (ofGetWidth()-DEFAULT_X*scale)/2;
    //offsetY = (ofGetHeight()-480*scale)/2;
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//-----------------------------------------------------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
