#ifndef _IMAGE_TEXTURE_ADV_H_
#define _IMAGE_TEXTURE_ADV_H_

#include "ofMain.h"
//render for of 007

#define GRID_X 8
#define GRID_Y 8
//#define USE_SHADER

class ofTextureAdv : public ofFbo
{

public :
    ofTextureAdv();
    ~ofTextureAdv();
   // void allocate(int w, int h, int internalGlDataType);
    void clear();
    void loadData(unsigned char * data, int w, int h, int glDataType);
    void setPoint(int which, float x, float y);
    void setPoints(ofVec2f pts[4]);
    void setPoints(ofVec2f inputPts[4], ofVec2f outputPts[4]);
	void draw();
    void draw(ofVec2f *offGrid , int selectIndex);
    void draw(float x, float y)
    {
        ofFbo::draw(x,y);
    };
    void draw(float x, float y, float w, float h)
    {
        ofFbo::draw(x,y,w,h);
    };
    void allocate(int w, int h, bool clear)
    {
        printf("ofTextureAdv allocate %i %i\n",w,h);
        ofFbo::allocate(w,h, clear);
    };
    void toggleDebug();


    int width, height;

    bool bDebug;

protected:

    virtual void updatePoints();

    ofVec2f quad[4];
    ofVec2f utQuad[4];
    ofVec2f * grid;
    ofVec2f * coor;
	//ofVbo vbo;
#ifdef USE_SHADER
	ofShader shader;
#endif
};

#endif
