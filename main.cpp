///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// test plane and intersect with a line
//
//  AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2016-01-20
// UPDATED: 2020-07-22
///////////////////////////////////////////////////////////////////////////////
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Matrices.h"
#include "Plane.h"
#include "Line.h"
#include "Cylinder.h"

#include "RT.h"
#include "Scene.h"

#include "Ray.h"
#include "Square.h"
#include "Cube.h"

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);
void specialCB(int key, int x, int y);

// CALLBACK function when exit() called ///////////////////////////////////////
void exitCB();

void initGL();
int initGLUT(int argc, char** argv);
bool initSharedMem();
void clearSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void drawString(const char* str, int x, int y, float color[4], void* font);
void drawString3D(const char* str, float pos[3], float color[4], void* font);
void showInfo();
void toOrtho();
void toPerspective();

void displayRT_CB();
int initSecondWindow(int argc, char** argv);


void drawPlane(const Plane& p, const Vector3& color);
void drawRay(const Line& line, const Vector3& color);
void drawLine(const Line& line, const Vector3& color);
void drawPoint(const Vector3& point, float scale, const Vector3& color);
void drawSquare(const Square& square, const Vector3& color);
void drawCube(const Cube& cube, const Vector3& color);

void animatePSCube(Cube& cube, float speedZ = 1, float speedRot = 1, float rangeZ = 0.5);

// constants
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 500;
const float CAMERA_DISTANCE = 25.0f;
const float CAMERA_ANGLE_X = 45.0f;
const float CAMERA_ANGLE_Y = -45.0f;
const int TEXT_WIDTH = 8;
const int TEXT_HEIGHT = 13;
const float DEG2RAD = acos(-1) / 180;

const int RAY_LEN = 20;

// global variables
void* font = GLUT_BITMAP_8_BY_13;
int screenWidth;
int screenHeight;
bool mouseLeftDown;
bool mouseRightDown;
float mouseX, mouseY;
float cameraAngleX;
float cameraAngleY;
float cameraX;
float cameraY;
float cameraDistance;
int drawMode = 0;
Matrix4 matrixView;
Plane plane1;
Plane plane2;
Line line;
Ray demoLine;
Ray ray;
Ray rays[10];
Ray cameraLook;
Vector3 color1;
Vector3 color2;
Vector3 color3;
Vector3 color4;
Cylinder cylinder; // to draw aline
Square sq, sqScreen;
Cube PSCube;

int image[RT_SCREEN_RES][RT_SCREEN_RES];

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// init global vars
	initSharedMem();

	// register exit callback
	atexit(exitCB);

	int window1 = initGLUT(argc, argv);
	//int window2 = initSecondWindow(argc, argv);

	// init  GL
	initGL();




	// Process events for the first window
	glutSetWindow(window1);

	// Process events for the second window
	//glutSetWindow(window2);

	// the last GLUT call (LOOP)
	// window will be shown and display callback is triggered by events
	// NOTE: this call never return main().
	glutMainLoop(); /* Start GLUT event-processing loop */


	return 0;
}

int initSecondWindow(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(600, 100);
	int handle = glutCreateWindow("RT");

	glutDisplayFunc(displayRT_CB); // Use the new display function for the second window

	return handle;
}

///////////////////////////////////////////////////////////////////////////////
// draw a plane
///////////////////////////////////////////////////////////////////////////////
void drawPlane(const Plane& plane, const Vector3& color)
{
	Vector3 n = plane.getNormal();
	float d = plane.getDistance();
	n.normalize();
	Vector3 t = n * d;
	// std::cout << n << std::endl;

	glPushMatrix();

	Matrix4 m1, m2;
	m1.lookAt(n);
	m1.translate(t);

	m2 = matrixView * m1;
	glLoadMatrixf(m2.get());

	glDisable(GL_CULL_FACE);
	glColor3f(color.x, color.y, color.z);
	glNormal3f(0, 0, 1);
	glBegin(GL_TRIANGLES);
	glVertex3f(-10, -10, 0);
	glVertex3f(10, -10, 0);
	glVertex3f(10, 10, 0);
	glVertex3f(10, 10, 0);
	glVertex3f(-10, 10, 0);
	glVertex3f(-10, -10, 0);
	glEnd();
	glEnable(GL_CULL_FACE);

	glPopMatrix();
}

