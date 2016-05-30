        /**
        @brief Draws and defines controls for Settings User Interface.
        @author Katherine Mann
        @version 01
        @date 13/5/2016
        */

#ifndef UISETTINGS_H
#define UISETTINGS_H

#include <stdio.h>
#include <math.h>
#include "globalState.h"
#include "dispText.h"
#include "uiControls.h"

typedef GLfloat point2[2];
typedef GLfloat flt3[3];
typedef struct triangle{
    point2 top;
    point2 left;
    point2 right;
    point2 mtl; // mid point between top and left
    point2 mlr; // mid point between left and right
    point2 mtr; // mid point between top and right
    point2 centre; // centroid of triangle
}triangle;

//used to know if mouse has clicked on a particular control
struct uiS {
    GLshort height; // height of window for mouse/window coord match up
    struct triangle clrpick;
    GLshort chosenClr[2]; //x,y coords of bottom left of chosen colour square
    GLshort randClr[2]; // x,y coords of checkbox for random colour
    GLshort anglesBL[2]; //x,y coords of bottom left of angles area
    GLshort anglesTR[2]; //x,y coords of top right of angles area
    GLshort anglesPS[2][9]; //individual x,y coords of 9 angle preset centres
    GLshort randAng[2]; //x,y coords of checkbox for random angle
    GLshort velocityL[2]; //x,y coords of left end  of velocity slider
    GLshort velocityR[2]; //x,y coords of right end of velocity slider
    GLfloat velocityChX; // x coord of centre of marker
    char velocityChTxt[6]; // chosen value converted to char for display
    char velocityMnTxt[6]; // min value converted to char for display
    char velocityMxTxt[6]; // max value converted to char for display
    GLshort fuseL[2]; //x,y coords of left end  of fuse slider
    GLshort fuseR[2]; //x,y coords of right end of fuse slider
    GLfloat fuseChX; // x coord of centre of marker
    char fuseChTxt[6]; // chosen value converted to char for display
    char fuseMnTxt[6]; // min value converted to char for display
    char fuseMxTxt[6]; // max value converted to char for display
    GLshort saveBL[2]; //x,y coords of bottom left of save button
    GLshort saveTR[2]; //x,y coords of top right of save button
    GLshort cancelBL[2]; //x,y coords of bottom left of cancel button
    GLshort cancelTR[2]; //x,y coords of top right of cancel button
    }uiS;

void displayUISettings();
void keysUISettings(unsigned char key, int x, int y);
void mouseUISettings(int button, int state, int x, int y);
void drawColourPicker(GLshort percUnitW, GLshort percUnitH);
void drawAnglePresets(GLshort percUnitW, GLshort percUnitH);


//extern struct globalState tmpState;

#endif // UISETTINGS_H
