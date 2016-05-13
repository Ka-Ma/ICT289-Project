#include "dispText.h"

void displayText(const char* text, GLshort x, GLshort y)
{
    size_t length = strlen(text);
    int i;

    glColor3f(0,0,0);
    glRasterPos2s(x, y);

    for(i=0;i<length;i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
    }
}

