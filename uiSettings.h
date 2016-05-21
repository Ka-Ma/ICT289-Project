        /**
        @brief Draws and defines controls for Settings User Interface.
        @author Katherine Mann
        @version 01
        @date 13/5/2016
        */

#ifndef UISETTINGS_H
#define UISETTINGS_H

#include "globalState.h"
#include "dispText.h"
#include "uiControls.h"

typedef GLfloat point2[2];
typedef GLfloat flt3[3];
typedef struct triangle{
    point2 top;
    point2 left;
    point2 right;
    point2 mtl;
    point2 mlr;
    point2 mtr;
    point2 centre;
};

struct globalState tmpState;
struct uiBits {
    GLshort height; // height of window
    GLshort saveBL[2]; //x,y coords of bottom left of save button
    GLshort saveTR[2]; //x,y coords of top right of save button
    GLshort cancelBL[2]; //x,y coords of bottom left of cancel button
    GLshort cancelTR[2]; //x,y coords of top right of cancel button
    struct triangle clrpick;
    }uiBits;

void displayUISettings();
void keysUISettings(unsigned char key, int x, int y);
void mouseUISettings(int button, int state, int x, int y);
void drawColourPicker(GLshort percUnitW, GLshort percUnitH);

extern struct triangle clrpick;
extern struct globalState tmpState;

#endif // UISETTINGS_H
