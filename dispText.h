#ifndef DISPTEXT_H
#define DISPTEXT_H

#include <GL/freeglut.h>

        /**
        @brief displays given text to the screen at the given coordinates in the given size
        @param text
        @param x
        @param y
        @param txtSize - s = small, m = medium, l = large
        */
void displayText(const char* text, GLshort x, GLshort y, char txtSize);

#endif // DISPTEXT_H
