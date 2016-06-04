#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/freeglut.h>
#include <fcntl.h>
#include <unistd.h>

#include "objloader.h"
#include "globalState.h" // global variables for fireworks settings and UI display states KM
#include "uiKeysHandler.h"
#include "uiMouseHandler.h"
#include "uiSettings.h"
#include "uiOptions.h"
#include "uiQuit.h"
#include "AABBCol.h"
#include "particles.h"

#define ESCAPE          27
#define MAX_FILE_NAME   20
#define MODEL_ONE       0
#define FIREWORK        1

//Player dimensions
#define PLAYER_X    2
#define PLAYER_Y    5
#define PLAYER_Z    2

#define M_PI 3.14159265358979323846 //dunno why, but math.h's M_PI wasn't working...
//Fireworks/Realtime stuff
#define TIMERSECS 33
#define GRAV -24.5 //gravity
#define THETA 75 //Launch angle for tubes not middle, middle tube is 0 degrees (directly upwards)

int fwNum = 0;
int startTime, prevTime;
float maxS =0;
float prevLoc[3] = {0.0f,0.0f,0.0f};

int wRes = 1600; int hRes = 1000;
// Angle of rotation for camera direction
float angle = 0.0f;
// Actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera

float x_loc = -5.0f, y_loc = 5.0f, z_loc = -10.0f;

// Key states. Will be 0 when no keys are being pressed
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

int t;
int frame = 0, time, timebase = 0;
int fps;

//Dynamic collision indexes
int player_col;

bool filesUnRead = true;

bool collisionsAdded = false;
bool isColliding = false;

//For Collision corners (pier)
double corners = 45*M_PI/180.0;

void createFirework()
{
    GLUquadricObj *quadObj;

    glNewList(FIREWORK, GL_COMPILE);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluCylinder(quadObj, 0.6, 0.6, 2.4, 24, 4);
    gluDisk(quadObj, 0, 0.6, 24, 5);
    glTranslatef(0, 0, 3);
    gluSphere(quadObj, 0.9, 24, 24);
    glEndList();
}

void readModels()
{
    if (filesUnRead)
    {
        char* status;

        printf("Loading bone.off...");
        status = readModelFile("bone.off", MODEL_ONE);
        printf(status);

        filesUnRead = false;
    }
}

void drawModels()
{
    glPushMatrix();
    glTranslatef(-30.0f, 40.0f, 0.0f);

    drawModelFile(MODEL_ONE);

    glPopMatrix();

    glPushMatrix();
    glTranslatef(-60.0f, 40.0f, 0.0f);

    drawModelFile(MODEL_ONE);

    glPopMatrix();
}

void animateFW(int val)
{

    int currTime = glutGet(GLUT_ELAPSED_TIME);
    int elapsedTime = currTime - prevTime;

    if(elapsedTime<gState.fuseCh)
    {
        double s, r, x1, z1; //vertical displacement and horizontal displacement

        //Calc the y coord (vertical displacement)
        s = pow((((gState.velocityCh*2.5) * (elapsedTime/1000)) + (0.5*GRAV*(elapsedTime/1000))),2.0);
        if(s>maxS)
        {
            maxS = s;
        }

        if(val!=5) //4, as 5-1 (due to first being 0)
        {
            //Calc the horizontal displacement assuming terminal velocity is infinite
            r = (gState.velocityCh*2.5) * cos(THETA*M_PI/180.0) * (elapsedTime/1000);

            //switch case for angle calcs
            switch(val)
            {
                case 1:
                    x1 = -1.0 * r * cos(corners);
                    z1 = r * sin(corners);
                    break;
                case 2:
                    x1 = 0;
                    z1 = r;
                    break;
                case 3:
                    x1 = r * cos(corners);
                    z1 = r * sin(corners);
                    break;
                case 4:
                    x1 = -r;
                    z1 = 0;
                    break;
                case 6:
                    x1 = r;
                    z1 = 0;
                    break;
                case 7:
                    x1 = -r * cos(corners);
                    z1 = -r * sin(corners);
                    break;
                case 8:
                    x1 = 0;
                    z1 = -r;
                    break;
                case 9:
                    x1 = r * cos(corners);
                    z1 = -r * cos(corners);
                    break;
                default:
                    break;
            }
        }

        if(gState.trackFW)
        {
            gluLookAt(x_loc, 5.0f, z_loc,
                x1 , maxS, z1,
              0.0f, 0.0f, 1.0f);
        }

        //generate firework at {x,s,z}
        glTranslatef((x1-(prevLoc[0])),(s-prevLoc[1]),(z1-prevLoc[2]));
        createFirework();
        prevLoc[0] = x1;
        prevLoc[1] = s;
        prevLoc[2] = z1;

        glutTimerFunc(TIMERSECS, animateFW, val);
    }
    else{
        //call fw detonation func

        maxS = 0;
    }
}

