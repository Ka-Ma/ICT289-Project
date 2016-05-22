#include "dispText.h"

void displayText(const char* text, GLshort x, GLshort y, char txtSize)
{
    size_t length = strlen(text);
    int i;

    glColor3f(0,0,0);
    glRasterPos2s(x, y);

    if(txtSize == 's'){
        for(i=0;i<length;i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10, text[i]);
        }
    }
    else if(txtSize =='m'){
        for(i=0;i<length;i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, text[i]);
        }
    }
    else if(txtSize =='l'){
        for(i=0;i<length;i++)
        {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
        }
    }
}

