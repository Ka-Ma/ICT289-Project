#ifndef AABBCOL_H
#define AABBCOL_H

#define NUMSTATICBOX    20      //Max number of static objects (walls, buildings)
#define NUMDYNAMICBOX   2       //Max number of dynamic objects (player, firework)

#include <stdbool.h>

#include "math.h"

StaticAABB  sAABB[NUMSTATICBOX];   //Number of static bounding boxes
DynamicAABB dAABB[NUMDYNAMICBOX];  //Number of dynamic bounding boxes

void SetLastLocation(float x, float y, float z, int dynamicIndex);      //Save dynamic object (player, firework) last location to associated dynamic array

float ReturnXLoc(int dynamicIndex);     //Return associated dynamic x location

float ReturnYLoc(int dynamicIndex);     //Return associated dynamic y location

float ReturnZLoc(int dynamicIndex);     //Return associated dynamic z location

void AddToStatic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax);     //Add Xmin, Xmax, Ymin, Ymax, Zmin, Zmax of static AABB to array

void AddToDynamic(double xMin, double xMax,
                  double yMin, double yMax,
                  double zMin, double zMax, int dynamicIndex);      //Add Xmin, Xmax, Ymin, Ymax, Zmin, Zmax of dynamic AABB to array

bool CheckCollisions(int dynamik);      //Check if dynamic object (player, firework) is colliding with any static

#endif // AABBCOL_H
