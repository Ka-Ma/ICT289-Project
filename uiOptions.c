#include "uiOptions.h"



void displayUIOptions()
{
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    uiO.height=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = uiO.height/100;

    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); //to prepare for return to 3D
    glLoadIdentity();
    gluOrtho2D(0,w,0,uiO.height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //Options UI background
    glColor3f(1,1,1); //white

    glBegin(GL_QUADS);
        glVertex2s(percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,uiO.height-percUnitH*20);
        glVertex2s(percUnitW*20,uiO.height-percUnitH*20);
    glEnd();

    displayText("OPTIONS: q to quit, esc to resume", percUnitW*22, percUnitH*78, 'l');

    //instructions
    displayText("Movement Controls", percUnitW*22, percUnitH* 70, 'l');
    displayText("W", percUnitW*22, percUnitH*68, 'l');
    displayText("walk forward", percUnitW*23, percUnitH*68, 'm');

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // returning to 3D
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

void keysUIOptions(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'q': //quit
        gState.uiQuit = true;
        break;
    case 27: //resume ... pressing esc a second time to enact this control doesn't work.
        gState.uiOptions = false;
        break;
    }
}

void mouseUIOptions(int button, int state, int x, int y)
{

}