void drawCube(const Cube& cube, const Vector3& color)
{
	static const float deltaTone = 0.15;
	//Vector3 deltaColor(deltaTone, deltaTone, deltaTone);
	FOR6i
	{
		drawSquare(cube.sides[i], color * (1 + (i - 2.5) * deltaTone));
	}
}

void animatePSCube(Cube& cube, float speedZ, float speedRot, float rangeZ)
{
	static float t = 0, prevZ = 0;

	Matrix4 m;
	float currZ = sin(DEG2RAD * t) * rangeZ / 2;
	m.translate(0, currZ - prevZ, 0);
	m.rotateY(speedRot);

	cube.transform(m);

	prevZ = currZ;
	t += speedZ;
}


void drawSquare(const Square& square, const Vector3& color)
{
	const Vector3* p = square.getCorners();

	glPushMatrix();

	glDisable(GL_CULL_FACE);
	glColor3f(color.x, color.y, color.z);
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glVertex3f(p[0].x, p[0].y, p[0].z);
	glVertex3f(p[1].x, p[1].y, p[1].z);
	glVertex3f(p[2].x, p[2].y, p[2].z);
	glVertex3f(p[3].x, p[3].y, p[3].z);
	glEnd();
	glEnable(GL_CULL_FACE);

	glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
// draw a line using cylinder
///////////////////////////////////////////////////////////////////////////////
void drawLine(const Line& line, const Vector3& color)
{
	// glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
	glDisable(GL_LIGHTING);

	// find the point on the line which closest from origin (0,0,0)
	// since the 3rd plane passes the origin, the shortest distance from
	// the line is actually the point of the line
	Vector3 v = line.getDirection();
	Vector3 p = line.getPoint();
	/*
	Vector3 v = line.getDirection();
	Vector3 p = line.getPoint();
	float d = p.dot(v);     // dot product
	float s = v.dot(v);     // squared
	float t = d / s;        // parameter t
	p += t * v;             // point intersected with perpendicular line
	std::cout << p << std::endl;
	*/

	// transform cylinder
	glPushMatrix();
	Matrix4 m, r;
	r.lookAt(v);
	m.scale(0.1f, 0.1f, 40);
	m = r * m; // apply lookat rotation
	m.translate(p);
	m = matrixView * m;
	glLoadMatrixf(m.get());

	// draw cylinder
	glColor3f(color.x, color.y, color.z);
	cylinder.draw();

	glPopMatrix();

	// restore default settings
	glEnable(GL_LIGHTING);
	// glDepthFunc(GL_LEQUAL);
}

void drawRay(const Line& line, const Vector3& color)
{
	glDisable(GL_LIGHTING);

	Vector3 v = line.getDirection();
	Vector3 p = line.getPoint();

	// transform cylinder
	//to move cylinder to begin in starting point
	v.normalize();
	v *= RAY_LEN;


	glPushMatrix();
	Matrix4 m, r;
	r.lookAt(v);
	m.scale(0.1f, 0.1f, 40);
	m = r * m; // apply lookat rotation
	m.translate(p + v);
	m = matrixView * m;
	glLoadMatrixf(m.get());

	// draw cylinder
	glColor3f(color.x, color.y, color.z);
	cylinder.draw();

	glPopMatrix();
	// restore default settings
	glEnable(GL_LIGHTING);
}

void drawPoint(const Vector3& point, float scale, const Vector3& color)
{
	glDisable(GL_LIGHTING);

	// transform cylinder

	glPushMatrix();
	Matrix4 m;
	m.scale(scale, scale, 2 * scale);
	m.translate(point);
	m = matrixView * m;
	glLoadMatrixf(m.get());

	// draw cylinder
	glColor3f(color.x, color.y, color.z);
	cylinder.draw();

	glPopMatrix();
	// restore default settings
	glEnable(GL_LIGHTING);

}

///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUT(int argc, char** argv)
{
	// GLUT stuff for windowing
	// initialization openGL window.
	// it is called before any other GLUT routine
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL); // display mode

	glutInitWindowSize(screenWidth, screenHeight); // window size

	glutInitWindowPosition(100, 100); // window location

	// finally, create a window with openGL context
	// Window will not displayed until glutMainLoop() is called
	// it returns a unique ID
	int handle = glutCreateWindow(argv[0]); // param is the title of window

	// register GLUT callback functions
	glutDisplayFunc(displayCB);
	glutTimerFunc(33, timerCB, 33); // redraw only every given millisec
	// glutIdleFunc(idleCB);                       // redraw whenever system is idle
	glutReshapeFunc(reshapeCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutSpecialFunc(specialCB);

	return handle;
}

///////////////////////////////////////////////////////////////////////////////
// initialize OpenGL
// disable unused features
///////////////////////////////////////////////////////////////////////////////
void initGL()
{
	glShadeModel(GL_SMOOTH);               // shading mathod: GL_SMOOTH or GL_FLAT
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // 4-byte pixel alignment

	// enable /disable features
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);

	// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(0, 0, 0, 0); // background color
	glClearStencil(0);        // clear stencil buffer
	glClearDepth(1.0f);       // 0 is near, 1 is far
	glDepthFunc(GL_LEQUAL);

	initLights();
}

