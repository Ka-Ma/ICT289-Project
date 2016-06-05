#include "AABBCol.h"

int indexAABB = 0;      //Initial index of static array

void SetLastLocation(float x, float y, float z, int dynamicIndex)
{
    physiO[dynamicIndex].xLoc = x;       //Set last x location of dynamic (player, firework)
    physiO[dynamicIndex].yLoc = y;       //Set last y location of dynamic (player, firework)
    physiO[dynamicIndex].zLoc = z;       //Set last z location of dynamic (player, firework)
}

float ReturnXLoc(int dynamicIndex)
{
    return physiO[dynamicIndex].xLoc;    //Return x location of dynamic (player, firework)
}

float ReturnYLoc(int dynamicIndex)
{
    return physiO[dynamicIndex].yLoc;    //Return y location of dynamic (player, firework)
}

float ReturnZLoc(int dynamicIndex)
{
    return physiO[dynamicIndex].zLoc;    //Return z location of dynamic (player, firework)
}

void AddToStatic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax)
{
    physiO[indexAABB].x.min = xMin;      //Add xMin to associated "static" array slot
    physiO[indexAABB].x.max = xMax;      //Add xMax to associated "static" array slot
    physiO[indexAABB].y.min = yMin;      //Add yMin to associated "static" array slot
    physiO[indexAABB].y.max = yMax;      //Add yMax to associated "static" array slot
    physiO[indexAABB].z.min = zMin;      //Add zMin to associated "static" array slot
    physiO[indexAABB].z.max = zMax;      //Add zMax to associated "static" array slot
    physiO[indexAABB].isStatic = true;   //Mark as static

    indexAABB += 1;     //Increment static index, preventing double-assigning and therefore erasing alternate static AABBs
}

int AddToDynamic(double xMin, double xMax,
                 double yMin, double yMax,
                 double zMin, double zMax)
{
    physiO[indexAABB].x.min = xMin;      //Add xMin to associated "dynamic" array slot
    physiO[indexAABB].x.max = xMax;      //Add xMax to associated "dynamic" array slot
    physiO[indexAABB].y.min = yMin;      //Add yMin to associated "dynamic" array slot
    physiO[indexAABB].y.max = yMax;      //Add yMax to associated "dynamic" array slot
    physiO[indexAABB].z.min = zMin;      //Add zMin to associated "dynamic" array slot
    physiO[indexAABB].z.max = zMax;      //Add zMax to associated "dynamic" array slot
    physiO[indexAABB].isStatic = false;  //Mark as non-static (dynamic)

    indexAABB += 1;     //Increment index, preventing double-assigning

    return indexAABB - 1;   //Return index of "dynamic" slot
}

void UpdateDynamic(double xMin, double xMax,
                   double yMin, double yMax,
                   double zMin, double zMax, int dynamicIndex)
{
    physiO[dynamicIndex].x.min = xMin;   //Add xMin to associated dynamic array
    physiO[dynamicIndex].x.max = xMax;   //Add xMax to associated dynamic array
    physiO[dynamicIndex].y.min = yMin;   //Add yMin to associated dynamic array
    physiO[dynamicIndex].y.max = yMax;   //Add yMax to associated dynamic array
    physiO[dynamicIndex].z.min = zMin;   //Add zMin to associated dynamic array
    physiO[dynamicIndex].z.max = zMax;	 //Add zMin to associated dynamic array
}

bool CheckCollisions(int dynamik)
{
    bool testCollision = false;     //Collisions assumed to be false
    for (int statik = 0; statik <= indexAABB; statik++)     //For each defined static AABB
    {
        if (statik != dynamik)
        {
            if (((physiO[dynamik].x.min <= physiO[statik].x.max) && (physiO[dynamik].x.max >= physiO[statik].x.min)) &&     //Check dynamic xMin less than static xMax, and dynamic xMax greater than static xMin
                ((physiO[dynamik].y.min <= physiO[statik].y.max) && (physiO[dynamik].y.max >= physiO[statik].y.min)) &&     //  and dynamic yMin less than static yMax, and dynamic yMax greater than static yMin
                ((physiO[dynamik].z.min <= physiO[statik].z.max) && (physiO[dynamik].z.max >= physiO[statik].z.min)))       //  and dynamic zMin less than static zMax, and dynamic zMax greater than static zMin
            {
                testCollision = true;
            }
        }
    }
    return testCollision;
}
