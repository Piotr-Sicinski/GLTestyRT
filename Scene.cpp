#include "Scene.h"

Cylinder cylinder; // to draw aline

extern Matrix4 matrixView;

///////////////////////////////////////////////////////////////////////////////
// draw a grid on XZ-plane
///////////////////////////////////////////////////////////////////////////////
void drawGrid(float size, float step)
{
	// disable lighting
	glDisable(GL_LIGHTING);

	// 20x20 grid
	glBegin(GL_LINES);

	glColor3f(0.5f, 0.5f, 0.5f);
	for (float i = step; i <= size; i += step)
	{
		glVertex3f(-size, 0, i); // lines parallel to X-axis
		glVertex3f(size, 0, i);
		glVertex3f(-size, 0, -i); // lines parallel to X-axis
		glVertex3f(size, 0, -i);

		glVertex3f(i, 0, -size); // lines parallel to Z-axis
		glVertex3f(i, 0, size);
		glVertex3f(-i, 0, -size); // lines parallel to Z-axis
		glVertex3f(-i, 0, size);
	}

	// x-axis
	glColor3f(1, 0, 0);
	glVertex3f(-size, 0, 0);
	glVertex3f(size, 0, 0);

	// z-axis
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -size);
	glVertex3f(0, 0, size);

	glEnd();

	// enable lighting back
	glEnable(GL_LIGHTING);
}

///////////////////////////////////////////////////////////////////////////////
// draw axis
///////////////////////////////////////////////////////////////////////////////
void drawAxis(float size)
{
	// glDepthFunc(GL_ALWAYS);     // to avoid visual artifacts with grid lines
	glDisable(GL_LIGHTING);

	float s = size * 0.5f;

	// draw axis
	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(s, 0, 0);
	// glColor3f(0.5f, 0, 0);
	// glVertex3f(0, 0, 0);
	// glVertex3f(-s, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, s, 0);
	// glColor3f(0, 0.5f, 0);
	// glVertex3f(0, 0, 0);
	// glVertex3f(0, -s, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, s);
	// glColor3f(0, 0, 0.5f);
	// glVertex3f(0, 0, 0);
	// glVertex3f(0, 0, -s);
	glEnd();
	glLineWidth(1);

	// restore default settings
	glEnable(GL_LIGHTING);
	// glDepthFunc(GL_LEQUAL);
}

