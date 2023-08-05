///////////////////////////////////////////////////////////////////////////////
// main.cpp
// ========
// test plane and intersect with a line
//
// AUTHOR: Song Ho Ahn (song.ahn@gmail.com)
// CREATED: 2016-01-20
// UPDATED: 2020-07-22
///////////////////////////////////////////////////////////////////////////////
#include <GL/glut.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Matrices.h"
#include "Plane.h"
#include "Line.h"
#include "Cylinder.h"

#include "Common.h"
#include "RT.h"
#include "DrawGL.h"

#include "Ray.h"
#include "Square.h"
#include "Cube.h"

#include "Render.h"

// GLUT CALLBACK functions
void displayCB();
void reshapeCB(int w, int h);
void reshapeRTCB(int w, int h);
void timerCB(int millisec);
void idleCB();
void keyboardCB(unsigned char key, int x, int y);
void mouseCB(int button, int stat, int x, int y);
void mouseMotionCB(int x, int y);
void specialCB(int key, int x, int y);

void displayRT_CB();

// CALLBACK function when exit() called
void exitCB();

void initGL();
int initGLUTWindow(int argc, char** argv);
bool initSharedMem();
void initLights();
void setCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ);
void toOrtho();
void toPerspective();
void toPerspectiveRT();
int initRTWindow(int argc, char** argv);

void animatePSCube(Cube& cube, float speedZ = 1, float speedRot = 1, float rangeZ = 0.5);


// global variables
int screenWidth;
int screenHeight;
int screenWidthRT;
int screenHeightRT;
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
Square sq, sqScreen;
Cube PSCube;

Render render;
SceneObject* objects[MAX_OBJ_COUNT];
int objN = 0;

int deg = 0;

Ray demoCasted[DEMO_CASTED_RAYS_COUNT];
int demoCastedN = 0;

int windowGLUT, windowRT;
bool windowRTReady;
uint8_t image[RT_RENDER_RES][RT_RENDER_RES];
uint8_t texture[RT_RENDER_RES * RT_RENDER_RES * 3];

///////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
	// init global vars
	initSharedMem();

	// register exit callback
	atexit(exitCB);

	glutInit(&argc, argv);

	windowGLUT = initGLUTWindow(argc, argv);
	initGL();

	windowRT = initRTWindow(argc, argv);

	//glutSetWindow(windowGLUT);
	//glutSetWindow(windowRT);

	// window will be shown and display callback is triggered by events
	glutMainLoop(); /* Start GLUT event-processing loop */
	return 0;
}

int initRTWindow(int argc, char** argv)
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(600, 100);
	int handle = glutCreateWindow("RT");

	glutDisplayFunc(displayRT_CB); // Use the new display function for the second window
	//glutTimerFunc(33, timerCB, 33); // redraw only every given millisec
	//// glutIdleFunc(idleCB);                       // redraw whenever system is idle
	glutReshapeFunc(reshapeRTCB);
	glutKeyboardFunc(keyboardCB);
	glutMouseFunc(mouseCB);
	glutMotionFunc(mouseMotionCB);
	glutSpecialFunc(specialCB);

	return handle;
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

