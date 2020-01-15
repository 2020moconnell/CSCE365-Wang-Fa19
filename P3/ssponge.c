#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h> 
#include <GL/gl.h>
#include <vector>

GLfloat depth = 1;
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
bool plus = false;
bool minus = false;


//=================================================================================
//=================================================================================
class point
{
public:
	point(GLfloat x, GLfloat y, GLfloat z, point* ref = NULL);
	void visualize();

	GLfloat x, y, z;
};

point::point(GLfloat x, GLfloat y, GLfloat z, point* ref)
{
	this->x = x;
	this->y = y;
	this->z = z;

	if (ref != NULL)
	{
		this->x += ref->x;
		this->y += ref->y;
		this->z += ref->z;
	}
}

//=================================================================================
//=================================================================================

class Sponge
{
public:
	Sponge(point* origin, GLfloat edgelength, GLfloat depth);
	~Sponge();

	void visualize();

private:
	void MakeFace(int i, int j, int k, int l);
	void Group(point* p, GLfloat e, GLfloat d);
	void ActAsCube(point* p, GLfloat e);

	point** Points;
	Sponge** SubCubes;
};

Sponge::Sponge(point* origin, GLfloat edgelength, GLfloat depth)
{
	if (depth <= 1.0)
	{
		ActAsCube(origin, edgelength);
	}
	else {
		Group(origin, edgelength, depth);
	}
}

Sponge::~Sponge()
{
	int i;

	if (Points != NULL)
	{
		for (i = 0; i < 8; i++)
			delete Points[i];
		delete[] Points;
	}

	if (SubCubes != NULL)
	{
		for (i = 0; i < 20; i++)
			delete SubCubes[i];
		delete[] SubCubes;
	}
}

void Sponge::ActAsCube(point * p, GLfloat e)
{
	GLfloat ne = e / 2.0;

	Points = new point * [8];
	SubCubes = NULL;

	Points[0] = new point(ne, ne, ne, p);     
	Points[1] = new point(ne, -ne, ne, p);     
	Points[2] = new point(-ne, ne, ne, p);     
	Points[3] = new point(-ne, -ne, ne, p);     
	Points[4] = new point(ne, ne, -ne, p);     
	Points[5] = new point(ne, -ne, -ne, p);     
	Points[6] = new point(-ne, ne, -ne, p);    
	Points[7] = new point(-ne, -ne, -ne, p);    
}

void Sponge::Group(point * p, GLfloat e, GLfloat d)
{
	GLfloat ne = e / 3.0;

	SubCubes = new Sponge * [20]; 
	Points = NULL;

	SubCubes[0] = new Sponge(new point(-ne, ne, ne, p), ne, d - 1.0);
	SubCubes[1] = new Sponge(new point(0.0, ne, ne, p), ne, d - 1.0);
	SubCubes[2] = new Sponge(new point(ne, ne, ne, p), ne, d - 1.0);
	SubCubes[3] = new Sponge(new point(ne, 0.0, ne, p), ne, d - 1.0);
	SubCubes[4] = new Sponge(new point(ne, -ne, ne, p), ne, d - 1.0);
	SubCubes[5] = new Sponge(new point(0.0, -ne, ne, p), ne, d - 1.0);
	SubCubes[6] = new Sponge(new point(-ne, -ne, ne, p), ne, d - 1.0);
	SubCubes[7] = new Sponge(new point(-ne, 0.0, ne, p), ne, d - 1.0);

	SubCubes[8] = new Sponge(new point(ne, ne, 0.0, p), ne, d - 1.0);
	SubCubes[9] = new Sponge(new point(ne, -ne, 0.0, p), ne, d - 1.0);
	SubCubes[10] = new Sponge(new point(-ne, ne, 0.0, p), ne, d - 1.0);
	SubCubes[11] = new Sponge(new point(-ne, -ne, 0.0, p), ne, d - 1.0);

	SubCubes[12] = new Sponge(new point(-ne, ne, -ne, p), ne, d - 1.0);
	SubCubes[13] = new Sponge(new point(0.0, ne, -ne, p), ne, d - 1.0);
	SubCubes[14] = new Sponge(new point(ne, ne, -ne, p), ne, d - 1.0);
	SubCubes[15] = new Sponge(new point(ne, 0.0, -ne, p), ne, d - 1.0);
	SubCubes[16] = new Sponge(new point(ne, -ne, -ne, p), ne, d - 1.0);
	SubCubes[17] = new Sponge(new point(0.0, -ne, -ne, p), ne, d - 1.0);
	SubCubes[18] = new Sponge(new point(-ne, -ne, -ne, p), ne, d - 1.0);
	SubCubes[19] = new Sponge(new point(-ne, 0.0, -ne, p), ne, d - 1.0);
}