void rasterChar (const char* dWords, int xPos, int yPos, int r, int g, int b)
{
    int dLen, dCount;

    glColor3f(r, g, b);
    glRasterPos2f(xPos, yPos);

    dLen = strlen(dWords);
    for (dCount = 0; dCount < dLen; dCount++) {

        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, dWords[dCount]);

    }
}

void rasterInt(int dNum, int xPos, int yPos, int r, int g, int b)
{
    char dStr[100];

    glColor3f(r, g, b);
    glRasterPos2f(xPos, yPos);

    sprintf(dStr, "%d", dNum);

    int dLen, dCount;
    dLen = strlen(dStr);
    for (dCount = 0; dCount < dLen; dCount++) {

        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, dStr[dCount]);

    }
}

void mouseMove(int x, int y)
{
    // This will only be true when the left button is down
    if (xOrigin >= 0)
    {
        // Update deltaAngle
        deltaAngle = (x - xOrigin) * 0.001f;
        xOrigin = x;

        // Update camera's direction
        lx = sin(angle + deltaAngle);
        lz = -cos(angle + deltaAngle);

        glutPostRedisplay();
    }
}

void mouseButton(int button, int state, int x, int y)
{
    if(!uiMouseHandler(button, state, x, y)) //filter for UI -KM
    {
        // Only start motion if the left button is pressed
        if (button == GLUT_LEFT_BUTTON)
        {
            // When the button is released
            if (state == GLUT_UP)
            {
                angle += deltaAngle;
                deltaAngle = 0;
                xOrigin = -1;
            }else
            {
                xOrigin = x;
            }
        }
    }
}

void keyRelease(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT: deltaAngle = 0; break;
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN: deltaMove = 0; break;
    }
}

void keyUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 's': deltaMove = 0; break;
    case 'a':
    case 'd': deltaAngle = 0; break;
    }
}

void computeDir(float deltaAngle)
{
    angle += deltaAngle;
    lx = sin(angle);
    lz = -cos(angle);
}

void computePos(float deltaMove)
{
    SetLastLocation(x_loc, y_loc, z_loc, player_col);     //Store player's last location for collision resolution

    x_loc += deltaMove * lx * 0.1f;
    z_loc += deltaMove * lz * 0.1f;

    UpdateDynamic(x_loc - PLAYER_X, x_loc + PLAYER_X, y_loc - PLAYER_Y, y_loc + PLAYER_Y, z_loc - PLAYER_Z, z_loc + PLAYER_Z, player_col);      //Add modified player Xmin, Xmax, Ymin, Ymax, Zmin, Zmax to dynamic AABB array
    isColliding = CheckCollisions(player_col);       //Check player (0) collisions

    if (isColliding)
    {
        x_loc = ReturnXLoc(player_col);      //Return saved x location
        y_loc = ReturnYLoc(player_col);      //Return saved y location
        z_loc = ReturnZLoc(player_col);      //Return saved z location
        isColliding = false;
    }
}

