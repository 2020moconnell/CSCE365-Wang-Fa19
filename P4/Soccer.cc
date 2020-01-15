#include "Soccer.h"

void display() {
	glutMouseFunc(OnMouseClick);
	glutKeyboardFunc(OnKeyboardStroke);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	drawGrass();
	glTranslatef(width/2, height/10, 0);
	glRotatef(x,0,1,0);
	glRotatef(y,1,0,0);
	drawSphere();
	glFlush();	
	glPopMatrix();
}

void myinit() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	LoadPPMImage(InputColorImage, Alpha, InputColorData);
  //LoadPPMImage(InputSoccerImage, Alpha, InputSoccerColor);
  LoadTexture();

  GLfloat position[]={0.0, width/2, -2*height/3, 1.0};
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
  GLfloat local_view[] = {0.0};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
  glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);

	//Soccer 1
  GLfloat amb1[]={0.75, 0.75, 0.75, 1.0};
  GLfloat dif1[]={1.0, 1.0, 1.0, 1.0};
  GLfloat spc1[]={0.5, 0.5, 0.5, 1.0};
  GLfloat shn1[] = {0.6};
  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightfv(GL_LIGHT0, GL_AMBIENT, amb1);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, dif1);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spc1);
  glLightfv(GL_LIGHT0, GL_SHININESS, shn1);
}

int main(int argc, char** argv) {
  glutInit(&argc,argv);
  glClearColor(0,0,0,0);

  glutInitWindowSize(width, height);
  glutInitWindowPosition(0,0);
  glutInitDisplayMode (GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow("Didn't see that one coming");
  glutDisplayFunc(display);
  glutIdleFunc(animate);

  //The projection of the Teapots
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,width, 0,height, -1000.0,1000.0);

  //How the orientation is for the Teapots
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0,0.0,0.0,0.0,0.0,-1.0,0.0,1.0,0.0);

  myinit();
  glutMainLoop();
}