#include "particles.h"
float slowdown = 2.0f;
float xspeed;
float yspeed;

GLuint loop;
GLuint col;
GLuint delay;
GLuint texture[1];


AUX_RGBImageRec *LoadBMP(const char *Filename)
{
    FILE *File=NULL;
    if(!Filename)
    {
        return NULL;
    }

    File = fopen(Filename, "r");
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

    if(TextureImage[0]==LoadBMP("Data\\Particle.bmp"))
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

void PositionParticles(int p_x, int p_y, int p_z)
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)
    {
        particle[loop].x = p_x;
        particle[loop].y = p_y;
        particle[loop].z = p_z;
    }
}

void ColourParticles(float p_rColour, float p_gColour, float p_bColour)
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)
    {
        particle[loop].r = p_rColour;
        particle[loop].g = p_gColour;
        particle[loop].b = p_bColour;
    }
}

void ParticleSpread(int p_xSpread, int p_ySpread, int p_zSpread)
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)
    {
        particle[loop].xi = (float)((rand() % p_xSpread) - 26.0f) * 10.0f;
        particle[loop].yi = (float)((rand() & p_ySpread) - 25.0f) * 10.0f;
        particle[loop].zi = (float)((rand() & p_zSpread) - 25.0f) * 10.0f;
    }
}

void InitParticles()
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)
    {
        particle[loop].active = true;
        particle[loop].life = 5.0f;
        particle[loop].fade = 0.0025;
        particle[loop].xg = 0.0f;
        particle[loop].yg = -0.2f;
        particle[loop].zg = 0.0f;
    }
}

void DrawParticles()
{
    for (loop = 0; loop < MAX_PARTICLES; loop++)
    {
        if(particle[loop].active)
        {
            float p_x = particle[loop].x;
            float p_y = particle[loop].y;
            float p_z = particle[loop].z;
            glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);
            glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2d(1, 1);
            glVertex3f(p_x + 0.5f, p_y + 0.5f, p_z);
            glTexCoord2d(0, 1);
            glVertex3f(p_x - 0.5f, p_y + 0.5f, p_z);
            glTexCoord2d(1, 0);
            glVertex3f(p_x + 0.5f, p_y - 0.5f, p_z);
            glTexCoord2d(0, 0);
            glVertex3f(p_x - 0.5f, p_y - 0.5f, p_z);
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
                particle[loop].active = false;
            }
        }
    }
}