void specialKeyPress(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_LEFT: deltaAngle = -0.01f; break;
    case GLUT_KEY_RIGHT: deltaAngle = 0.01f; break;
    case GLUT_KEY_UP: deltaMove = 0.5f; break;
    case GLUT_KEY_DOWN: deltaMove = -0.5f; break;
    }

    glutPostRedisplay();
}

void keyPress(unsigned char key, int x, int y)
{
    if(!uiKeysHandler(key, x, y)) //filter for UI -KM
    {
        switch (key)
        {
        case ESCAPE:
            gState.uiOptions = true;
            break;
        case 'f':
            gState.uiSettings = true;
            tmpState = gState; // temporary holder of settings to be confirmed on exit
            break;
        case 'w': deltaMove = 0.5f; break;
        case 's': deltaMove = -0.5f; break;
        case 'a': deltaAngle = -0.01f; break;
        case 'd': deltaAngle = 0.01f; break;
        case 'l':
            startTime = glutGet(GLUT_ELAPSED_TIME);
            prevTime = startTime;
            glutTimerFunc(TIMERSECS, animateFW, gState.angle);
            PositionParticles(0, 5, -20);
            ColourParticles(0.8f, 0.0f, 0.5f);
            ParticleSpread(75, 50, 75);
            InitParticles();
            break;
        }
    }

    glutPostRedisplay();

}

void changeSize(int w, int h)
{
    // Prevent a divide by zero
    if (h == 0)
        h = 1;

    float ratio = 1.0 * w / h;

    // Use the Projection matrix
    glMatrixMode(GL_PROJECTION);

    // Reset matrix
    glLoadIdentity();

    // Set the viewport to be the whole window
    glViewport(0, 0, w, h);

    // Set the correct perspective
    gluPerspective(45, ratio, 1, 1000);

    // Get back to Modelview
    glMatrixMode(GL_MODELVIEW);
}

void SetAABBs()
{
    //Dynamic AABBs
    //Player
    player_col = AddToDynamic(x_loc - PLAYER_X, x_loc + PLAYER_X, y_loc - PLAYER_Y, y_loc + PLAYER_Y, z_loc - PLAYER_Z, z_loc + PLAYER_Z);

    //Static AABBs
    //Red test box
    //AddToStatic(45.0, 55.0, 0.0, 20.0, 50.0, 70.0); //Add Object Xmin, Xmax, Ymin, Ymax, Zmin, Zmax for AABB collisions
    AddToStatic(-100.0,100.0,0.0,10,-110.0,-100.0); //Collision stuff for Back fence; front fence removed
    AddToStatic(-110.0,-100.0,0.0,10,-100.0,100.0); //Collision stuff for left fence
    AddToStatic(100.0,110.0,0.0,10,-100.0,100.0); //Collision stuff for right fence

    //Water/Pier Collisions
    AddToStatic(-100.0f,-5.0f, 0.0f,10.0f, 0.0f, 1.0f);
    AddToStatic(5.0f,100.0f, 0.0f,10.0f, 0.0f, 1.0f);
    AddToStatic(-6.0f,-5.0f, 0.0f,10.0f, 0.0f, 11.0f);
    AddToStatic(-6.0f,6.0f, 0.0f,10.0f, 7.5f, 11.0f); //Includes collisions with box, as box takes up most of pier end.
    AddToStatic(5.0f,6.0f, 0.0f,10.0f, 0.0f, 11.0f);

}

//EXPLODE FIREWORK FUNCTION
// //TO USE PARTICLES, SIMPLY USE THE FOLLOWING CALLS
// PositionParticles(0, 5, -20);
// ColourParticles(0.8f, 0.0f, 0.5f);
// ParticleSpread(75, 50, 75);
// InitParticles();
// //WHERE fx, fy and fz ARE THE X, Y AND Z COORDINATES OF THE FIREWORK
// //NEED BRANDON'S LAUNCH FUNCTIONS TO CONTINUE
// //CURRENTLY ALL PARTICLES ARE ONE COLOUR, WILL WORK ON CHANGING SAID COLOUR IF NECESSARY


