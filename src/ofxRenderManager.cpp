

#include "ofxRenderManager.h"
#define TAG "ofxRenderManager"
//---------------------------------------------------------------------------
ofxRenderManager::ofxRenderManager()
{
    bInputSelected = false;
	
    activeScreenIn = -1;
    activePointIn  = -1;
	
    activeScreenOut = -1;
    activePointOut  = -1;
	
    xmlFile = "";
	
    nScreens = 0;
	
	
}

//---------------------------------------------------------------------------
void ofxRenderManager::allocateForNScreens(int numScreens, int renderWidth, int renderHeight)
{
    if ( numScreens < 1)
    {
        ofLog(OF_LOG_VERBOSE,"allocateForNScreens - you need at lease one screen!\n");
        return;
    }
    width   = renderWidth;
    height  = renderHeight;
	ENABLE_SCREEN = new bool[numScreens];
    screenWidth  = (float)width / (float)numScreens;
    screenHeight = height;
	
    myOffscreenTexture.ofFbo::allocate(renderWidth, renderHeight, GL_RGBA);
	
    nScreens            = numScreens;
    outputPositions     = new ofVec2f*[nScreens];
    inputPositions      = new ofVec2f*[nScreens];
	draggable = new ofxDraggable*[nScreens];
	
    float w_d_3 = (float)width / (float)nScreens;
    float inner_d_3 = 1.0f / (float)nScreens;
    for (int i = 0; i < nScreens; i++)
    {
		
        inputPositions[i]  = new ofVec2f[4];
        outputPositions[i] = new ofVec2f[4];
		draggable[i] = new ofxDraggable[8];
		
        //-----------------------------------------
        outputPositions[i][0].set(w_d_3*(i),0);
        outputPositions[i][1].set(w_d_3*(i+1),0);
        outputPositions[i][2].set(w_d_3*(i+1),height);
        outputPositions[i][3].set(w_d_3*(i),height);
		
        inputPositions[i][0].set(inner_d_3*(i),0);
        inputPositions[i][1].set(inner_d_3*(i+1),0);
        inputPositions[i][2].set(inner_d_3*(i+1),1);
        inputPositions[i][3].set(inner_d_3*(i),1);
		
		draggable[i][0].set(w_d_3*(i),0,5,5);
		draggable[i][1].set(w_d_3*(i),0,5,5);
		draggable[i][2].set(w_d_3*(i+1),0,5,5);
		draggable[i][3].set(w_d_3*(i+1),0,5,5);
		draggable[i][4].set(w_d_3*(i+1),height,5,5);
		draggable[i][5].set(w_d_3*(i+1),height,5,5);
		draggable[i][6].set(w_d_3*(i),height,5,5);
		draggable[i][7].set(w_d_3*(i),height,5,5);
		
		
		
        ofLog(OF_LOG_VERBOSE,"input is \n");
        ofLog(OF_LOG_VERBOSE,"(%f %f)          ", inputPositions[i][0].x, inputPositions[i][0].y);
        ofLog(OF_LOG_VERBOSE,"(%f %f)\n", inputPositions[i][1].x, inputPositions[i][1].y);
        ofLog(OF_LOG_VERBOSE,"(%f %f)          ", inputPositions[i][2].x, inputPositions[i][2].y);
        ofLog(OF_LOG_VERBOSE,"(%f %f)\n", inputPositions[i][3].x, inputPositions[i][3].y);
		
        ofLog(OF_LOG_VERBOSE,"\noutput is \n");
        ofLog(OF_LOG_VERBOSE,"(%f %f)          ", outputPositions[i][0].x, outputPositions[i][0].y);
        ofLog(OF_LOG_VERBOSE,"(%f %f)\n", inputPositions[i][1].x, outputPositions[i][1].y);
        ofLog(OF_LOG_VERBOSE,"(%f %f)          ", outputPositions[i][2].x, outputPositions[i][2].y);
        ofLog(OF_LOG_VERBOSE,"(%f %f)\n", outputPositions[i][3].x, outputPositions[i][3].y);
    }
}

//---------------------------------------------------------------------------
void ofxRenderManager::startOffscreenDraw()
{
    if ( nScreens <= 0 ) return;
	myOffscreenTexture.begin();
	// myOffscreenTexture.swapIn();
	// myOffscreenTexture.setupScreenForMe();
}