///////////////////////////////////////////////////////////////////////////////
// write 2d text using GLUT
// The projection matrix must be set to orthogonal before call this function.
///////////////////////////////////////////////////////////////////////////////
void drawString(const char* str, int x, int y, float color[4], void* font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);                         // need to disable lighting for proper text color
	glDisable(GL_TEXTURE_2D);

	glColor4fv(color);   // set text color
	glRasterPos2i(x, y); // place text position

	// loop all characters in the string
	while (*str)
	{
		glutBitmapCharacter(font, *str);
		++str;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

///////////////////////////////////////////////////////////////////////////////
// draw a string in 3D space
///////////////////////////////////////////////////////////////////////////////
void drawString3D(const char* str, float pos[3], float color[4], void* font)
{
	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT); // lighting and color mask
	glDisable(GL_LIGHTING);                         // need to disable lighting for proper text color
	glDisable(GL_TEXTURE_2D);

	glColor4fv(color);   // set text color
	glRasterPos3fv(pos); // place text position

	// loop all characters in the string
	while (*str)
	{
		glutBitmapCharacter(font, *str);
		++str;
	}

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopAttrib();
}

///////////////////////////////////////////////////////////////////////////////
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
	Matrix4 mc;
	std::cout << "===== RT Tests by Piotr Sicinski =====" << std::endl;

	plane1.set(2, 3, 1, 12);


	demoLine.set(Vector3(1, 1, 1), Vector3(0, 0, 0));
	ray.set(Vector3(1, 1, 1), Vector3(0, 0, 0));

	PSCube = Cube(3);
	mc.rotateX(45);
	mc.rotateZ(atan(1 / sqrt(2)) / DEG2RAD); // cant rotate by 45 and 45, becasue than top is no the top
	PSCube.reset();
	PSCube.transform(mc);

	// plane and line colours
	color1.set(0.8f, 0.9f, 0.8f); // plane1
	color2.set(0.8f, 0.8f, 0.9f); // plane2
	color3.set(1.0f, 0.5f, 0.0f); // line
	color4.set(1.0f, 0.0f, 1.0f); // demoLine

	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;

	mouseLeftDown = mouseRightDown = false;
	mouseX = mouseY = 0;

	cameraX = cameraY = 0;
	cameraAngleX = CAMERA_ANGLE_X;
	cameraAngleY = CAMERA_ANGLE_Y;
	cameraDistance = CAMERA_DISTANCE;

	drawMode = 0; // 0:fill, 1: wireframe, 2:points

	//RT
	fillPattern(image);

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// clean up shared memory
///////////////////////////////////////////////////////////////////////////////
void clearSharedMem()
{
}

