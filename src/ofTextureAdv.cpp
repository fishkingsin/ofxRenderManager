#include "ofTextureAdv.h"

//----------------------------------------------------------
ofTextureAdv::ofTextureAdv()
{

    //getTextureReference().texData.textureID = 0;//getTextureReference().texData.textureName[0] = 0;
    grid = new ofVec2f[GRID_X * GRID_Y];
    coor = new ofVec2f[GRID_X * GRID_Y];
    //offGrid  = new ofVec2f[GRID_X * GRID_Y];
	bDebug  =false;
	//vbo.setVertexData(coor,GRID_X * GRID_Y,GL_STREAM_DRAW);
	//vbo.setTexCoordData(grid,GRID_X * GRID_Y,GL_STREAM_DRAW);
	#ifdef USE_SHADER
	shader.setGeometryInputType(GL_LINES);
	shader.setGeometryOutputType(GL_QUADS);
	shader.setGeometryOutputCount(4);	
	shader.load("shaders/shader.vert","shaders/shader.frag");//,"shader.geom");
#endif
}


//----------------------------------------------------------
ofTextureAdv::~ofTextureAdv()
{
    delete []grid;
    delete []coor;
    ofLog(OF_LOG_NOTICE,"~ofTextureAdv() | \n");
    //delete offGrid;
}

//----------------------------------------------------------
void ofTextureAdv::clear()
{
    getTextureReference().texData.width = 0;
    getTextureReference().texData.height = 0;
}

//----------------------------------------------------------
void ofTextureAdv::setPoint(int which, float x, float y)
{

    if ( which>4 || which < 0)
    {
        ofLog(OF_LOG_WARNING,"ofTextureAdv:: please choose point 0-3\n");
        return;
    }

    quad[which].set(x, y);

    updatePoints();
}


//----------------------------------------------------------
void ofTextureAdv::setPoints(ofVec2f outputPts[4])
{
    for (int i = 0; i < 4; i++)
    {
        quad[i].set(outputPts[i].x, outputPts[i].y);
    }
    updatePoints();
}

//----------------------------------------------------------
void ofTextureAdv::setPoints(ofVec2f inputPts[4], ofVec2f outputPts[4])
{
    for (int i = 0; i < 4; i++)
    {
        quad[i].set(outputPts[i].x, outputPts[i].y);
        utQuad[i].set(inputPts[i].x, inputPts[i].y);
    }
    updatePoints();
}

