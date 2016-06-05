#include "particles.h"
float slowdown = 2.0f;  //How quickly particles slow down after initial burst
float xspeed;           //Speed along the x axis
float yspeed;           //Speed along the y axis

GLuint loop;
GLuint col;
GLuint delay;
GLuint texture[1];


AUX_RGBImageRec *LoadBMP(const char *Filename)
{
    FILE *File=NULL;                            //Initialise file
    if(!Filename)                               //Check file initialised
    {
        return NULL;                            //Return null if not
    }

    File = fopen(Filename, "r");                //Open specific file
    if (File)                                   //If file is open
    {
        fclose(File);                           //Close it
        return auxDIBImageLoad(Filename);       //Return somthing
    }
    return NULL;                                //Return null
}

int LoadGLTextures()
{
    int Status = FALSE;                                                                                                                         //Set status marker to FALSE

    AUX_RGBImageRec *TextureImage[1];                                                                                                           //Do something with image stuff

    memset(TextureImage, 0, sizeof(void*)*1);                                                                                                   //Set image memory size

    if(TextureImage[0]==LoadBMP("Data\\Particle.bmp"))                                                                                          //Load file
    {
        Status=TRUE;                                                                                                                            //If successful, status = TRUE
        glGenTextures(1, &texture[0]);                                                                                                          //Generate textures

        glBindTexture(GL_TEXTURE_2D, texture[0]);                                                                                               //Bind textures
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                                                                       //Texture set up stuff
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);                                                                       //More texture set up stuff
        glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);  //Some more
    }

    if(TextureImage[0])                                                                                                                         //If textures not set?
    {
        if(TextureImage[0]->data)                                                                                                               //Check if data exists
        {
            free(TextureImage[0]->data);                                                                                                        //If so free data memory
        }
        free(TextureImage[0]);                                                                                                                  //Otherwise free all texture memory
    }
    return Status;                                                                                                                              //Return the status
}

void PositionParticles(int p_x, int p_y, int p_z)   //Set particle locations
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)     //Loop through all particles
    {
        particle[loop].x = p_x;                     //Set particle x
        particle[loop].y = p_y;                     //Set particle y
        particle[loop].z = p_z;                     //Set particle z
    }
}

void ColourParticles(float p_rColour, float p_gColour, float p_bColour) //Set colour of particles
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)                         //Loop through all particles
    {
        particle[loop].r = p_rColour;                                   //Set particle red
        particle[loop].g = p_gColour;                                   //Set particle green
        particle[loop].b = p_bColour;                                   //Set particle blue
    }
}

void ParticleSpread(int p_xSpread, int p_ySpread, int p_zSpread)            //Set particle spread (ie how far they move from centre
{
    for(loop = 0; loop < MAX_PARTICLES; loop++)                             //Loop through all particles
    {
        particle[loop].xi = (float)((rand() % p_xSpread) - 26.0f) * 10.0f;  //Set xi
        particle[loop].yi = (float)((rand() & p_ySpread) - 25.0f) * 10.0f;  //Set yi
        particle[loop].zi = (float)((rand() & p_zSpread) - 25.0f) * 10.0f;  //Set zi
    }
}

void InitParticles()                            //Activate particles
{
    for(loop = 0; loop < MAX_PARTICLES; loop++) //Loop through all particles
    {
        particle[loop].active = true;           //Activate particle
        particle[loop].life = 5.0f;             //Give particles a life
        particle[loop].fade = 0.0025;           //Set particle life-drain speed
        particle[loop].xg = 0.0f;               //Set "gravity" pull on x axis
        particle[loop].yg = -0.2f;              //Set "gravity" pull on y axis
        particle[loop].zg = 0.0f;               //Set "gravity" pull on z axis
    }
}

void DrawParticles()                                                                                    //Call active particles
{
    for (loop = 0; loop < MAX_PARTICLES; loop++)                                                        //Loop through all particles
    {
        if(particle[loop].active)                                                                       //If a particle is active
        {
            float p_x = particle[loop].x;                                                               //Store current x location
            float p_y = particle[loop].y;                                                               //Store current y location
            float p_z = particle[loop].z;                                                               //Store current z location
            glColor4f(particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life);       //Set particle colour
            float col[4] = {particle[loop].r, particle[loop].g, particle[loop].b, particle[loop].life}; //Create particle colour array
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, col);                                           //Set particle diffuse material properties
            glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, col);                                          //Set particle specular material properties
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, col);                                           //Set particle ambient material properties
            glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, col);                                          //Set particle emission material properties
            glBegin(GL_TRIANGLE_STRIP);                                                                 //Begin a triangle strip to draw particle squares
            glTexCoord2d(1, 1);
            glVertex3f(p_x + 0.5f, p_y + 0.5f, p_z);
            glTexCoord2d(0, 1);
            glVertex3f(p_x - 0.5f, p_y + 0.5f, p_z);
            glTexCoord2d(1, 0);
            glVertex3f(p_x + 0.5f, p_y - 0.5f, p_z);
            glTexCoord2d(0, 0);
            glVertex3f(p_x - 0.5f, p_y - 0.5f, p_z);
            glEnd();
            particle[loop].x += particle[loop].xi / (slowdown * 1000);                                  //Modify particle x location
            particle[loop].y += particle[loop].yi / (slowdown * 1000);                                  //Modify particle y location
            particle[loop].z += particle[loop].zi / (slowdown * 1000);                                  //Modify particle z location
            particle[loop].xi += particle[loop].xg;                                                     //Modify particle x "velocity" with x "gravity"
            particle[loop].yi += particle[loop].yg;                                                     //Modify particle y "velocity" with y "gravity"
            particle[loop].zi += particle[loop].zg;                                                     //Modify particle Z "velocity" with z "gravity"
            particle[loop].life -= particle[loop].fade;                                                 //Lower particle life
            if (particle[loop].life < 0.0f)                                                             //If life reaches zero
            {
                particle[loop].active = false;                                                          //Deactivate particle
            }
        }
    }
}

