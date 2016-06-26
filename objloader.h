#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>

#include "math.h"
#include "calcNorm.h"

#define MAX_MODELS  50

FILE  *oFile;

Objects objectList[MAX_MODELS];

char* readModelFile(char *fTitle, int index);

void drawModelFile(const int index);

#endif // OBJLOADER_H
