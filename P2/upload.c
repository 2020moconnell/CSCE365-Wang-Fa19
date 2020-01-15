#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h> 
#include <GL/gl.h>
#include <vector>

/******************************************************************************/
// Utility Functions

//borrowed from example
#define HEIGHT 500
#define WIDTH 500
#define M_PI 3.14159265358979323846
// Variables
GLfloat theta_calc_denom;
int left_mouse_state;
int right_mouse_state;
GLfloat prev_x;
GLfloat prev_y;
GLfloat angle_x1;
GLfloat angle_y1;
bool closed;
std::vector<std::vector<int> > verts;//{ {0,0},{1,0},{1,1},{0,1} };
std::vector<int> currVert;

void setMaterial(GLfloat ambientR, GLfloat ambientG, GLfloat ambientB, GLfloat diffuseR, GLfloat diffuseG, GLfloat diffuseB, GLfloat specularR, GLfloat specularG, GLfloat specularB, GLfloat shininess) //easier to see depth if this is turned off

{//sets the material for the object
	GLfloat ambient[] = { ambientR, ambientG, ambientB };
	GLfloat diffuse[] = { diffuseR, diffuseG, diffuseB, };
	GLfloat specular[] = { specularR, specularG, specularB };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void makePolygon()
{
	//glPushMatrix();
	glBegin(GL_POLYGON); //front poly
	glNormal3f(-1, 0, 0);
	for (int i = 0; i < verts.size(); i++)
	{
		glVertex3f(verts.at(i).at(0), verts.at(i).at(1), -1);
		printf("front vertex at (%d , %d)\n", verts.at(i).at(0), verts.at(i).at(1));
	}
	glEnd();

	//glPushMatrix(); //back poly
	glBegin(GL_POLYGON);
	glNormal3f(1,0,0)
	for (int j = 0; j < verts.size(); j++)
	{
		glVertex3f(verts.at(j).at(0), verts.at(j).at(1), -2);
		printf("back vertex at (%d , %d)\n", verts.at(j).at(0), verts.at(j).at(1));
	}
	glEnd();

	for (int j = 0; j < verts.size(); j++) //connects back to front polygons
	{
		if (j == verts.size() - 1)
		{
			//glPushMatrix();
			glBegin(GL_POLYGON);
			glVertex3f(verts.at(0).at(0), verts.at(0).at(1), -1);
			glVertex3f(verts.at(verts.size() - 1).at(0), verts.at(verts.size() - 1).at(1), -1);
			glVertex3f(verts.at(verts.size() - 1).at(0), verts.at(verts.size() - 1).at(1), -2);
			glVertex3f(verts.at(0).at(0), verts.at(0).at(1), -2);
			glEnd();


		}
		else {
			//glPushMatrix();
			glBegin(GL_POLYGON);
			glVertex3f(verts.at(j).at(0), verts.at(j).at(1), -1);
			glVertex3f(verts.at(j + 1).at(0), verts.at(j + 1).at(1), -1);
			glVertex3f(verts.at(j + 1).at(0), verts.at(j + 1).at(1), -2);
			glVertex3f(verts.at(j).at(0), verts.at(j).at(1), -2);
			glEnd();
		}
	}
}

void polygon() {

	glPushMatrix();
	//glTranslatef(-10,-10,0); //added
	setMaterial(0, .5, 0, 0, .5, .7, .7, .7, .7, .7);
	//comment out to see depth more easily
	glRotatef(angle_x1, 0, 1, 0);
	glRotatef(angle_y1, 1, 0, 0);
	makePolygon();
	glPopMatrix();
}


void myinit()
//borrowed from example
{
	theta_calc_denom = sqrtf(WIDTH * WIDTH + HEIGHT * HEIGHT);

	gluLookAt(0, 1, 0, 0, 0, 0, 0, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.3, 0.3, 0.3, 1.0); //bg color
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();
	glOrtho(-10.0, 10.0, -10.0, 10.0, 0.0, 10.0);

	GLfloat position[] = { 1,0,0,1 };
	GLfloat amb[] = { .6,.4,.3,.5 };
	GLfloat dif[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat spc[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat local_view[] = { 0.0 };
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spc);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	angle_x1 = 0;
	angle_y1 = 0;
	left_mouse_state = 1;
	right_mouse_state = 1;

}

/******************************************************************************/

void display()
{
	glViewport(0, 0, 500, 500);
	polygon();
	glFlush();
}
/******************************************************************************/
void rotate(int x, int y) {
	//borrowed from ex
	if (closed) {
		GLfloat angle_x = (360.0 * (-1 * x + prev_x)) / theta_calc_denom;
		GLfloat angle_y = (360.0 * (-1 * y + prev_y)) / theta_calc_denom;
		if (left_mouse_state == 0) {
			angle_x1 = angle_x;
			angle_y1 = angle_y;
		}
		glClear(GL_COLOR_BUFFER_BIT);
		glutPostRedisplay();
	}

}
/******************************************************************************/
void drawPoly(int x, int y)
{
	currVert.push_back(x - 4); //check this
	currVert.push_back(y - 4);
	verts.push_back(currVert);
	currVert.clear();
	printf("pushing corrd: (%d,%d) \n", x, y);
}
/******************************************************************************/
void mouse_func(int button, int state, int x, int y) { //keeps track of the status of the mouse buttons
	prev_x = x;
	prev_y = y;
	if (button == GLUT_LEFT_BUTTON) {

		left_mouse_state = state;
		if (!closed)
			drawPoly(x / 50, y / 50);
	}
	if (button == GLUT_RIGHT_BUTTON) {
		closed = true;
		right_mouse_state = state;
	}
}

void mouse_motion(int x, int y) { //mouseMotion

	rotate(x, y);
}
/******************************************************************************/
void keyboard_func(unsigned char key, int x, int y) {
	//borrowed from ex
	if (key == 'l' || key == 'L') {
		angle_x1 = 0;
		angle_y1 = 0;
		glClear(GL_COLOR_BUFFER_BIT);
		glutPostRedisplay();
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutInitWindowSize(HEIGHT, WIDTH);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Teapots");
	myinit();

	glutDisplayFunc(display);
	glutMouseFunc(mouse_func);
	glutMotionFunc(mouse_motion);
	glutKeyboardFunc(keyboard_func);
	glutMainLoop();
	return 0;
}



