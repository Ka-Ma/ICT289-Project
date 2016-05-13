#include "uiSettings.h"
#include "state.h"
#include "dispText.h"


void displayUISettings()
{
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    GLshort h=glutGet(GLUT_WINDOW_HEIGHT);

    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); //to prepare for return to 3D
    glLoadIdentity();
    gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1,1,1);

    glBegin(GL_QUADS);
        glVertex2s(10,10);
        glVertex2s(w-10,10);
        glVertex2s(w-10,h-10);
        glVertex2s(10,h-10);
    glEnd();

    glColor3f(0,0,1);
    displayText("SETTINGS", w/2, h/2);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // returning to 3D
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

void keysUISettings(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: //escape
        state.uiSettings = false;
        break;
    }
}

void mouseUISettings(int button, int state, int x, int y)
{

}
