#include "uiOptions.h"



void displayUIOptions()
{
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    uiO.height=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = uiO.height/100;

    int iTW, iTH; //for image size

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
    unsigned char* instrTex;
    instrTex = SOIL_load_image("uiO-instruct.PNG", &iTW, &iTH, 0, SOIL_LOAD_RGBA);

    //draw image
    glPixelZoom(1, -1);
    glRasterPos2s(percUnitW*22, percUnitH*75);
    glDrawPixels(iTW,iTH,GL_RGBA,GL_UNSIGNED_BYTE, instrTex);
    glPixelZoom(1,1);


    //check box for tracking firework
    uiO.trackBL[0] = percUnitW*22;
    uiO.trackBL[1] = percUnitH*22;
    drawCheckBox(uiO.trackBL[0], uiO.trackBL[1] ,"Track Firework");
    //if track selected draw cross
    if(gState.trackFW){
        drawCheck(uiO.trackBL[0],uiO.trackBL[1]);
    }


    //resume button
    uiO.resumeBL[0] = percUnitW*70;
    uiO.resumeBL[1] = percUnitH*31;
    uiO.resumeTR[0] = percUnitW*79;
    uiO.resumeTR[1] = percUnitH*39;
    glColor3f(0, 0.5, 0);
    glBegin(GL_QUADS);
        glVertex2sv(uiO.resumeBL);
        glVertex2s(uiO.resumeTR[0], uiO.resumeBL[1]);
        glVertex2sv(uiO.resumeTR);
        glVertex2s(uiO.resumeBL[0], uiO.resumeTR[1]);
    glEnd();
    displayText("Resume", uiO.resumeBL[0]+5, uiO.resumeBL[1]+5,'l');


    //Quit button
    uiO.quitBL[0] = percUnitW*70;
    uiO.quitBL[1] = percUnitH*22;
    uiO.quitTR[0] = percUnitW*79;
    uiO.quitTR[1] = percUnitH*30;
    glColor3f(0.6, 0, 0);
    glBegin(GL_QUADS);
        glVertex2sv(uiO.quitBL);
        glVertex2s(uiO.quitTR[0], uiO.quitBL[1]);
        glVertex2sv(uiO.quitTR);
        glVertex2s(uiO.quitBL[0], uiO.quitTR[1]);
    glEnd();
    displayText("Quit", uiO.quitBL[0]+5, uiO.quitBL[1]+5,'l');
    //underline Q
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2s(uiO.quitBL[0]+6,uiO.quitBL[1]+3);
        glVertex2s(uiO.quitBL[0]+18,uiO.quitBL[1]+3);
    glEnd();
    glLineWidth(1);


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

        //resume
        else if(x > uiO.resumeBL[0] && x < uiO.resumeTR[0] && y > uiO.resumeBL[1] && y < uiO.resumeTR[1]){
            gState.uiOptions = false;
        }

        //quit
        else if(x > uiO.quitBL[0] && x < uiO.quitTR[0] && y > uiO.quitBL[1] && y < uiO.quitTR[1]){
            gState.uiQuit = true;
        }
    }
}

