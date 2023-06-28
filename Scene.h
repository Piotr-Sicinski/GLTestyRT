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

void drawAxis(float size = 10);
void drawRoom(float size = 10);
void drawGrid(float size = 10.0f, float step = 1.0f);


#endif