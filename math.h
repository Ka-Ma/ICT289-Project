#ifndef MATH_H
#define MATH_H

#define PI 3.14159265358979323846

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

typedef struct StaticAABB{
    MinMax x, y, z;
} StaticAABB;

typedef struct DynamicAABB{
    MinMax x, y, z;
    float locX, locY, locZ;
} DynamicAABB;

#endif // MATH_H