///////////////////////////////////////////////////////////////////////////////
// draw a square room
///////////////////////////////////////////////////////////////////////////////
void drawRoom(float size)
{
	float d = 0.02f;
	float s = size * 0.5f;

	glColor4f(0.3f, 0.3f, 0.3f, 1.0f);
	glBegin(GL_TRIANGLES);
	for (int i = -(int)s; i <= (int)s; ++i)
	{
		if (i == 0)
			continue;

		// -X
		glNormal3f(1, 0, 0);
		glVertex3f(-s, i - d, -s); // horizontal
		glVertex3f(-s, i + d, -s);
		glVertex3f(-s, i + d, s);
		glVertex3f(-s, i + d, s);
		glVertex3f(-s, i - d, s);
		glVertex3f(-s, i - d, -s);

		glVertex3f(-s, -s, i - d); // vertical
		glVertex3f(-s, s, i - d);
		glVertex3f(-s, s, i + d);
		glVertex3f(-s, s, i + d);
		glVertex3f(-s, -s, i + d);
		glVertex3f(-s, -s, i - d);

		// +X
		glNormal3f(-1, 0, 0);
		glVertex3f(s, i - d, -s);
		glVertex3f(s, i - d, s);
		glVertex3f(s, i + d, s);
		glVertex3f(s, i + d, s);
		glVertex3f(s, i + d, -s);
		glVertex3f(s, i - d, -s);

		glVertex3f(s, -s, i - d);
		glVertex3f(s, -s, i + d);
		glVertex3f(s, s, i + d);
		glVertex3f(s, s, i + d);
		glVertex3f(s, s, i - d);
		glVertex3f(s, -s, i - d);

		// -Y
		glNormal3f(0, 1, 0);
		glVertex3f(-s, -s, i - d);
		glVertex3f(-s, -s, i + d);
		glVertex3f(s, -s, i + d);
		glVertex3f(s, -s, i + d);
		glVertex3f(s, -s, i - d);
		glVertex3f(-s, -s, i - d);

		glVertex3f(i - d, -s, -s);
		glVertex3f(i - d, -s, s);
		glVertex3f(i + d, -s, s);
		glVertex3f(i + d, -s, s);
		glVertex3f(i + d, -s, -s);
		glVertex3f(i - d, -s, -s);

		// +Y
		glNormal3f(0, -1, 0);
		glVertex3f(-s, s, i - d);
		glVertex3f(s, s, i - d);
		glVertex3f(s, s, i + d);
		glVertex3f(s, s, i + d);
		glVertex3f(-s, s, i + d);
		glVertex3f(-s, s, i - d);

		glVertex3f(i - d, s, -s);
		glVertex3f(i + d, s, -s);
		glVertex3f(i + d, s, s);
		glVertex3f(i + d, s, s);
		glVertex3f(i - d, s, s);
		glVertex3f(i - d, s, -s);

		// -Z
		glNormal3f(0, 0, 1);
		glVertex3f(-s, i - d, -s);
		glVertex3f(s, i - d, -s);
		glVertex3f(s, i + d, -s);
		glVertex3f(s, i + d, -s);
		glVertex3f(-s, i + d, -s);
		glVertex3f(-s, i - d, -s);

		glVertex3f(i - d, -s, -s);
		glVertex3f(i + d, -s, -s);
		glVertex3f(i + d, s, -s);
		glVertex3f(i + d, s, -s);
		glVertex3f(i - d, s, -s);
		glVertex3f(i - d, -s, -s);

		// +Z
		glNormal3f(0, 0, -1);
		glVertex3f(-s, i - d, s);
		glVertex3f(-s, i + d, s);
		glVertex3f(s, i + d, s);
		glVertex3f(s, i + d, s);
		glVertex3f(s, i - d, s);
		glVertex3f(-s, i - d, s);

		glVertex3f(i - d, -s, s);
		glVertex3f(i - d, s, s);
		glVertex3f(i + d, s, s);
		glVertex3f(i + d, s, s);
		glVertex3f(i + d, -s, s);
		glVertex3f(i - d, -s, s);
	}

	d = 0.04f;
	glColor4f(0.5f, 0.5f, 0.5f, 1.0f);
	glNormal3f(1, 0, 0);
	glVertex3f(-s, -d, -s);
	glVertex3f(-s, d, -s);
	glVertex3f(-s, d, s);
	glVertex3f(-s, d, s);
	glVertex3f(-s, -d, s);
	glVertex3f(-s, -d, -s);

	glVertex3f(-s, -s, -d);
	glVertex3f(-s, s, -d);
	glVertex3f(-s, s, d);
	glVertex3f(-s, s, d);
	glVertex3f(-s, -s, d);
	glVertex3f(-s, -s, -d);

	glNormal3f(-1, 0, 0);
	glVertex3f(s, -d, -s);
	glVertex3f(s, -d, s);
	glVertex3f(s, d, s);
	glVertex3f(s, d, s);
	glVertex3f(s, d, -s);
	glVertex3f(s, -d, -s);

	glVertex3f(s, -s, -d);
	glVertex3f(s, -s, d);
	glVertex3f(s, s, d);
	glVertex3f(s, s, d);
	glVertex3f(s, s, -d);
	glVertex3f(s, -s, -d);

	glNormal3f(0, 1, 0);
	glVertex3f(-s, -s, -d);
	glVertex3f(-s, -s, d);
	glVertex3f(s, -s, d);
	glVertex3f(s, -s, d);
	glVertex3f(s, -s, -d);
	glVertex3f(-s, -s, -d);

	glVertex3f(-d, -s, -s);
	glVertex3f(-d, -s, s);
	glVertex3f(d, -s, s);
	glVertex3f(d, -s, s);
	glVertex3f(d, -s, -s);
	glVertex3f(-d, -s, -s);

	glNormal3f(0, -1, 0);
	glVertex3f(-s, s, -d);
	glVertex3f(s, s, -d);
	glVertex3f(s, s, d);
	glVertex3f(s, s, d);
	glVertex3f(-s, s, d);
	glVertex3f(-s, s, -d);

	glVertex3f(-d, s, -s);
	glVertex3f(d, s, -s);
	glVertex3f(d, s, s);
	glVertex3f(d, s, s);
	glVertex3f(-d, s, s);
	glVertex3f(-d, s, -s);

	glNormal3f(0, 0, 1);
	glVertex3f(-s, -d, -s);
	glVertex3f(s, -d, -s);
	glVertex3f(s, d, -s);
	glVertex3f(s, d, -s);
	glVertex3f(-s, d, -s);
	glVertex3f(-s, -d, -s);

	glVertex3f(-d, -s, -s);
	glVertex3f(d, -s, -s);
	glVertex3f(d, s, -s);
	glVertex3f(d, s, -s);
	glVertex3f(-d, s, -s);
	glVertex3f(-d, -s, -s);

	glNormal3f(0, 0, -1);
	glVertex3f(-s, -d, s);
	glVertex3f(-s, d, s);
	glVertex3f(s, d, s);
	glVertex3f(s, d, s);
	glVertex3f(s, -d, s);
	glVertex3f(-s, -d, s);

	glVertex3f(-d, -s, s);
	glVertex3f(-d, s, s);
	glVertex3f(d, s, s);
	glVertex3f(d, s, s);
	glVertex3f(d, -s, s);
	glVertex3f(-d, -s, s);

	glEnd();
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