///////////////////////////////////////////////////////////////////////////////
// initialize lights
///////////////////////////////////////////////////////////////////////////////
void initLights()
{
	// set up light colors (ambient, diffuse, specular)
	GLfloat lightKa[] = { .2f, .2f, .2f, 1.0f }; // ambient light
	GLfloat lightKd[] = { .7f, .7f, .7f, 1.0f }; // diffuse light
	GLfloat lightKs[] = { 1, 1, 1, 1 };          // specular light
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

	// position the light
	float lightPos[4] = { 0, 0, 20, 1 }; // positional light
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	glEnable(GL_LIGHT0); // MUST enable each light source after configuration
}

///////////////////////////////////////////////////////////////////////////////
// set camera position and lookat direction
///////////////////////////////////////////////////////////////////////////////
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, targetX, targetY, targetZ, 0, 1, 0); // eye(x,y,z), focal(x,y,z), up(x,y,z)
}

///////////////////////////////////////////////////////////////////////////////
// display info messages
///////////////////////////////////////////////////////////////////////////////
void showInfo()
{
	// backup current model-view matrix
	glPushMatrix();   // save current modelview matrix
	glLoadIdentity(); // reset modelview matrix

	// set to 2D orthogonal projection
	glMatrixMode(GL_PROJECTION);                 // switch to projection matrix
	glPushMatrix();                              // save current projection matrix
	glLoadIdentity();                            // reset projection matrix
	gluOrtho2D(0, screenWidth, 0, screenHeight); // set to orthogonal projection

	float color[4] = { 1, 1, 1, 1 };

	std::stringstream ss;
	ss << std::fixed << std::setprecision(3);

	// print here

	// unset floating format
	ss << std::resetiosflags(std::ios_base::fixed | std::ios_base::floatfield);

	// restore projection matrix
	glPopMatrix(); // restore to previous projection matrix

	// restore modelview matrix
	glMatrixMode(GL_MODELVIEW); // switch to modelview matrix
	glPopMatrix();              // restore to previous modelview matrix
}

