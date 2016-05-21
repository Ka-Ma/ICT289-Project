#ifndef MATH_H
#define MATH_H

#define PI 3.14159265358979323846

struct Vector3{double x, y, z;};

struct FaceVec{
    int faceType;
    struct Vector3 coord1, coord2, coord3;
};

struct Objects{
    struct Vector3 *coords;
    struct FaceVec *facets;
    int vLines, fLines, uLines;
};

#endif // MATH_H
