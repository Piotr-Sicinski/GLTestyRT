#include "Scene.h"

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