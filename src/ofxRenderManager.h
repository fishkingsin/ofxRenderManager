#ifndef _RENDER_MANAGER_H
#define _RENDER_MANAGER_H

#include "ofxXmlSettings.h"
#include "ofTextureAdv.h"
#include "ofxDraggable.h"
class ofxRenderManager {


    public:

        ofxRenderManager();
        void allocateForNScreens(int numScreens, int renderWidth, int renderHeight);
        void allocateForNScreens(int _col ,int _row , int renderWidth, int renderHeight);
		void drawInputDiagnostically(ofRectangle rect);
		void drawOutputDiagnostically(ofRectangle rect);
        void drawInputDiagnostically(float x, float y, float w, float h); // can't specify abs dimensions, since might be diff apsect ratio...
        void drawOutputDiagnostically(float x, float y, float w, float h); // can't specify abs dimensions, since might be diff apsect ratio...

        void drawScreen(int nScreen);
		void drawScreens();

        void startOffscreenDraw();
        void endOffscreenDraw();

        bool mouseSelectInputPoint(ofRectangle drawRect, ofVec2f mousePoint, float clickRadius = 15);
        bool mouseDragInputPoint(ofRectangle drawRect, ofVec2f mousePoint);

        bool mouseSelectOutputPoint(ofRectangle drawRect, ofVec2f mousePoint, float clickRadius = 15);
        bool mouseDragOutputPoint(ofRectangle drawRect, ofVec2f mousePoint);

        virtual void resetCoordinates();

        virtual void reloadFromXml();
		virtual void loadFromXml(string xmlFileName);
		virtual void saveToXml();
        virtual void saveToXml(string xmlFileName);


        int    activeScreenIn;
        int    activePointIn;

        int    activeScreenOut;
        int    activePointOut;

        float width, height;
        float screenWidth, screenHeight;
        int nScreens;

        bool bInputSelected;

        ofVec2f ** outputPositions;
        ofVec2f ** inputPositions;

        string xmlFile;

        ofTextureAdv myOffscreenTexture;
        ofxXmlSettings xml;
		ofxDraggable **draggable;

};

#endif

