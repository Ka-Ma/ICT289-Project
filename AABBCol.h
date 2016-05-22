#ifndef AABBCOL_H
#define AABBCOL_H

#include <stdbool.h>

#include "math.h"

void SetLastLocation(float x, float y, float z, int dynamicIndex);      //Save dynamic object (player, firework) last location to associated dynamic array

float ReturnXLoc(int dynamicIndex);     //Return associated dynamic x location

float ReturnYLoc(int dynamicIndex);     //Return associated dynamic y location

float ReturnZLoc(int dynamicIndex);     //Return associated dynamic z location

void AddToStatic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax);     //Add Xmin, Xmax, Ymin, Ymax, Zmin, Zmax of static AABB to array

int AddToDynamic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax);      //Add Xmin, Xmax, Ymin, Ymax, Zmin, Zmax of dynamic AABB to array

void UpdateDynamic(double xMin, double xMax,
                   double yMin, double yMax,
                   double zMin, double zMax, int dynamicIndex);     //Update details of dynamic AABBs

bool CheckCollisions(int dynamik);      //Check if dynamic object (player, firework) is colliding with any static

#endif // AABBCOL_H
