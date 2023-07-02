#ifndef SCENE_H
#define SCENE_H

#include <GL/glut.h>

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Matrices.h"
#include "Plane.h"
#include "Line.h"
#include "Cylinder.h"

#include "Ray.h"
#include "Square.h"
#include "Cube.h"

void drawAxis(float size = 10);
void drawRoom(float size = 10);
void drawGrid(float size = 10.0f, float step = 1.0f);
void drawPlane(const Plane& p, const Vector3& color);
void drawRay(const Line& line, const Vector3& color);
void drawLine(const Line& line, const Vector3& color);
void drawPoint(const Vector3& point, float scale, const Vector3& color);
void drawSquare(const Square& square, const Vector3& color);
void drawCube(const Cube& cube, const Vector3& color);


#endif