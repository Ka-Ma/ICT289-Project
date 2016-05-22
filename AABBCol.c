#include "AABBCol.h"

int indexAABB = 0;      //Initial index of static array

void SetLastLocation(float x, float y, float z, int dynamicIndex)
{
    gameO[dynamicIndex].xLoc = x;       //Set last x location of dynamic (player, firework)
    gameO[dynamicIndex].yLoc = y;       //Set last y location of dynamic (player, firework)
    gameO[dynamicIndex].zLoc = z;       //Set last z location of dynamic (player, firework)
}

float ReturnXLoc(int dynamicIndex)
{
    return gameO[dynamicIndex].xLoc;    //Return x location of dynamic (player, firework)
}

float ReturnYLoc(int dynamicIndex)
{
    return gameO[dynamicIndex].yLoc;    //Return y location of dynamic (player, firework)
}

float ReturnZLoc(int dynamicIndex)
{
    return gameO[dynamicIndex].zLoc;    //Return z location of dynamic (player, firework)
}

void AddToStatic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax)
{
    gameO[indexAABB].x.min = xMin;      //Add xMin to associated "static" array slot
    gameO[indexAABB].x.max = xMax;      //Add xMax to associated "static" array slot
    gameO[indexAABB].y.min = yMin;      //Add yMin to associated "static" array slot
    gameO[indexAABB].y.max = yMax;      //Add yMax to associated "static" array slot
    gameO[indexAABB].z.min = zMin;      //Add zMin to associated "static" array slot
    gameO[indexAABB].z.max = zMax;      //Add zMax to associated "static" array slot
    gameO[indexAABB].isStatic = true;   //Mark as static

    indexAABB += 1;     //Increment static index, preventing double-assigning and therefore erasing alternate static AABBs
}

int AddToDynamic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax)
{
    gameO[indexAABB].x.min = xMin;      //Add xMin to associated "dynamic" array slot
    gameO[indexAABB].x.max = xMax;      //Add xMax to associated "dynamic" array slot
    gameO[indexAABB].y.min = yMin;      //Add yMin to associated "dynamic" array slot
    gameO[indexAABB].y.max = yMax;      //Add yMax to associated "dynamic" array slot
    gameO[indexAABB].z.min = zMin;      //Add zMin to associated "dynamic" array slot
    gameO[indexAABB].z.max = zMax;      //Add zMax to associated "dynamic" array slot
    gameO[indexAABB].isStatic = false;  //Mark as non-static (dynamic)

    indexAABB += 1;     //Increment index, preventing double-assigning

    return indexAABB - 1;   //Return index of "dynamic" slot
}

void UpdateDynamic(double xMin, double xMax,
                   double yMin, double yMax,
                   double zMin, double zMax, int dynamicIndex)
{
    gameO[dynamicIndex].x.min = xMin;   //Add xMin to associated dynamic array
    gameO[dynamicIndex].x.max = xMax;   //Add xMax to associated dynamic array
    gameO[dynamicIndex].y.min = yMin;   //Add yMin to associated dynamic array
    gameO[dynamicIndex].y.max = yMax;   //Add yMax to associated dynamic array
    gameO[dynamicIndex].z.min = zMin;   //Add zMin to associated dynamic array
    gameO[dynamicIndex].z.max = zMax;
}

bool CheckCollisions(int dynamik)
{
    bool testCollision = false;     //Collisions assumed to be false
    for (int statik = 0; statik <= indexAABB; statik++)     //For each defined static AABB
    {
        if (statik != dynamik)
        {
            if (((gameO[dynamik].x.min <= gameO[statik].x.max) && (gameO[dynamik].x.max >= gameO[statik].x.min)) &&     //Check dynamic xMin less than static xMax, and dynamic xMax greater than static xMin
                ((gameO[dynamik].y.min <= gameO[statik].y.max) && (gameO[dynamik].y.max >= gameO[statik].y.min)) &&     //  and dynamic yMin less than static yMax, and dynamic yMax greater than static yMin
                ((gameO[dynamik].z.min <= gameO[statik].z.max) && (gameO[dynamik].z.max >= gameO[statik].z.min)))       //  and dynamic zMin less than static zMax, and dynamic zMax greater than static zMin
            {
                testCollision = true;
            }
        }
    }
    return testCollision;
}
