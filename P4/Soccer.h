#ifndef Project4
#define Project4

#include <cstdlib>
#include <cstdio>
#include <cmath>

#include <ctime>
#include <GL/glut.h>
#include <stdlib.h> 
#include <iostream>
using namespace std;


#include "Image565.h"

const int MaxWidth=3000;
const int MaxHeight=3000;
const int height = 720;
const int width = 1280;
const int heighti = 800;
const int widthi = 800;

char InputColorImage[16]="soccerfield.PPM";
char InputSoccerImage[16]="soccerball.PPM";

static GLubyte InputColorData[MaxHeight][MaxWidth][4];
static GLubyte InputSoccerColor[MaxHeight][MaxWidth][4];

int x, y;
int ImageHeight;
int ImageWidth;

GLuint texid;

int sMove = 1;
double sScale = 1;
int sRotate;
double scaleVal;
int rotateVal;
int moveVal;
int moveLimit = 0;

GLUquadricObj *sphere;

unsigned char Alpha = 100;

void animate() {
	glutPostRedisplay();
}

void OnMotion(int x2, int y2) {
	x = 360*x2/(sqrt(height*height+width*width));
	y = 360*y2/(sqrt(height*height+width*width));
}

void OnMouseClick(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		glutMotionFunc(OnMotion);
}

void OnKeyboardStroke(unsigned char key, int x2, int y2) {
	if (key == 'l' || key == 'L') {
		x = y = 0;
    sScale = 1;
    sMove = 0;
    sRotate = 0;
    moveLimit = 0;
    rotateVal = 0;
  }
	if (key == 'h' || key == 'H') {
    if (moveLimit <= 300) {
      moveLimit += 75;
  		moveVal = 1;
      scaleVal = .00235;
      rotateVal += 4;
    }
  }
    
}

int LoadPPMImage(char* filename, unsigned char alpha, unsigned char pdata[MaxHeight][MaxWidth][4])
{
  ByteImage texture;
  
  texture.ReadPnmFile(filename);
  ImageHeight=texture.NumRows();
  ImageWidth=texture.NumCols();
  
  for(int i=0; i<ImageHeight; ++i) {
    for(int j=0; j<ImageWidth; ++j) {
      pdata[i][j][0]=texture.GetPixel(0,i,j);
      pdata[i][j][1]=texture.GetPixel(1,i,j);
      pdata[i][j][2]=texture.GetPixel(2,i,j);
      pdata[i][j][3]=alpha;
    }
  }
}

void LoadTexture() {
  glGenTextures(1, &texid); /* Texture name generation */
  glBindTexture(GL_TEXTURE_2D, texid); /* Binding of texture name */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); /* We will use linear interpolation for magnification filter */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); /* We will use linear interpolation for minifying filter */
}

void drawSphere() {
  glTranslatef(width/2, height/2,0);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glBegin(GL_POINTS);
  glVertex3d(0, -100, 0);
  for (int i=-900; i < 900; ++i) {
    float alt = i;
    float c_alt = cos(alt);
    float s_alt = sin(alt);
    for (int j=0; j < 3600; j++) {
      float azi = j;
      float c_azi = cos(azi);
      float s_azi = sin(azi);
      glVertex3d(c_azi*c_alt, s_alt, s_azi*c_alt);
    }
  }
  glVertex3d(0, 100, 0);
  glEnd();
  // glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
  // glEnable (GL_TEXTURE_2D);
  // sphere = gluNewQuadric();
  // glTranslatef(0,sMove,sMove);
  // glScaled(sScale,sScale,sScale);
  // glRotatef(sRotate,-1 ,0 , 0);
  // if (sMove < moveLimit) {
  //   x = y = 0;
  //   sMove = sMove + moveVal;
  //   sScale = sScale - scaleVal;
  //   sRotate = sRotate + rotateVal;
  // }
  // gluQuadricDrawStyle(sphere, GLU_FILL);
  // gluQuadricTexture(sphere, GL_TRUE);
  // gluQuadricNormals(sphere, GLU_SMOOTH);
  // gluSphere(sphere, 42, 210, 210);

  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHTING);
  glutSwapBuffers();
  //glDisable(GL_TEXTURE_2D);
}
void drawGrass() {
  glBegin(GL_POINTS);
  glRotatef(1200,1,1,1);
  for(int i=0; i<ImageHeight; ++i)
  {
    for(int j=0; j<ImageWidth; ++j)
    {
      glColor3ub(InputColorData[i][j][0], InputColorData[i][j][1], InputColorData[i][j][2]);
      glVertex3f(j,height-i,0);
    }
  }
  glEnd();
}

#endif