//----------------------------------------------------------
void ofTextureAdv::updatePoints()
{

    int gridSizeX = GRID_X;
    int gridSizeY = GRID_Y;

    float xRes = 1.0/(gridSizeX-1);
    float yRes = 1.0/(gridSizeY-1);

    //GLfloat offsetw = 1.0f/(tex_w);
    //GLfloat offseth = 1.0f/(tex_h);

    //this is because we want pct to go from offsetw to tex_t - offsetw
    GLfloat texTAdj = getTextureReference().texData.tex_t;
    GLfloat texUAdj = getTextureReference().texData.tex_u;
    //ofLog(OF_LOG_VERBOSE,"%f %f \n", tex_t, tex_u);

    for (int y = 0; y < gridSizeY; y++)
    {
        for (int x = 0; x < gridSizeX; x++)
        {

            float pctx = (float)x * xRes;
            float pcty = (float)y * yRes;

            float linePt0x = (1-pcty)*quad[0].x + pcty * quad[3].x;
            float linePt0y = (1-pcty)*quad[0].y + pcty * quad[3].y;
            float linePt1x = (1-pcty)*quad[1].x + pcty * quad[2].x;
            float linePt1y = (1-pcty)*quad[1].y + pcty * quad[2].y;
            float ptx 	   = (1-pctx) * linePt0x + pctx * linePt1x;
            float pty 	   = (1-pctx) * linePt0y + pctx * linePt1y;

            float utPt0x = (1-pcty)*utQuad[0].x + pcty * utQuad[3].x;
            float utPt0y = (1-pcty)*utQuad[0].y + pcty * utQuad[3].y;
            float utPt1x = (1-pcty)*utQuad[1].x + pcty * utQuad[2].x;
            float utPt1y = (1-pcty)*utQuad[1].y + pcty * utQuad[2].y;
            float tt 	   = (1-pctx) * utPt0x + pctx * utPt1x;
            float uu 	   = (1-pctx) * utPt0y + pctx * utPt1y;

            grid[y*gridSizeX + x].set(ptx, pty);
            coor[y*gridSizeX + x].set( (tt * getTextureReference().texData.tex_t), getTextureReference().texData.bFlipTexture ? getTextureReference().texData.tex_u - (uu * getTextureReference().texData.tex_u) : (uu * getTextureReference().texData.tex_u));
        }
    }
	//vbo.setVertexData(coor,GRID_X * GRID_Y,GL_STREAM_DRAW);
	//vbo.setTexCoordData(grid,GRID_X * GRID_Y,GL_STREAM_DRAW);

}
void ofTextureAdv::draw()
{

    int gridSizeX = GRID_X;
    int gridSizeY = GRID_Y;
	#ifdef USE_SHADER
	shader.begin();
	shader.setUniformTexture("tex", getTextureReference(),0);
#endif
	getTextureReference().bind();
	//vbo.draw(GL_QUADS, 0,GRID_X*GRID_Y);
    //glEnable(getTextureReference().texData.textureTarget);
    //glBindTexture( getTextureReference().texData.textureTarget,getTextureReference().texData.textureID);// (GLuint)getTextureReference().texData.textureName[0] );
    for (int y = 0; y < gridSizeY-1; y++)
    {
        for (int x = 0; x < gridSizeX-1; x++)
        {

            glBegin(GL_QUADS);

            int pt0 = x + y*gridSizeX;
            int pt1 = (x+1) + y*gridSizeX;
            int pt2 = (x+1) + (y+1)*gridSizeX;
            int pt3 = x + (y+1)*gridSizeX;


			glTexCoord2f(coor[pt0].x, coor[pt0].y );
			glVertex2f(  grid[pt0].x, grid[pt0].y);

			glTexCoord2f(coor[pt1].x, coor[pt1].y );
			glVertex2f(  grid[pt1].x, grid[pt1].y);

			glTexCoord2f(coor[pt2].x, coor[pt2].y );
			glVertex2f(  grid[pt2].x, grid[pt2].y);

			glTexCoord2f(coor[pt3].x, coor[pt3].y );
			glVertex2f(  grid[pt3].x, grid[pt3].y);

            glEnd();

        }
    }
	getTextureReference().unbind();
	#ifdef USE_SHADER
	shader.end();
#endif
	if (bDebug)
    {
        ofPushStyle();
        ofNoFill();
        ofSetColor(0,255,255);
        for (int y = 0; y < gridSizeY-1; y++)
        {
            for (int x = 0; x < gridSizeX-1; x++)
            {
                int pt0 = x + y*gridSizeX;
                int pt1 = (x+1) + y*gridSizeX;
                int pt2 = (x+1) + (y+1)*gridSizeX;
                int pt3 = x + (y+1)*gridSizeX;

				glBegin(GL_LINE_STRIP);
				glVertex2f(  grid[pt0].x, grid[pt0].y);


                glVertex2f(  grid[pt1].x, grid[pt1].y);

                glVertex2f(  grid[pt2].x, grid[pt2].y);

                glVertex2f(  grid[pt3].x, grid[pt3].y);
                /*ofRect(  grid[pt0].x-4, grid[pt0].y-4 , 8, 8);


                ofRect(  grid[pt1].x-4, grid[pt1].y-4, 8, 8);

                ofRect(  grid[pt2].x-4, grid[pt2].y-4, 8, 8);

                ofRect(  grid[pt3].x-4, grid[pt3].y-4, 8, 8);*/
				glEnd();

            }

        }
        ofPushStyle();
        ofFill();
        ofSetColor(255,0,0);

        ofPopStyle();
        ofPopStyle();

    }

}
void ofTextureAdv::draw(ofVec2f *offGrid , int selectIndex)
{

    int gridSizeX = GRID_X;
    int gridSizeY = GRID_Y;
	#ifdef USE_SHADER
	shader.begin();
	shader.setUniformTexture("tex", getTextureReference(),0);
#endif
	getTextureReference().bind();
    for (int y = 0; y < gridSizeY-1; y++)
    {
        for (int x = 0; x < gridSizeX-1; x++)
        {

            glBegin(GL_QUADS);

            int pt0 = x + y*gridSizeX;
            int pt1 = (x+1) + y*gridSizeX;
            int pt2 = (x+1) + (y+1)*gridSizeX;
            int pt3 = x + (y+1)*gridSizeX;

            glTexCoord2f(coor[pt0].x, coor[pt0].y );
            glVertex2f(  offGrid[pt0].x+grid[pt0].x, offGrid[pt0].y+grid[pt0].y);

            glTexCoord2f(coor[pt1].x, coor[pt1].y );
            glVertex2f(  offGrid[pt1].x+grid[pt1].x, offGrid[pt1].y+grid[pt1].y);

            glTexCoord2f(coor[pt2].x, coor[pt2].y );
            glVertex2f(  offGrid[pt2].x+grid[pt2].x, offGrid[pt2].y+grid[pt2].y);

            glTexCoord2f(coor[pt3].x, coor[pt3].y );
            glVertex2f(  offGrid[pt3].x+grid[pt3].x, offGrid[pt3].y+grid[pt3].y);

            glEnd();

        }
    }
	getTextureReference().unbind();
	#ifdef USE_SHADER
	shader.end();
#endif
    if (bDebug)
    {
        ofPushStyle();
        ofNoFill();
        ofSetColor(0,255,255);
        for (int y = 0; y < gridSizeY-1; y++)
        {
            for (int x = 0; x < gridSizeX-1; x++)
            {
                int pt0 = x + y*gridSizeX;
                int pt1 = (x+1) + y*gridSizeX;
                int pt2 = (x+1) + (y+1)*gridSizeX;
                int pt3 = x + (y+1)*gridSizeX;

                ofRect(  offGrid[pt0].x+grid[pt0].x-4, offGrid[pt0].y+grid[pt0].y-4 , 8, 8);


                ofRect(  offGrid[pt1].x+grid[pt1].x-4, offGrid[pt1].y+grid[pt1].y-4, 8, 8);

                ofRect(  offGrid[pt2].x+grid[pt2].x-4, offGrid[pt2].y+grid[pt2].y-4, 8, 8);

                ofRect(  offGrid[pt3].x+grid[pt3].x-4, offGrid[pt3].y+grid[pt3].y-4, 8, 8);

                /*                glBegin(GL_QUADS);

                                glVertex2f(  grid[pt0].x, grid[pt0].y);


                                glVertex2f(  grid[pt1].x, grid[pt1].y);

                                glVertex2f(  grid[pt2].x, grid[pt2].y);

                                glVertex2f(  grid[pt3].x, grid[pt3].y);

                                glEnd();*/

            }

        }
        ofPushStyle();
        ofFill();
        ofSetColor(255,0,0);
        if (selectIndex <GRID_X * GRID_Y)
        {
            ofRect(  offGrid[selectIndex].x+grid[selectIndex].x-4,
                     offGrid[selectIndex].y+grid[selectIndex].y-4, 8, 8);
        }
        ofPopStyle();
        ofPopStyle();

    }
}
void ofTextureAdv::toggleDebug()
{
    bDebug  =! bDebug;
}


