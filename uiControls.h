#ifndef UICONTROLS_H
#define UICONTROLS_H

#include "math.h"
#include <GL/freeglut.h>

/**
 * Function that handles the drawing of a circle using the triangle fan
 * method. This will create a filled circle.
 *
 * Sourced from https://gist.github.com/strife25/803118
 *
 * Params:
 *	x (GLFloat) - the x position of the center point of the circle
 *	y (GLFloat) - the y position of the center point of the circle
 *	radius (GLFloat) - the radius that the painted circle will have
 */
void drawFilledCircle(GLfloat x, GLfloat y, GLfloat radius);

/**
 * Function that handles the drawing of a circle using the line loop
 * method. This will create a hollow circle.
 *
 * Sourced from https://gist.github.com/strife25/803118
 *
 * Params:
 *	x (GLFloat) - the x position of the center point of the circle
 *	y (GLFloat) - the y position of the center point of the circle
 *	radius (GLFloat) - the radius that the painted circle will have
 */
void drawHollowCircle(GLfloat x, GLfloat y, GLfloat radius);


/**
@brief Function to draw an empty checkbox
@param x - x position of start point (bottom left)
@param y - y position of start point (bottom left)
@param text - the text to be positioned left of the checkbox
*/
void drawCheckBox(GLshort x, GLshort y, const char* text);

/**
@brief Function to draw a check in the box
@param x - x position of start point (bottom left)
@param y - y position of start point (bottom left)
*/
void drawCheck(GLshort x, GLshort y);

#endif // UICONTROLS_H
