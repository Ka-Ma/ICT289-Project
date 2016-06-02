#include "particles.h"
float slowdown = 2.0f;
float xspeed;
float yspeed;
//float zoom = -40.0f;

GLuint loop;
GLuint col;
GLuint delay;
GLuint texture[1];


AUX_RGBImageRec *LoadBMP(const char *Filename)
{
    FILE*File=NULL;
    if(!Filename)
    {
        return NULL;
    }

    File = fopen(Filename, 'r');
    if (File)
    {
        fclose(File);
        return auxDIBImageLoad(Filename);
    }
    return NULL;
}

int LoadGLTextures()
{
    int Status = FALSE;

    AUX_RGBImageRec *TextureImage[1];

    memset(TextureImage, 0, sizeof(void*)*1);

    if(TextureImage[0]==LoadBMP("Data/Particle.bmp"))
    {
        Status=TRUE;
        glGenTextures(1, &texture[0]);

        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
    }

    if(TextureImage[0])
    {
        if(TextureImage[0]->data)
        {
            free(TextureImage[0]->data);
        }
        free(TextureImage[0]);
    }
    return Status;
}

void InitParticles()
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)
    {
        particle[loop].active = true;
        particle[loop].life = 1.0f;
        particle[loop].fade = (float)(rand()%100) / 1000.0f + 0.003f;
        particle[loop].r = colours[loop * (12/MAX_PARTICLES)][0];
        particle[loop].g = colours[loop * (12/MAX_PARTICLES)][1];
        particle[loop].b = colours[loop * (12/MAX_PARTICLES)][2];
        particle[loop].xi = (float)((rand() % 50) - 26.0f) * 10.0f;
        particle[loop].yi = (float)((rand() & 50) - 25.0f) * 10.0f;
        particle[loop].zi = (float)((rand() & 50) - 25.0f) * 10.0f;
        particle[loop].xg = 0.0f;
        particle[loop].yg = -0.8f;
        particle[loop].zg = 0.0f;
    }
}

void DrawParticles()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    for (loop = 0; loop < MAX_PARTICLES; loop++)
    {
        if(particle[loop].active)
        {
            float x = particle[loop].x;
            float y = particle[loop].y;
            float z = particle[loop].z;
            glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2d(1, 1);
            glVertex3f(x + 0.5f, y + 0.5f, z);
            glTexCoord2d(0, 1);
            glVertex3f(x - 0.5f, y + 0.5f, z);
            glTexCoord2d(1, 0);
            glVertex3f(x + 0.5f, y - 0.5f, z);
            glTexCoord2d(0, 0);
            glVertex3f(x - 0.5f, y - 0.5f, z);
            glEnd();
            particle[loop].x += particle[loop].xi / (slowdown * 1000);
            particle[loop].y += particle[loop].yi / (slowdown * 1000);
            particle[loop].z += particle[loop].zi / (slowdown * 1000);
            particle[loop].xi += particle[loop].xg;
            particle[loop].yi += particle[loop].yg;
            particle[loop].zi += particle[loop].zg;
            particle[loop].life -= particle[loop].fade;
            if (particle[loop].life < 0.0f)
            {
                particle[loop].life = 1.0f;
                particle[loop].fade = (float)(rand() % 100) / 1000.0f + 0.003f;
                particle[loop].x = 0.0f;
                particle[loop].y = 0.0f;
                particle[loop].z = 0.0f;
                particle[loop].xi = xspeed + (float)((rand() % 60) - 32.0f);
                particle[loop].yi = yspeed + (float)((rand() % 60) - 30.0f);
                particle[loop].zi = (float)((rand() % 60) - 30.0f);
                particle[loop].r = colours[col][0];
                particle[loop].g = colours[col][1];
                particle[loop].b = colours[col][2];
            }
        }
    }
}