void renderScene(void)
{

    if(!collisionsAdded)            //If static collisions not added to array
    {                               //NOTE: Must be initiated before computePos called
        SetAABBs();
        collisionsAdded = true;
    }

    if (deltaMove)
    {
        computePos(deltaMove);
        glutPostRedisplay();
    }
    if (deltaAngle)
    {
        computeDir(deltaAngle);
        glutPostRedisplay();
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(x_loc, y_loc, z_loc,
              x_loc + lx, 5.0f, z_loc + lz,
              0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.21f, 0.0f);
    //grass floor
    glBegin(GL_POLYGON);
        glVertex3f(-1000.0f, 0.0f, -1000.0f);
        glVertex3f(1000.0f, 0.0f, -1000.0f);
        glVertex3f(1000.0f, 0.0f, 0.0f);
        glVertex3f(-1000.0f, 0.0f, 0.0f);
        //glVertex3f(100.0f, 0.0f, 100.0f);
    glEnd();
    //water/ground cliff
    glBegin(GL_POLYGON);
        glVertex3f(-1000.0f, -0.05f, 0.0f);
        glVertex3f(-1000.0f, 0.0f, 0.0f);
        glVertex3f(1000.0f, 0.0f, 0.0f);
        glVertex3f(1000.0f, -0.05f, 0.0f);
    //water floor
    glColor3f(0.21f, 0.11f, 0.5f);
    glBegin(GL_POLYGON);
        glVertex3f(-1000.0f, -0.05f, 0.0f);
        glVertex3f(1000.0f, -0.05f, 0.0f);
        glVertex3f(1000.0f, -0.05f, 1000.0f);
        glVertex3f(-1000.0f, -0.05f,1000.0f);
    glEnd();
    //fence bits
    glColor3f(0.32f, 0.18f, 0.14f);
    glBegin(GL_QUADS); //Left Fence
        glVertex3f(-100.0f,0.0f,-100.0f);
        glVertex3f(-100.0f,0.0f,0.0f);
        glVertex3f(-100.0f,2.5f,0.0f);
        glVertex3f(-100.0f,2.5f,-100.0f);
    glEnd();
        //Front fence removed
    glBegin(GL_QUADS); //Right fence
        glVertex3f(100.0f,0.0f,100.0f);
        glVertex3f(100.0f,0.0f,-100.0f);
        glVertex3f(100.0f,2.5f,-100.0f);
        glVertex3f(100.0f,2.5f,100.0f);
    glEnd();
    glBegin(GL_QUADS); //Back Fence
        glVertex3f(-100.0f,0.0f,-100.0f);
        glVertex3f(100.0f,0.0f,-100.0f);
        glVertex3f(100.0f,2.5f,-100.0f);
        glVertex3f(-100.0f,2.5f,-100.0f);
    glEnd();
    //Pier
    glColor3f(0.25f,0.25f,0.25f);
    glBegin(GL_POLYGON); //Pier top
        glVertex3f(-5.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 0.0f);
        glVertex3f(5.0f, 0.0f, 10.0f);
        glVertex3f(-5.0f, 0.0f, 10.0f);
    glEnd();
    //pier sides
    glBegin(GL_POLYGON);
        glVertex3f(-5.0f,-0.05f,0.0f);
        glVertex3f(-5.0f,0.0f,0.0f);
        glVertex3f(-5.0f,0.0f,10.0f);
        glVertex3f(-5.0f,-0.05f,10.0f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(-5.0f,-0.05f,10.0f);
        glVertex3f(-5.0f,0.0f,10.0f);
        glVertex3f(5.0f,0.0f,10.0f);
        glVertex3f(5.0f,-0.05f,10.0f);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(5.0f,-0.05f,10.0f);
        glVertex3f(5.0f,0.0f,10.0f);
        glVertex3f(5.0f,0.0f,0.0f);
        glVertex3f(5.0f,-0.05f,0.0f);
    glEnd();
    //fireworks box
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_POLYGON); //Box Top
        glVertex3f(-3.75f,2.5,10.0f);
        glVertex3f(-3.75f,2.5,7.5f);
        glVertex3f(3.75f,2.5,7.5f);
        glVertex3f(3.75f,2.5,10.0f);
    glEnd();
    glBegin(GL_POLYGON); //Box Left
        glVertex3f(-3.75f,2.5,10.0f);
        glVertex3f(-3.75f,0.0,10.0f);
        glVertex3f(-3.75f,0.0,7.5f);
        glVertex3f(-3.75f,2.5,7.5f);
    glEnd();
    glBegin(GL_POLYGON); //Box Front
        glVertex3f(-3.75f,2.5,7.5f);
        glVertex3f(-3.75f,0.0,7.5f);
        glVertex3f(3.75f,0.0,7.5f);
        glVertex3f(3.75f,2.5,7.5f);
    glEnd();
    glBegin(GL_POLYGON); //Box Right
        glVertex3f(3.75f,2.5,7.5f);
        glVertex3f(3.75f,0.0,7.5f);
        glVertex3f(3.75f,0.0,10.0f);
        glVertex3f(3.75f,2.5,10.0f);
    glEnd();
    glBegin(GL_POLYGON); //Box Back
        glVertex3f(3.75f,2.5,10.0f);
        glVertex3f(3.75f,0.0,10.0f);
        glVertex3f(-3.75f,0.0,10.0f);
        glVertex3f(-3.75f,2.5,10.0f);
    glEnd();

    glTranslatef(0.0f, 0.0f, 8.75f);

    for(int i = -3; i < 3; i++)
        for(int j = -3; j < 3; j++)
    {
        glPushMatrix();

        glTranslatef(i * 20.0, 0, j * 20.0);
        glScalef(0.05f, 0.05f, 0.05f);

        //drawHouse();          Commented out to remove all house instances from project. Leaves blank "test" ground

        glPopMatrix();
    }

    glPushMatrix();
    //glScalef(5, 5, 5);
    glRotatef(270, 1.0, 0, 0);
    glTranslatef(0, 2, 0);
    glCallList(FIREWORK);
    glPopMatrix();

    // readModels();      Commented out as we don't want bones either KM

    // drawModels();      Commented out as we don't want bones either KM

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);

    if (time - timebase > 1000)
    {
        fps = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
        //y -= 1;
        glutPostRedisplay();
    }

    glPushMatrix();
    DrawParticles();
    glPopMatrix();

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, wRes, hRes, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    rasterInt(fps, 50, 50, 200, 0, 0);
    rasterInt(x_loc, 150, 50, 0, 100, 0);
    rasterInt(y_loc, 200, 50, 0, 100, 0);
    rasterInt(z_loc, 250, 50, 0, 100, 0);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    if(gState.uiSettings)
        displayUISettings();
    if(gState.uiOptions)
        displayUIOptions();
    if(gState.uiQuit)
        displayUIQuit();

    glutSwapBuffers();
}

void myInit()
{

    if (!LoadGLTextures())
    {
        printf("Textures not loaded");
    }

    createFirework();

    //set Sky colour
    //glClearColor(0.32f, 0.21f, 0.53f, 0.0f); pansy
    glClearColor(0.21f, 0.11f, 0.43, 0.0f);
    // Register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);
    glutKeyboardFunc(keyPress);
    glutKeyboardUpFunc(keyUp);
    glutSpecialFunc(specialKeyPress);
    glutIgnoreKeyRepeat(1);
    glutSpecialUpFunc(keyRelease);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);
}

int main(int argc, char **argv)
{
    // Init GLUT and create window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(5, 5);
    glutInitWindowSize(wRes, hRes);
    glutCreateWindow("ICT289 Project");

    // Register Callbacks
    myInit();

    // OpenGL Init
    glEnable(GL_DEPTH_TEST);

    // Enter GLUT event processing cycle
    glutMainLoop();

    return(0);
}
