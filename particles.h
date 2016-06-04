#ifndef PARTICLES_H
#define PARTICLES_H

#include <stdio.h>
#include <GL/glaux.h>
#include <stdbool.h>

#define MAX_PARTICLES 200

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

AUX_RGBImageRec *LoadBMP(const char *Filename);

int LoadGLTextures();

void PositionParticles(int p_x, int p_y, int p_z);

void ColourParticles(float p_r, float p_g, float p_b);

void ParticleSpread(int p_xSpread, int p_ySpread, int p_zSpread);

void InitParticles();

void DrawParticles();

#endif // PARTICLES_H