//---------------------------------------------------------------------------
void ofxRenderManager::endOffscreenDraw()
{
    if ( nScreens <= 0 ) return;
	myOffscreenTexture.end();
    //myOffscreenTexture.swapOut();
	// myOffscreenTexture.setupScreenForThem();
}
void ofxRenderManager::drawInputDiagnostically(ofRectangle rect)
{
	drawInputDiagnostically( rect.x,  rect.y,  rect.width,  rect.height);
}
//---------------------------------------------------------------------------
void ofxRenderManager::drawInputDiagnostically(float x, float y, float w, float h)
{
    if ( nScreens <= 0 ) return;
	ofEnableAlphaBlending();
	ofPushStyle();
	ofSetColor(255, 255, 255,150);
    myOffscreenTexture.draw(x, y, w,h);
	ofPopStyle();
    for (int i = 0; i < nScreens; i++)
    {
		if(ENABLE_SCREEN[i])
		{
			ofSetHexColor(0x33DD44);
			ofNoFill();
			ofBeginShape();
			for (int j = 0; j < 4; j++)
			{
				ofVertex( inputPositions[i][j].x * w + x, inputPositions[i][j].y * h + y);
			}
			ofEndShape(true);
			
			ofFill();
			ofSetHexColor(0x3344FF);
			for (int j = 0; j < 4; j++)
			{
				
				ofCircle(  inputPositions[i][j].x * w + x, inputPositions[i][j].y * h + y,3);
			}
			if(activeScreenIn>=0)
			{
				
				if(activePointIn>=0)
				{
					ofPushStyle();
					ofNoFill();
					ofCircle(  inputPositions[activeScreenIn][activePointIn].x * w + x, inputPositions[activeScreenIn][activePointIn].y * h + y,10);
					ofPopStyle();
				}
			}
			//		   activeScreenOut     = -1;
			//		   activePointOut)
		}
    }
	
}
void ofxRenderManager::drawOutputDiagnostically(ofRectangle rect)
{
	drawOutputDiagnostically( rect.x,  rect.y,  rect.width,  rect.height);
}
//---------------------------------------------------------------------------
void ofxRenderManager::drawOutputDiagnostically(float x, float y, float w, float h)
{
    if ( nScreens <= 0 ) return;
	
    glPushMatrix();
    glTranslatef(x, y, 0);
	
    ofNoFill();
    ofSetHexColor(0x000000);
    ofRect(0, 0, w, h);
    ofFill();
	
    for (int i = 0; i < nScreens; i++)
    {
		if(ENABLE_SCREEN[i])
		{
			
			glPushMatrix();
			
			ofPushStyle();
			ofSetColor(255, 255, 255,150);
			ofEnableAlphaBlending();
			float wScale = ( w / screenWidth ) / nScreens;
			float hScale = h / screenHeight;
			glScalef(wScale, hScale, 1);
			drawScreen(i);
			ofPopStyle();
			ofSetHexColor(0x4444CC);
			ofNoFill();
			ofBeginShape();
			for (int j = 0; j < 4; j++)
			{
				ofVertex( outputPositions[i][j].x  , outputPositions[i][j].y );
			}
			ofEndShape(true);
			
			ofFill();
			ofSetHexColor(0xFF3333);
			for (int j = 0; j < 4; j++)
			{
				ofCircle(  outputPositions[i][j].x , outputPositions[i][j].y, 5 );
			}
			if(activeScreenOut>=0)
			{
				
				if(activePointOut>=0)
				{
					ofPushStyle();
					ofNoFill();
					ofCircle(  outputPositions[activeScreenOut][activePointOut].x , outputPositions[activeScreenOut][activePointOut].y ,10);
					ofPopStyle();
				}
			}
			glPopMatrix();
		}
    }
	
    glPopMatrix();
	
}

