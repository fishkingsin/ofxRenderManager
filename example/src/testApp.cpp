#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	ofSetVerticalSync(true);
	ofSetFrameRate(60);
	ofEnableSmoothing();
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofEnableAlphaBlending();
	glDisable(GL_DEPTH_TEST);
    
    ofEnableArbTex();
    
    rm.allocateForNScreens(2,ofGetWidth(),ofGetHeight());
	rm.loadFromXml("fboSettings.xml");
    guiIn.set(300,300,360,240);
    guiOut.set(300+360+10,300,360,240);
    img.loadImage("url.jpeg");
}

//--------------------------------------------------------------
void testApp::update(){
    rm.startOffscreenDraw();
    ofClear(0);
    ofSetColor(100, 100, 100);
    ofRect(0, 0, rm.width, rm.height);
    
    //thick grid lines for blending
    ofSetColor(255, 255, 255);
    ofSetLineWidth(3);
    
    //vertical line
    for(int i = 0; i <= rm.width; i+=40){
        ofLine(i, 0, i, rm.height);
    }
    
    //horizontal lines
    for(int j = 0; j <= rm.height; j+=40){
        ofLine(0, j, rm.width, j);
    }
    img.draw(0,0);
    rm.endOffscreenDraw();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(100);
   
	
    ofSetColor(255);
    rm.drawScreens();
    ofPushStyle();
    rm.drawOutputDiagnostically(guiOut);
    rm.drawInputDiagnostically(guiIn);
    ofPopStyle();
    string str = "'d':rm.myOffscreenTexture.toggleDebug()\n"
    "'r':rm.reloadFromXml()\n"
    "'s':rm.saveSettings()\n"
    "'c':rm.resetCoordinates()\n"
    "'1':rm.ENABLE_SCREEN[0] Toggle\n"
    "'2':rm.ENABLE_SCREEN[1] Toggle\n"
    "'3':rm.CONTROL_SCREEN[0] Toggle\n"
    "'4':rm.CONTROL_SCREEN[1] Toggle\n";
    
    ofDrawBitmapString(str, 20,20);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if(!rm.keyPressedOutputPoint(key))
	{
		if(rm.keyPressedInputPoint(key))
		{
			return ;
		}
	}

    switch(key)
	{
        case 'd':rm.myOffscreenTexture.toggleDebug();break;
        case 'r':rm.reloadFromXml();break;
        case 's':rm.saveToXml() ;break;
        case 'c':rm.resetCoordinates();break;
        case '1':rm.ENABLE_SCREEN[0] = !rm.ENABLE_SCREEN[0]; break;
        case '2':rm.ENABLE_SCREEN[1] = !rm.ENABLE_SCREEN[1]; break;
            case '3':rm.CONTROL_SCREEN[0] = !rm.CONTROL_SCREEN[0]; break;
            case '4':rm.CONTROL_SCREEN[1] = !rm.CONTROL_SCREEN[1]; break;
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}
//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    if(!rm.mouseDragInputPoint(guiIn, ofPoint(x,y)))
    {
        rm.mouseDragOutputPoint(guiOut, ofPoint(x,y));
    }
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if(!rm.mouseSelectInputPoint(guiIn, ofPoint(x,y)))
    {
        rm.mouseSelectOutputPoint(guiOut, ofPoint(x,y));
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    if(!rm.mouseSelectInputPoint(guiIn, ofPoint(x,y)))
    {
        rm.mouseSelectOutputPoint(guiOut, ofPoint(x,y));
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}