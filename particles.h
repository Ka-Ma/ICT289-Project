#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdio.h>
#include <GL/glaux.h>
#include <stdbool.h>

#define MAX_PARTICLES 1000



typedef struct particles
{
    bool active;
    float life;
    float fade;
    float r;
    float g;
    float b;
    float x;
    float y;
    float z;
    float xi;
    float yi;
    float zi;
    float xg;
    float yg;
    float zg;
} particles;

particles particle[MAX_PARTICLES];

static GLfloat colours[12][3] =
{
    {1.0f, 0.5f, 0.5f}, {1.0f, 0.75f, 0.5f}, {1.0f, 1.0f, 0.5f}, {0.75f, 1.0f, 0.5f},
    {0.5f, 1.0f, 0.5f}, {0.5f, 1.0f, 0.75f}, {0.5f, 1.0f, 1.0f}, {0.5f, 0.75f, 1.0f},
    {0.5f, 0.5f, 1.0f}, {0.75f, 0.5f, 1.0f}, {1.0f, 0.5f, 1.0f}, {1.0f, 0.5f, 0.75f}
};

AUX_RGBImageRec *LoadBMP(const char *Filename);

int LoadGLTextures();

void InitParticles();

void DrawParticles();

#endif // PARTICLES_H
