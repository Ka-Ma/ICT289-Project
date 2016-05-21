#include "uiControls.h"
#include "dispText.h"


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

void drawCheckBox(GLfloat x, GLfloat y, const char* text)
{
    glColor3f(0,0,0);
    glBegin(GL_LINE_LOOP);
        glVertex2s(x, y);
        glVertex2s(x+20, y);
        glVertex2s(x+20, y+20);
        glVertex2s(x, y+20);
    glEnd();
    displayText(text, x+25, y+5);
}

void drawCheck(GLfloat x, GLfloat y)
{
    glColor3f(0,0,0);
    glBegin(GL_LINE);
            glVertex2s(x, y);
            glVertex2s(x+20, y+20);
        glEnd();
        glBegin(GL_LINE);
            glVertex2s(x+20, y);
            glVertex2s(x, y+20);
        glEnd();
}