void Sponge::MakeFace(int i, int j, int k, int l)
{
	glVertex3f(Points[i]->x, Points[i]->y, Points[i]->z);
	glVertex3f(Points[j]->x, Points[j]->y, Points[j]->z);
	glVertex3f(Points[k]->x, Points[k]->y, Points[k]->z);
	glVertex3f(Points[l]->x, Points[l]->y, Points[l]->z);

}

void Sponge::visualize()
{
	int i;

	if (Points != NULL)
	{
		glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0);// top
		MakeFace(0, 2, 3, 1);
		glColor3f(1.0, 0,0);//bottom
		MakeFace(4, 6, 7, 5);

		glColor3f(.8,0,0);// north
		MakeFace(0, 2, 6, 4);
		glColor3f(.8,0,0);// south
		MakeFace(1, 3, 7, 5);

		glColor3f(.6,0,0);//east
		MakeFace(0, 4, 5, 1);
		glColor3f(.6,0,0);// west
		MakeFace(2, 6, 7, 3);
		glEnd();
	}

	if (SubCubes != NULL)
	{
		for (i = 0; i < 20; i++)
		{
			SubCubes[i]->visualize();
		}
	}

}

Sponge* S_Sponge;

void idle()
{
	glutPostRedisplay();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GLfloat poly_ambient[] = { 0.0, 0.1, 0.06, 1.0 };
	GLfloat poly_diffusive[] = { 0.0, 0.50980392, 0.50980392, 1.0 };
	GLfloat poly_specular[] = { 0.714, 0.4284, 0.18144, 1.0 };
	GLfloat poly_shininess[] = { 0.6 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, poly_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, poly_diffusive);
	glMaterialfv(GL_FRONT, GL_SPECULAR, poly_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, poly_shininess);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1.0, 1.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glRotatef(angle_x1, 0, 1, 0);
	glRotatef(angle_y1, 1, 0, 0);

	S_Sponge->visualize(); 

	glutSwapBuffers();
}


void rotate(int x, int y) {
	//borrowed from ex
	GLfloat angle_x = (360.0 * (-1 * x + prev_x)) / theta_calc_denom;
	GLfloat angle_y = (360.0 * (-1 * y + prev_y)) / theta_calc_denom;
	if (left_mouse_state == 0) {
		angle_x1 = angle_x;
		angle_y1 = angle_y;
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glutPostRedisplay();
}

void mouse_func(int button, int state, int x, int y) { //keeps track of the status of the mouse buttons
	prev_x = x;
	prev_y = y;
	if (button == GLUT_LEFT_BUTTON) {
		left_mouse_state = state;
	}
	if (button == GLUT_RIGHT_BUTTON) {
		right_mouse_state = state;
	}
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-8.0, 8.0, -8.0, 8.0, -8.0, 8.0);
}

void init()
{
	theta_calc_denom = sqrtf(WIDTH * WIDTH + HEIGHT * HEIGHT);
	glShadeModel(GL_SMOOTH);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glColor3f(1.0, 1.0, 1.0);

	GLfloat light_position1[] = { .0, 3.0, -3.0, 1.0 };
	GLfloat light_position2[] = { .0, -3.0, 3.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat local_view[] = { 0.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position2);
	glLightfv(GL_LIGHT0, GL_AMBIENT, mat_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, mat_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
}

GLfloat getDepth(char* depth)
{
	int k = atoi(depth);
	if (k <= 1) return 1.0;
	else if (k >= 5) return 5.0;
	else return (GLfloat)k;
}

void keyboard_func(unsigned char key, int x, int y) {
	//borrowed from ex
	point origin(0.0, 0.0, 0.0);
	if (key == '+' || key == '=') {
		depth++;
		plus = true;
		S_Sponge = new Sponge(&origin, 8.0, depth);
		printf("depth = %lf \n", depth - 1);
		glutPostRedisplay();
	}
	if (key == '-') {
		depth--;
		minus = false;
		S_Sponge = new Sponge(&origin, 8.0, depth -1);
		printf("depth = %lf \n", depth);
		glutPostRedisplay();
	}
	if (key == 'l' || key == 'L' || key == 'r' || key == 'R') {
		angle_x1 = 0;
		angle_y1 = 0;
		glClear(GL_COLOR_BUFFER_BIT);
		glutPostRedisplay();
	}
}

void mouse_motion(int x, int y) { //mouseMotion

	rotate(x, y);
}

int main(int argc, char* argv[])
{
	//GLfloat depth;
	bool viewAsPointCloud = false;
	point origin(0.0, 0.0, 0.0);

	S_Sponge = new Sponge(&origin, 8.0, depth);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
	glEnable(GL_DEPTH_TEST);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(*argv);
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutMouseFunc(mouse_func);
	glutMotionFunc(mouse_motion);
	glutKeyboardFunc(keyboard_func);
	glutIdleFunc(idle);
	init(); 
	glutMainLoop();	

	delete S_Sponge;
}
