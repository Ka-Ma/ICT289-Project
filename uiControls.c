#include "uiControls.h"
#include "dispText.h"
#include <math.h>
#include <stdbool.h>


void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int triangleAmount = 20; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_TRIANGLE_FAN);
		glVertex2f(x, y); // center of circle
		for(i = 0; i <= triangleAmount;i++) {
			glVertex2f(
                x + (radius * cos(i *  twicePi / triangleAmount)),
                y + (radius * sin(i * twicePi / triangleAmount))
            );
		}
	glEnd();
}

void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius){
	int i;
	int lineAmount = 100; //# of triangles used to draw circle

	//GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * PI;

	glBegin(GL_LINE_LOOP);
		for(i = 0; i <= lineAmount;i++) {
			glVertex2f(
			    x + (radius * cos(i *  twicePi / lineAmount)),
			    y + (radius* sin(i * twicePi / lineAmount))
			);
		}
	glEnd();
}

void drawCheckBox(GLshort x, GLshort y, const char* text)
{
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2s(x, y);
        glVertex2s(x+20, y);
        glVertex2s(x+20, y+20);
        glVertex2s(x, y+20);
    glEnd();
    displayText(text, x+25, y+5, 'm');
}

void drawCheck(GLshort x, GLshort y)
{
    glColor3f(0,0,0);
    glBegin(GL_LINES);
        glVertex2s(x, y);
        glVertex2s(x+20, y+20);
    glEnd();
    glBegin(GL_LINES);
        glVertex2s(x+20, y);
        glVertex2s(x, y+20);
    glEnd();
}

void drawSlider(GLshort left[], GLshort right[])
{
    int i,j;
    GLshort between;
    bool done = false;

    //to find ideal between
    for(j=8; j<20; j++)
    {
        if((right[0]-left[0])%j == 0 && !done)
        {
            between = (right[0]-left[0])/j;
            done = true;
        }
    }

    //if can't find ideal between
    if(!done)
        between = 100;

    glColor3f(0,0,0);

    //draw main line
    glBegin(GL_LINES);
        glVertex2sv(left);
        glVertex2sv(right);
    glEnd();

    //draw interval markers
    for(i = 0; i <= (right[0] - left[0]); i = i + between)
    {
        glBegin(GL_LINES);
            glVertex2s(left[0]+i, left[1]+5);
            glVertex2s(left[0]+i, left[1]-5);
        glEnd();
    }
}

void drawMarker(GLshort x, GLshort y)
{
    glBegin(GL_QUADS);
        glVertex2s(x-10, y-7);
        glVertex2s(x+10, y-7);
        glVertex2s(x+10, y+7);
        glVertex2s(x-10, y+7);
    glEnd();
}