//---------------------------------------------------------------------------
void ofxRenderManager::drawScreen(int nScreen)
{
    if ( nScreens <= nScreen) return;
	if(ENABLE_SCREEN[nScreen])
	{
		myOffscreenTexture.setPoints(inputPositions[nScreen], outputPositions[nScreen]);
		myOffscreenTexture.draw();
		if(myOffscreenTexture.bDebug)
		{
			ofPushStyle();
			ofSetColor(255,0,255);
			ofNoFill();
			ofBeginShape();
			for (int j = 0; j < 4; j++)
			{
				ofVertex( outputPositions[nScreen][j].x  , outputPositions[nScreen][j].y );
			}
			ofEndShape(true);
			ofPopStyle();
			ofFill();
			ofSetHexColor(0xFF3333);
			for (int j = 0; j < 4; j++)
			{
				ofLine( outputPositions[nScreen][j].x , outputPositions[nScreen][j].y -5,
					   outputPositions[nScreen][j].x , outputPositions[nScreen][j].y + 5 );
				ofLine( outputPositions[nScreen][j].x -5, outputPositions[nScreen][j].y,
					   outputPositions[nScreen][j].x +5, outputPositions[nScreen][j].y );
			}
			if(activeScreenOut>=0)
			{
				
				if(activePointOut>=0)
				{
					ofPushStyle();
					ofNoFill();
					ofCircle(  outputPositions[activeScreenOut][activePointOut].x , outputPositions[activeScreenOut][activePointOut].y ,10);
					ofPopStyle();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void ofxRenderManager::drawScreens()
{
	for (int i = 0; i < nScreens; i++)
    {
		drawScreen(i);
	}
}

//---------------------------------------------------------------------------
void ofxRenderManager::resetCoordinates()
{
    if ( nScreens <= 0)return;
	
    float inner_d_3 = 1.0f / (float)nScreens;
    for (int i = 0; i < nScreens; i++)
    {
		
        //-----------------------------------------
        outputPositions[i][0].set(screenWidth*(i),0);
        outputPositions[i][1].set(screenWidth*(i+1),0);
        outputPositions[i][2].set(screenWidth*(i+1),height);
        outputPositions[i][3].set(screenWidth*(i),height);
		
        inputPositions[i][0].set(inner_d_3*(i),0);
        inputPositions[i][1].set(inner_d_3*(i+1),0);
        inputPositions[i][2].set(inner_d_3*(i+1),1);
        inputPositions[i][3].set(inner_d_3*(i),1);
    }
}

//---------------------------------------------------------------------------
void ofxRenderManager::loadFromXml(string xmlFileName)
{
    xmlFile = xmlFileName;
    xml.loadFile(xmlFile);
    reloadFromXml();
}

//---------------------------------------------------------------------------
void ofxRenderManager::reloadFromXml()
{
    if (xmlFile == "")return;
	
    for (int i = 0; i < nScreens; i++)
    {
		
        float inner_d_3 = 1.0 / nScreens;
		
        if ( xml.pushTag("QUAD_IN_"+ofToString(i)) )
        {
			
            float inner_d_3 = 1.0 / nScreens;
			
            inputPositions[i][0].x  = xml.getValue("pt0:x", inner_d_3*(i));
            inputPositions[i][0].y  = xml.getValue("pt0:y", 0.0f);
			
            inputPositions[i][1].x  = xml.getValue("pt1:x", inner_d_3*(i+1));
            inputPositions[i][1].y  = xml.getValue("pt1:y", 0.0f);
			
            inputPositions[i][2].x  = xml.getValue("pt2:x", inner_d_3*(i+1));
            inputPositions[i][2].y  = xml.getValue("pt2:y", 1.0f);
			
            inputPositions[i][3].x  = xml.getValue("pt3:x", inner_d_3*(i));
            inputPositions[i][3].y  = xml.getValue("pt3:y", 1.0f);
			
            xml.popTag();
        }
        if ( xml.pushTag("QUAD_OUT_"+ofToString(i)) )
        {
			
            outputPositions[i][0].x  = xml.getValue("pt0:x", screenWidth*(i));
            outputPositions[i][0].y  = xml.getValue("pt0:y", 0);
			
            outputPositions[i][1].x  = xml.getValue("pt1:x", screenWidth*(i+1));
            outputPositions[i][1].y  = xml.getValue("pt1:y", 0);
			
            outputPositions[i][2].x  = xml.getValue("pt2:x", screenWidth*(i+1));
            outputPositions[i][2].y  = xml.getValue("pt2:y", height);
			
            outputPositions[i][3].x  = xml.getValue("pt3:x", screenWidth*(i));
            outputPositions[i][3].y  = xml.getValue("pt3:y", height);
			
            xml.popTag();
        }
		
    }
}


//---------------------------------------------------------------------------
void ofxRenderManager::saveToXml()
{
    if (xmlFile != "")
    {
        saveToXml(xmlFile);
    }
}


void ofxRenderManager::saveToXml(string xmlFileName)
{
    if (xmlFileName == "")return;
	
    //clear out the file
    xml.clear();
	
    for (int i = 0; i < nScreens; i++)
    {
		
        float inner_d_3 = 1.0 / nScreens;
		
        xml.addTag("QUAD_IN_"+ofToString(i) );
        if ( xml.pushTag("QUAD_IN_"+ofToString(i)) )
        {
			
            float inner_d_3 = 1.0 / nScreens;
			
            xml.setValue("pt0:x", inputPositions[i][0].x);
            xml.setValue("pt0:y", inputPositions[i][0].y);
			
            xml.setValue("pt1:x", inputPositions[i][1].x);
            xml.setValue("pt1:y", inputPositions[i][1].y);
			
            xml.setValue("pt2:x", inputPositions[i][2].x);
            xml.setValue("pt2:y", inputPositions[i][2].y);
			
            xml.setValue("pt3:x",  inputPositions[i][3].x);
            xml.setValue("pt3:y",  inputPositions[i][3].y);
			
            xml.popTag();
        }
		
        xml.addTag("QUAD_OUT_"+ofToString(i) );
        if ( xml.pushTag("QUAD_OUT_"+ofToString(i)) )
        {
			
            xml.setValue("pt0:x", outputPositions[i][0].x);
            xml.setValue("pt0:y", outputPositions[i][0].y);
			
            xml.setValue("pt1:x", outputPositions[i][1].x);
            xml.setValue("pt1:y", outputPositions[i][1].y);
			
            xml.setValue("pt2:x", outputPositions[i][2].x);
            xml.setValue("pt2:y", outputPositions[i][2].y);
			
            xml.setValue("pt3:x", outputPositions[i][3].x);
            xml.setValue("pt3:y", outputPositions[i][3].y);
			
            xml.popTag();
        }
    }
	
    xml.saveFile(xmlFileName);
}

//---------------------------------------------------------------------------
bool ofxRenderManager::mouseSelectInputPoint(ofRectangle drawRect, ofVec2f mousePoint, float clickRadius)
{
    if ( nScreens <= 0 ) return false;
	
    //this is not the best way
    activeScreenIn      = -1;
    activePointIn       = -1;
    activeScreenOut     = -1;
    activePointOut      = -1;
	
    float smallestDist  = 999999;
	
    bool bFound = false;
	
    for (int i = 0; i < nScreens; i++)
    {
		if(ENABLE_SCREEN[i])
		{
			for (int j = 0; j < 4; j++)
			{
				ofVec2f inputPt;
				inputPt.x = inputPositions[i][j].x * drawRect.width + drawRect.x;
				inputPt.y = inputPositions[i][j].y * drawRect.height + drawRect.y;
				
				float len = sqrt( (inputPt.x - mousePoint.x) * (inputPt.x - mousePoint.x) +
								 (inputPt.y - mousePoint.y) * (inputPt.y - mousePoint.y));
				if (len < clickRadius && len < smallestDist)
				{
					activeScreenIn = i;
					activePointIn  = j;
					smallestDist = len;
					
					bFound = true;
				}
			}
		}
    }
	
    return bFound;
}

//---------------------------------------------------------------------------
bool ofxRenderManager::mouseDragInputPoint(ofRectangle drawRect, ofVec2f mousePoint)
{
    if ( nScreens <= 0 ) return false;
	
    if (activeScreenIn > -1 && activePointIn > -1)
    {
		
		if(ENABLE_SCREEN[activeScreenIn])
		{
			
			float xpct = (mousePoint.x - drawRect.x) / drawRect.width;
			float ypct = (mousePoint.y - drawRect.y) / drawRect.height;
			inputPositions[activeScreenIn][activePointIn].x = xpct;
			inputPositions[activeScreenIn][activePointIn].y = ypct;
			
			return true;
		}
		
    }
	
    return false;
}

//---------------------------------------------------------------------------
bool ofxRenderManager::mouseSelectOutputPoint(ofRectangle drawRect, ofVec2f mousePoint, float clickRadius)
{
    if ( nScreens <= 0 ) return false;
	
    //this is not the best way
    activeScreenIn      = -1;
    activePointIn       = -1;
    activeScreenOut     = -1;
    activePointOut      = -1;
	
    float smallestDist  = 999999;
	
    bool bFound = false;
	
    for (int i = 0; i < nScreens; i++)
    {
		if(ENABLE_SCREEN[i])
		{
			for (int j = 0; j < 4; j++)
			{
				ofVec2f outputPt;
				
				//lets get the screen width and height at our scale
				outputPt.x = outputPositions[i][j].x;// * drawRect.width) + drawRect.x;
				outputPt.y = outputPositions[i][j].y;// * drawRect.height) + drawRect.y;
				
				outputPt.x *= ( (float)drawRect.width  / screenWidth )/nScreens;
				outputPt.y *= (float)drawRect.height / screenHeight;
				
				outputPt.x += drawRect.x;
				outputPt.y += drawRect.y;
				
				//ofLog(OF_LOG_VERBOSE,"outputPt  is %f %f \n", outputPt.x, outputPt.y);
				
				float len = sqrt( (outputPt.x - mousePoint.x) * (outputPt.x - mousePoint.x) +
								 (outputPt.y - mousePoint.y) * (outputPt.y - mousePoint.y));
				if (len < clickRadius && len < smallestDist)
				{
					activeScreenOut = i;
					activePointOut = j;
					smallestDist = len;
					
					//ofLog(OF_LOG_VERBOSE,"setting point %i of screen %i \n", activePointOut, activeScreenOut);
					
					bFound = true;
				}
			}
        }
    }
    return bFound;
	
}

//---------------------------------------------------------------------------
bool ofxRenderManager::mouseDragOutputPoint(ofRectangle drawRect, ofVec2f mousePoint)
{
    if ( nScreens <= 0 ) return false;
	
    if (activeScreenOut > -1 && activePointOut > -1)
    {
		
		activeScreenIn      = -1;
		activePointIn       = -1;
		
		
        float xpct = (mousePoint.x - drawRect.x) / (drawRect.width / nScreens);
        float ypct = (mousePoint.y - drawRect.y) / drawRect.height;
		
        float outPtX = xpct * screenWidth;
        float outPtY = ypct * screenHeight;
		if(ENABLE_SCREEN[activeScreenOut])
		{
			outputPositions[activeScreenOut][activePointOut].x = outPtX;
			outputPositions[activeScreenOut][activePointOut].y = outPtY;
			
			//ofLog(OF_LOG_VERBOSE,"point - %i  of screen %i setting output position to %f %f \n",activePointOut,  activeScreenOut, outputPositions[activeScreenOut][activePointOut].x, outputPositions[activeScreenOut][activePointOut].y);
			
			return true;
		}else{
			return false;
		}
    }
    return false;
}
bool ofxRenderManager::keyPressedInputPoint(int key)
{
    if ( nScreens <= 0 ) return false;
	
    if (activeScreenIn > -1 && activePointIn > -1)
    {
		if(ENABLE_SCREEN[activeScreenIn])
		{
			float diffX = 0;
			float diffY = 0;
			switch(key)
			{
				case OF_KEY_LEFT:
					diffX = -ofGetLastFrameTime();
					break;
				case OF_KEY_RIGHT:
					diffX = ofGetLastFrameTime();
					break;
				case OF_KEY_UP:
					diffY = -ofGetLastFrameTime();
					break;
				case OF_KEY_DOWN:
					diffY = ofGetLastFrameTime();
					break;
			}
			inputPositions[activeScreenIn][activePointIn].x += diffX*0.1;
			inputPositions[activeScreenIn][activePointIn].y += diffY*0.1;
			
			//        ofLog(OF_LOG_VERBOSE,"point - %i  of screen %i setting output position to %f %f \n",activePointOut,  activeScreenOut, outputPositions[activeScreenOut][activePointOut].x, outputPositions[activeScreenOut][activePointOut].y);
		}
        return true;
    }
    return false;
}

bool ofxRenderManager::keyPressedOutputPoint(int key)
{
    if ( nScreens <= 0 ) return false;
	
    if (activeScreenOut > -1 && activePointOut > -1)
    {
		
        if(ENABLE_SCREEN[activeScreenOut])
		{
			float diffX = 0;
			float diffY = 0;
			switch(key)
			{
				case OF_KEY_LEFT:
					diffX = -ofGetLastFrameTime();
					break;
				case OF_KEY_RIGHT:
					diffX = ofGetLastFrameTime();
					break;
				case OF_KEY_UP:
					diffY = -ofGetLastFrameTime();
					break;
				case OF_KEY_DOWN:
					diffY = ofGetLastFrameTime();
					break;
			}
			outputPositions[activeScreenOut][activePointOut].x += diffX*10;
			outputPositions[activeScreenOut][activePointOut].y += diffY*10;
			
			//        ofLog(OF_LOG_VERBOSE,"point - %i  of screen %i setting output position to %f %f \n",activePointOut,  activeScreenOut, outputPositions[activeScreenOut][activePointOut].x, outputPositions[activeScreenOut][activePointOut].y);
		}
        return true;
    }
    return false;
}


