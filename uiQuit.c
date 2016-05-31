#include "uiQuit.h"



void displayUIQuit()
{
    int dh, dw, bh, bw, kh, kw; // for image dimensions

    // getting window dimensions
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    GLshort h=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = h/100;

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
        glVertex2s(percUnitW*10,percUnitH*10);
        glVertex2s(w-percUnitW*10,percUnitH*10);
        glVertex2s(w-percUnitW*10,h-percUnitH*10);
        glVertex2s(percUnitW*10,h-percUnitH*10);
    glEnd();


    unsigned char* BLTex;
    BLTex = SOIL_load_image("BL.jpg", &bw, &bh, 0, SOIL_LOAD_RGBA);

    unsigned char* KMTex;
    KMTex = SOIL_load_image("KM.jpg", &kw, &kh, 0, SOIL_LOAD_RGB);

    unsigned char* DNTex;
    DNTex = SOIL_load_image("DN.jpg", &dw, &dh, 0, SOIL_LOAD_RGB);

    //draw images
    glPixelZoom(1, -1);
    glRasterPos2s(percUnitW*12, percUnitH*85);
    glDrawPixels(bw,bh,GL_RGBA,GL_UNSIGNED_BYTE, BLTex);
    displayText("Brandon Lim", percUnitW*12, percUnitH*85-bh-20, 'l');

    glRasterPos2s(percUnitW*45, percUnitH*85);
    glDrawPixels(kw,kh,GL_RGB,GL_UNSIGNED_BYTE, KMTex);
    displayText("Katherine Mann", percUnitW*45, percUnitH*85-kh-20, 'l');

    glRasterPos2s(percUnitW*65, percUnitH*85);
    glDrawPixels(dw,dh,GL_RGB,GL_UNSIGNED_BYTE, DNTex);
    displayText("David Nankivell", percUnitW*65, percUnitH*85-dh-20, 'l');
    glPixelZoom(1,1);


    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // returning to 3D
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

void keysUIQuit(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27: //resume
        exit(0);
        break;
    }
}

void mouseUIQuit(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
        exit(0);
}


