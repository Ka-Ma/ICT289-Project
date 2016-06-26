#include "calcNorm.h"

//for lighting KM
void calcNormal( float p1[3], float p2[3], float p3[3], float n[3])
{
    float v1[3], v2[3], length;

    // calculate two vectors from given vertices
    v1[0] = p2[0] - p1[0];
    v1[1] = p2[1] - p1[1];
    v1[2] = p2[2] - p1[2];
    v2[0] = p3[0] - p1[0];
    v2[1] = p3[1] - p1[1];
    v2[2] = p3[2] - p1[2];
    // calculate cross product of v1 & v2 to get normal
    n[0] = v1[1]*v2[2] - v2[1]*v1[2];
    n[1] = v1[2]*v2[0] - v2[2]*v1[0];
    n[2] = v1[0]*v2[1] - v2[0]*v1[1];
    // calculate length of vector
    length = (float)sqrt (n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
    // make sure vector is not too close to zero!
    if (length == 0.0f)
    length = 1.0f;
    // normalise vector by dividing by length
    n[0] /= length;
    n[1] /= length;
    n[2] /= length;
}