///////////////////////////////////////////////////////////////////////////////
// set projection matrix as orthogonal
///////////////////////////////////////////////////////////////////////////////
void toOrtho()
{
	// set viewport to be the entire window
	glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

	// set orthographic viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, screenWidth, 0, screenHeight, -1, 1);

	// switch to modelview matrix in order to set scene
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

///////////////////////////////////////////////////////////////////////////////
// set the projection matrix as perspective
///////////////////////////////////////////////////////////////////////////////
void toPerspective()
{
	// set viewport to be the entire window
	glViewport(0, 0, (GLsizei)screenWidth, (GLsizei)screenHeight);

	// set perspective viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, (float)(screenWidth) / screenHeight, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

	// switch to modelview matrix in order to set scene
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//=============================================================================
// CALLBACKS
//=============================================================================

void displayCB()
{
	Matrix4 m1, m2, m3, m4, m5, mc;
	static int deg = 0;

	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	// save the initial ModelView matrix before modifying ModelView matrix
	glPushMatrix();
	// tramsform camera
	matrixView.identity();
	matrixView.rotateY(cameraAngleY);
	matrixView.rotateX(cameraAngleX);
	matrixView.translate(0, 0, -cameraDistance);
	glLoadMatrixf(matrixView.get());

	drawRoom(20);
	drawAxis(20);

	if (deg % 100 == 0)
	{
		//Square sqMig;
		//sqMig.move(Matrix4().scale(10));
		//drawSquare(sqMig, Vector3(1, 0, 0));

		std::cout << "=====  =====" << std::endl;
		std::cout << "cameraAngleX\t" << cameraAngleX << "\ncameraAngleY\t" << cameraAngleY << std::endl;
		matrixView.printSelf();

		m3.translate(0, 0, cameraDistance - 2);
		m3.rotateX(-cameraAngleX);
		m3.rotateY(-cameraAngleY);
		m3 = matrixView.invert();
		cameraLook.setDirection(m3.getRotationMatrix() * Vector3(0, 0, 1));


		//m5.scale()
		sqScreen.reset();
		sqScreen.transform(m3);
	}
	//drawRay(cameraLook, color3);
	//drawSquare(sqScreen, Vector3(0.7, 1, 1));

	//drawPlane(plane1, color1);
	//drawPoint(Vector3(3, 3, 3), 0.2f, color3);



	////m4.scale(5);// .rotateZ(30).rotateX(30).rotateY(90);
	////m4.rotateZ(30);
	////m4.rotateX(-30);
	////m4.rotateY(90);
	////m4.translate(-5, 1, 1.8);
	////sq.reset();
	////sq.transform(m4);
	////drawSquare(sq, Vector3(0.6, 0, 0));

	//////rays[1].set(Vector3(0, 0, -1), Vector3(0.5, 0.5, 5));
	//////drawRay(rays[1], Vector3(0, 0.4, 0));

	////m1.rotateX(deg);
	////demoLine.setDirection(m1 * Vector3(-1, 0, -0.4));
	////drawRay(demoLine, color4);

	//////Vector3 point = sq.intersect(demoLine);
	//////if (point != NAN_VECTOR3)
	//////{
	//////	drawPoint(point, 0.15f, color3);
	//////}

	////ray = sq.reflect(demoLine);

	////if (ray.getPoint() != NAN_VECTOR3)
	////{
	////	drawPoint(ray.getPoint(), 0.15f, color3);
	////	drawRay(ray, color4 * 0.75f);
	////}


	animatePSCube(PSCube, 2.4, 1.5, 1);
	drawCube(PSCube, Vector3(0.5, 0.5, 0.5));


	demoLine.set(Vector3(-1, 0.6, -1), Vector3(5, 0, 5));
	drawRay(demoLine, color4);

	ray = PSCube.reflect(demoLine);

	if (ray.getPoint() != NAN_VECTOR3)
	{
		drawPoint(ray.getPoint(), 0.15f, color3);
		drawRay(ray, color4 * 0.75f);
	}



	deg++;
	showInfo();
	glPopMatrix();
	glutSwapBuffers();
}

void reshapeCB(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	toPerspective();
}

void timerCB(int millisec)
{
	glutTimerFunc(millisec, timerCB, millisec);
	glutPostRedisplay();
}

void idleCB()
{
	glutPostRedisplay();
}

void keyboardCB(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27: // ESCAPE
		exit(0);
		break;

	case ' ':
		break;

	case 'd': // switch rendering modes (fill -> wire -> point)
	case 'D':
		drawMode = ++drawMode % 3;
		if (drawMode == 0) // fill mode
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_CULL_FACE);
		}
		else if (drawMode == 1) // wireframe mode
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
		else // point mode
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glDisable(GL_DEPTH_TEST);
			glDisable(GL_CULL_FACE);
		}
		break;

	default:;
	}
}

void mouseCB(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;

	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseLeftDown = true;
		}
		else if (state == GLUT_UP)
			mouseLeftDown = false;
	}

	else if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_DOWN)
		{
			mouseRightDown = true;
		}
		else if (state == GLUT_UP)
			mouseRightDown = false;
	}
}

void mouseMotionCB(int x, int y)
{
	if (mouseLeftDown)
	{
		cameraAngleY += (x - mouseX);
		cameraAngleX += (y - mouseY);
		mouseX = x;
		mouseY = y;
	}
	if (mouseRightDown)
	{
		cameraDistance -= (y - mouseY) * 0.2f;
		mouseY = y;
	}
}

void specialCB(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		cameraX -= 1;
		break;

	case GLUT_KEY_RIGHT:
		cameraX += 1;
		break;

	case GLUT_KEY_UP:
		cameraY += 1;
		break;

	case GLUT_KEY_DOWN:
		cameraY -= 1;
		break;

	default:;
	}
}

void exitCB()
{
	clearSharedMem();
}


void displayRT_CB()
{
	toOrtho();

	// Draw your image table here
	glBegin(GL_POINTS);
	for (int row = 0; row < RT_SCREEN_RES; row++)
	{
		for (int col = 0; col < RT_SCREEN_RES; col++)
		{
			int value = image[row][col];
			float color = value / static_cast<float>((COLOR_DEPTH - 1));
			glColor3f(color, color, color);
			glVertex2f(col, row);
		}
	}
	glEnd();

	glutSwapBuffers();
}
