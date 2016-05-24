#ifndef MATH_H
#define MATH_H

#include <stdbool.h>

#define GAMEOBJECTS     22      //Total GameObjects, Dynamic and Static

#define PI              3.14159265358979323846
#define GRAVITY         9.81

typedef struct Vector3{double x, y, z;} Vector3;

typedef struct FaceVec{
    int faceType;
    Vector3 coord1, coord2, coord3;
} FaceVec;

typedef struct Objects{
    Vector3 *coords;
    FaceVec *facets;
    int vLines, fLines, uLines;
} Objects;

typedef struct MinMax{
    double min, max;
} MinMax;

typedef struct PhysicsObject{
    MinMax x, y, z;
    float xLoc, yLoc, zLoc, upVel;
    bool isStatic;
} PhysicsObject;

PhysicsObject physiO[GAMEOBJECTS];

#endif // MATH_H
