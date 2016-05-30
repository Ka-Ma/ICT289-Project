#include "uiOptions.h"



void displayUIOptions()
{
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    uiO.height=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = uiO.height/100;

    int iTW, iTH;

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
    //read in image
    unsigned char* instrTex = SOIL_load_image("ICT289-Controls.PNG", &iTW, &iTH, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, iTW, iTH, 0, GL_RGB, GL_UNSIGNED_BYTE, instrTex);
    glBindTexture(GL_TEXTURE_2D, instrTex);

    //draw image onto quad
    // next try porting across image code, see if that works...

    displayText("Movement Controls", percUnitW*22, percUnitH* 70, 'l');
    displayText("W", percUnitW*22, percUnitH*68, 'l');
    displayText("walk forward", percUnitW*23, percUnitH*68, 'm');

    //check box for tracking firework
    uiO.trackBL[0] = percUnitW*22;
    uiO.trackBL[1] = percUnitH*22;
    drawCheckBox(uiO.trackBL[0], uiO.trackBL[1] ,"Track Firework");
    //if track selected draw cross
    if(gState.trackFW){
        drawCheck(uiO.trackBL[0],uiO.trackBL[1]);
    }

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
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        //height-y to convert coordinates to match screen and mouse
        y = uiO.height - y;

        //track fireworks
        if(x > uiO.trackBL[0] && x < uiO.trackBL[0]+20 && y > uiO.trackBL[1] && y < uiO.trackBL[1]+20){
            if(gState.trackFW){
                gState.trackFW = false;
            }
            else{
                gState.trackFW = true;
            }
        }
    }
}