///////////////////////////////////////////////////////////////////////////////
// initialize GLUT for windowing
///////////////////////////////////////////////////////////////////////////////
int initGLUTWindow(int argc, char** argv)
{
	// GLUT stuff for windowing
	// initialization openGL window.
	// it is called before any other GLUT routine

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
// initialize global variables
///////////////////////////////////////////////////////////////////////////////
bool initSharedMem()
{
	Matrix4 mc;
	std::cout << "===== RT Tests by Piotr Sicinski =====" << std::endl;

	//demonstration animation
	plane1.set(2, 3, 1, 12);
	demoLine.set(Vector3(1, 1, 1), Vector3(0, 0, 0));
	ray.set(Vector3(1, 1, 1), Vector3(0, 0, 0));

	//PSCube
	PSCube = Cube(3);
	mc.rotateX(45);
	mc.rotateZ(atan(1 / sqrt(2)) / DEG2RAD); // cant rotate by 45 and 45, becasue than top is no the top
	PSCube.reset();
	PSCube.transform(mc);

	objN++;
	objects[0] = &PSCube;

	render = Render();

	// plane and line colours
	color1.set(0.8f, 0.9f, 0.8f); // plane1
	color2.set(0.8f, 0.8f, 0.9f); // plane2
	color3.set(1.0f, 0.5f, 0.0f); // line
	color4.set(1.0f, 0.0f, 1.0f); // demoLine

	//screen
	screenWidth = SCREEN_WIDTH;
	screenHeight = SCREEN_HEIGHT;

	screenWidthRT = RT_RENDER_RES;
	screenHeightRT = RT_RENDER_RES;

	mouseLeftDown = mouseRightDown = false;
	mouseX = mouseY = 0;

	cameraX = cameraY = 0;
	cameraAngleX = CAMERA_ANGLE_X;
	cameraAngleY = CAMERA_ANGLE_Y;
	cameraDistance = CAMERA_DISTANCE;

	drawMode = 0; // 0:fill, 1: wireframe, 2:points

	//RT
	fillPattern(image);
	fillTexture(image, texture);

	return true;
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

void toPerspectiveRT()
{
	// set viewport to be the entire window
	glViewport(0, 0, (GLsizei)screenWidthRT, (GLsizei)screenHeightRT);

	// set perspective viewing frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOV, (float)(screenWidthRT) / screenHeightRT, 1.0f, 1000.0f); // FOV, AspectRatio, NearClip, FarClip

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

	if (deg % 100 == 20)
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
		//m3 = matrixView.invert();
		cameraLook.setDirection(m3.getRotationMatrix() * Vector3(0, 0, -1));
		cameraLook.setPoint(m3 * Vector3());

		sqScreen.reset();
		sqScreen.transform(m3);


	}
	drawRay(demoCasted[0], color3);
	for (size_t i = 1; i < DEMO_CASTED_RAYS_COUNT; i++)
	{
		drawRay(demoCasted[i], Vector3(0.7, 1, 1));

		ray = PSCube.reflect(demoCasted[i]);

		if (ray.getPoint() != NAN_VECTOR3)
		{
			drawPoint(ray.getPoint(), 0.15f, color3);
			drawRay(ray, color4 * 0.75f);
		}
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


	//demoLine.set(Vector3(-1, 0.5, -1), Vector3(5, 0, 5));
	//drawRay(demoLine, color4);

	//ray = PSCube.reflect(demoLine);

	//if (ray.getPoint() != NAN_VECTOR3)
	//{
	//	drawPoint(ray.getPoint(), 0.15f, color3);
	//	drawRay(ray, color4 * 0.75f);
	//}


	glPopMatrix();
	glutSwapBuffers();
}

void reshapeCB(int w, int h)
{
	//screenWidth = w;
	//screenHeight = h;
	int size = (w < h) ? w : h;
	screenWidth = screenHeight = size;

	// Set the viewport to the updated dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	toPerspective();
}

void reshapeRTCB(int w, int h)
{
	//screenWidth = w;
	//screenHeight = h;
	int size = (w < h) ? w : h;
	screenWidthRT = screenHeightRT = size;

	// Set the viewport to the updated dimensions
	glViewport(0, 0, screenWidthRT, screenHeightRT);

	//toPerspectiveRT();
}

void timerCB(int millisec)
{
	static bool ready = true;
	glutTimerFunc(millisec, timerCB, millisec);
	//	glutPostRedisplay();

	if (!ready)
	{
		return;
	}
	ready = false;
	glutSetWindow(windowGLUT);
	glutPostRedisplay();

	glutSetWindow(windowRT);
	glutPostRedisplay();
	deg++;
	ready = true;
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
	//clearSharedMem();
}


void displayRT_CB()
{
	render.renderFrame();
	fillTexture(render.ready.image, texture);



	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, screenWidth, 0, screenHeight);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Enable texture mapping
	glEnable(GL_TEXTURE_2D);

	// Generate a new texture ID
	GLuint textureID;
	glGenTextures(1, &textureID);

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Set texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load the image data to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, RT_RENDER_RES, RT_RENDER_RES, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

	// Generate texture coordinates automatically
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	// Draw a quad with the image texture
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex2f(0, 0);
	glTexCoord2f(1, 0); glVertex2f(RT_RENDER_RES, 0);
	glTexCoord2f(1, 1); glVertex2f(RT_RENDER_RES, RT_RENDER_RES);
	glTexCoord2f(0, 1); glVertex2f(0, RT_RENDER_RES);
	glEnd();

	glDisable(GL_TEXTURE_2D);

	// Delete the texture
	glDeleteTextures(1, &textureID);

	glutSwapBuffers();
}
