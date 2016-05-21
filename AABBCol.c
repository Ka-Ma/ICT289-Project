#include "AABBCol.h"

int statIndex = 0;      //Current index of static array

void SetLastLocation(float x, float y, float z, int dynamicIndex)
{
    dAABB[dynamicIndex].locX = x;       //Set last x location of dynamic (player, firework)
    dAABB[dynamicIndex].locY = y;       //Set last y location of dynamic (player, firework)
    dAABB[dynamicIndex].locZ = z;       //Set last z location of dynamic (player, firework)
}

float ReturnXLoc(int dynamicIndex)
{
    return dAABB[dynamicIndex].locX;    //Return x location of dynamic (player, firework)
}

float ReturnYLoc(int dynamicIndex)
{
    return dAABB[dynamicIndex].locY;    //Return y location of dynamic (player, firework)
}

float ReturnZLoc(int dynamicIndex)
{
    return dAABB[dynamicIndex].locZ;    //Return z location of dynamic (player, firework)
}

void AddToStatic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax)
{
    sAABB[statIndex].x.min = xMin;      //Add xMin to associated static array
    sAABB[statIndex].x.max = xMax;      //Add xMax to associated static array
    sAABB[statIndex].y.min = yMin;      //Add yMin to associated static array
    sAABB[statIndex].y.max = yMax;      //Add yMax to associated static array
    sAABB[statIndex].z.min = zMin;      //Add zMin to associated static array
    sAABB[statIndex].z.max = zMax;      //Add zMax to associated static array

    statIndex += 1;     //Increment static index, preventing double-assigning and therefore erasing alternate static AABBs
}

void AddToDynamic(double xMin, double xMax,
                  double yMin, double yMax,
                  double zMin, double zMax, int dynamicIndex)
{
    dAABB[dynamicIndex].x.min = xMin;   //Add xMin to associated dynamic array
    dAABB[dynamicIndex].x.max = xMax;   //Add xMax to associated dynamic array
    dAABB[dynamicIndex].y.min = yMin;   //Add yMin to associated dynamic array
    dAABB[dynamicIndex].y.max = yMax;   //Add yMax to associated dynamic array
    dAABB[dynamicIndex].z.min = zMin;   //Add zMin to associated dynamic array
    dAABB[dynamicIndex].z.max = zMax;   //Add zMax to associated dynamic array

    //NOTE: Does not need to auto-increment dynamic, as client needs to know dynamic array location for collisions etcetera
}

bool CheckCollisions(int dynamik)
{
    bool testCollision = false;     //Collisions assumed to be false
    for (int statik = 0; statik <= statIndex; statik++)     //For each defined static AABB
    {
        if (((dAABB[dynamik].x.min <= sAABB[statik].x.max) && (dAABB[dynamik].x.max >= sAABB[statik].x.min)) &&     //Check dynamic xMin less than static xMax, and dynamic xMax greater than static xMin
            ((dAABB[dynamik].y.min <= sAABB[statik].y.max) && (dAABB[dynamik].y.max >= sAABB[statik].y.min)) &&     //  and dynamic yMin less than static yMax, and dynamic yMax greater than static yMin
            ((dAABB[dynamik].z.min <= sAABB[statik].z.max) && (dAABB[dynamik].z.max >= sAABB[statik].z.min)))       //  and dynamic zMin less than static zMax, and dynamic zMax greater than static zMin
        {
            testCollision = true;
        }
    }
    return testCollision;
}
