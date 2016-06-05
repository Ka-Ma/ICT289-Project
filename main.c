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
float lx = 0.0f, lz = -1.0f, ly = 5.0f;
// XZ position of the camera
float x_loc = -5.0f, y_loc = 5.0f, z_loc = -10.0f;
// XYZ of up vector
float ux=0.0f, uy=1.0f, uz=0.0f;
// XYZ of right vector for up vec calcs
float rx=0.0f, ry = 0.0f, rz=0.0f;
// Key states. Will be 0 when no keys are being pressed
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1, yOrigin = -1;
float vertAngle = 0.0f, vAngle = 0.0f;
float a = 0.0f, b = 0.0f, c = 0.0f;
double x1, s, z1;

float corners = 45 * M_PI / 180.0;

int t;
int frame = 0, time, timebase = 0;
int fps;

bool isFiring = false;

//Dynamic collision indexes
int player_col;

bool filesUnRead = true;

bool collisionsAdded = false;
bool isColliding = false;

void createFirework()
{
    GLUquadricObj *quadObj;

    glNewList(FIREWORK, GL_COMPILE);
    quadObj = gluNewQuadric();
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    gluCylinder(quadObj, 0.6, 0.6, 0.4, 24, 4);
    gluDisk(quadObj, 0, 0.6, 24, 5);
    glTranslatef(0, 0, 1);
    gluSphere(quadObj, 0.9, 24, 24);
    glEndList();
}

void readModels()
{
    if (filesUnRead)
    {
        char* status;

        printf("Loading Duck.off...");
        status = readModelFile("Data/Duck.off", MODEL_ONE);
        printf(status);

        filesUnRead = false;
    }
}
void FWBoom(float x, float s, float z)
{
    PositionParticles(x, s, z);
    ColourParticles(gState.colour[0], gState.colour[1], gState.colour[2]);
    ParticleSpread(75, 50, 75);
    InitParticles();
}
void drawModels()
{
    glPushMatrix();
    //glTranslatef(-30.0f, 40.0f, 0.0f);

    drawModelFile(MODEL_ONE);

    glPopMatrix();

    glPushMatrix();
    //glTranslatef(-60.0f, 40.0f, 0.0f);

    drawModelFile(MODEL_ONE);

    glPopMatrix();
}

void animateFW(int val)
{

    int currTime = glutGet(GLUT_ELAPSED_TIME);
    //getchar();
    int elapsedTime = currTime - startTime;
    //getchar();

    double r=0.0; //vertical displacement and horizontal displacement

    //Calc the y coord (vertical displacement)
    s = pow((((gState.velocityCh*2.5) * (elapsedTime/1000.0)) + (0.5*GRAV*(elapsedTime/1000.0))),2.0);
    if(s>maxS)
    {
        maxS = s;
    }

    if(gState.angle!=5) //4, as 5-1 (due to first being 0)
    {
        //Calc the horizontal displacement assuming terminal velocity is infinite
        r = (gState.velocityCh*2.5) * cos(THETA*M_PI/180.0) * (elapsedTime/1000.0);

        //switch case for angle calcs
        switch(val)
        {
            case 1:
                x1 = -1.0 * r * cos(corners);
                z1 = r * sin(corners);
                break;
            case 4:
                x1 = 0;
                z1 = r;
                break;
            case 7:
                x1 = r * cos(corners);
                z1 = r * sin(corners);
                break;
            case 2:
                x1 = -r;
                z1 = 0;
                break;
            case 8:
                x1 = r;
                z1 = 0;
                break;
            case 3:
                x1 = -r * cos(corners);
                z1 = -r * sin(corners);
                break;
            case 6:
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
    else
    {
        x1 = 0.0f;
        z1 = 0.0f;
    }

    a = x1;
    b = s;
    c = z1;

    if(elapsedTime<gState.fuseCh)
    {
        //generate firework at {x,s,z}
        //glTranslatef((x1-(prevLoc[0])),(s-prevLoc[1]),(z1-prevLoc[2]));

        glutPostRedisplay();

        glutTimerFunc(TIMERSECS, animateFW, val);
    }
    else{
        //call fw detonation func
        FWBoom(x1,s,z1);
        maxS = 0;
        isFiring = false;
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


//temporary removal
void mouseMove(int x, int y)
{
    // This will only be true when the left button is down
    if (xOrigin >= 0)
    {
        // Update deltaAngle
        deltaAngle = (x - xOrigin) * 0.001f;
        xOrigin = x;

        lx = sin(angle + deltaAngle);
        lz = cos(angle + deltaAngle);

        glutPostRedisplay();
    }

    if(yOrigin >= 0)
    {
        vAngle = (y - yOrigin) * 0.001f;
        yOrigin = y;
        // Update camera's direction
        if(!(vAngle>90||vAngle<-90))
        {
            ly += sin(vAngle);
        }

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
                yOrigin = -1;
            }else
            {
                xOrigin = x;
                yOrigin = y;
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
            isFiring = true;
            if(gState.angleRand)
            {
                gState.angle = (rand()%9)+1;
            }
            glutTimerFunc(TIMERSECS, animateFW, gState.angle);
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

void renderScene(void)
{
    //normal for object being lit
    float normal[3];

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


    //Sun position before lookat so it doesn't move with lookat
    GLfloat sun_position[] = {0,3,-10,0}; // at horizon
    GLfloat sun_specular[] = {.1,.1,.1,1};
    GLfloat sun_ambient[] = {.25,.25,.25,1};
    GLfloat sun_diffuse[] = {1.0,1.0,1.0,1}; //white light
    glLightfv(GL_LIGHT0, GL_POSITION, sun_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_diffuse); //colour of the light
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_ambient);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    if(!gState.trackFW){
        gluLookAt(x_loc, y_loc, z_loc,
              x_loc + lx, ly, z_loc + lz,
              ux, uy, uz);
    } else {
        gluLookAt(x_loc, y_loc, z_loc,  a, b, c,  ux, uy, uz);
    }


    //materials for lighting (note that because most of the surfaces being lit are one big surface they don't light well
    //colours
    GLfloat grassClr[] = {0.0f, 0.21f, 0.0f, 0.0f};
    GLfloat waterClr[] = {0.21f, 0.11f, 0.5f, 0.0f};
    GLfloat fenceClr[] = {0.32f, 0.18f, 0.14f, 0.0f};
    GLfloat pierClr[] = {0.25f,0.25f,0.25f, 0.0f};
    GLfloat boxClr[] = {0.0f,0.0f,0.0f, 0.0f};
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 0.0f};

    //grass floor
    glColor3fv(grassClr);
    point3 grassV[4] = {{-1000.0f, 0.0f, -1000.0f}, {1000.0f, 0.0f, -1000.0f}, {1000.0f, 0.0f, 0.0f}, {-1000.0f, 0.0f, 0.0f}};
    calcNormal(grassV[1], grassV[0], grassV[2], normal);
    glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, grassClr);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glVertex3fv(grassV[0]);
        glVertex3fv(grassV[1]);
        glVertex3fv(grassV[2]);
        glVertex3fv(grassV[3]);
    glEnd();
    //water/ground cliff
    point3 wtrGrdV[4] = {{-1000.0f, -0.05f, 0.0f}, {-1000.0f, 0.0f, 0.0f}, {1000.0f, 0.0f, 0.0f}, {1000.0f, -0.05f, 0.0f}};
    calcNormal(wtrGrdV[0], wtrGrdV[1], wtrGrdV[2], normal);
    glColor3fv(grassClr);
    glBegin(GL_POLYGON);
        glNormal3fv(normal);
        glVertex3fv(wtrGrdV[0]);
        glVertex3fv(wtrGrdV[1]);
        glVertex3fv(wtrGrdV[2]);
        glVertex3fv(wtrGrdV[3]);
    glEnd();
    //water floor
    point3 waterV[4] = {{-1000.0f, -0.05f, 0.0f}, {1000.0f, -0.05f, 0.0f}, {1000.0f, -0.05f, 1000.0f}, {-1000.0f, -0.05f,1000.0f}};
    calcNormal(waterV[0], waterV[1], waterV[2], normal);
    glColor3fv(waterClr);
    glBegin(GL_POLYGON);
        glNormal3f(0,1,0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, waterClr);
        //glMaterialf(GL_FRONT, GL_SHININESS, 100);
        glVertex3fv(waterV[0]);
        glVertex3fv(waterV[1]);
        glVertex3fv(waterV[2]);
        glVertex3fv(waterV[3]);
    glEnd();
    //fence bits
    point3 fenceLV[4] = {{-100.0f,0.0f,-100.0f}, {-100.0f,0.0f,0.0f}, {-100.0f,2.5f,0.0f}, {-100.0f,2.5f,-100.0f}};
    calcNormal(fenceLV[1], fenceLV[0], fenceLV[2], normal);
    glColor3fv(fenceClr);
    glBegin(GL_QUADS); //Left Fence
        glNormal3fv(normal);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, fenceClr);
        glVertex3fv(fenceLV[0]);
        glVertex3fv(fenceLV[1]);
        glVertex3fv(fenceLV[2]);
        glVertex3fv(fenceLV[3]);
    glEnd();
    point3 fenceFV[4] = {{-100.0f,0.0f,0.0f}, {-5.0f,0.0f,0.0f}, {-5.0f,2.5f,0.0f}, {-100.0f,2.5f,0.0f}};
    glBegin(GL_QUADS); //Water Fence
        glNormal3fv(normal);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, fenceClr);
        glVertex3fv(fenceFV[0]);
        glVertex3fv(fenceFV[1]);
        glVertex3fv(fenceFV[2]);
        glVertex3fv(fenceFV[3]);
    glEnd();
    point3 fenceFV2[4] = {{100.0f,0.0f,0.0f}, {5.0f,0.0f,0.0f}, {5.0f,2.5f,0.0f}, {100.0f,2.5f,0.0f}};
    glBegin(GL_QUADS); //Water Fence
        glNormal3fv(normal);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, fenceClr);
        glVertex3fv(fenceFV2[0]);
        glVertex3fv(fenceFV2[1]);
        glVertex3fv(fenceFV2[2]);
        glVertex3fv(fenceFV2[3]);
    glEnd();
    //Front fence removed
    //Right fence
    point3 fenceRV[4] = {{100.0f,0.0f,0.0f}, {100.0f,0.0f,-100.0f}, {100.0f,2.5f,-100.0f}, {100.0f,2.5f,0.0f}};
    calcNormal(fenceRV[1], fenceRV[0], fenceRV[2], normal);
    glBegin(GL_QUADS);
        glNormal3fv(normal);
        glVertex3fv(fenceRV[0]);
        glVertex3fv(fenceRV[1]);
        glVertex3fv(fenceRV[2]);
        glVertex3fv(fenceRV[3]);
    glEnd();
    //Back Fence
    point3 fenceBV[4] = {{-100.0f,0.0f,-100.0f}, {100.0f,0.0f,-100.0f}, {100.0f,2.5f,-100.0f}, {-100.0f,2.5f,-100.0f}};
    calcNormal(fenceBV[0], fenceBV[1], fenceBV[2], normal);
    glBegin(GL_QUADS);
        glNormal3fv(normal);
        glVertex3fv(fenceBV[0]);
        glVertex3fv(fenceBV[1]);
        glVertex3fv(fenceBV[2]);
        glVertex3fv(fenceBV[3]);
    glEnd();
    //Pier
    point3 pierV[4] = {{-5.0f, 0.0f, 0.0f}, {5.0f, 0.0f, 0.0f}, {5.0f, 0.0f, 10.0f}, {-5.0f, 0.0f, 10.0f}};
    calcNormal(pierV[0], pierV[1], pierV[2], normal);
    glColor3fv(pierClr);
    glBegin(GL_POLYGON); //Pier top
        glNormal3f(0,1,0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, pierClr);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glVertex3fv(pierV[0]);
        glVertex3fv(pierV[1]);
        glVertex3fv(pierV[2]);
        glVertex3fv(pierV[3]);
    glEnd();
    //pier sides
    point3 pierSV[12] = {{-5.0f,-0.05f,0.0f}, {-5.0f,0.0f,0.0f}, {-5.0f,0.0f,10.0f}, {-5.0f,-0.05f,10.0f},
                        {-5.0f,-0.05f,10.0f}, {-5.0f,0.0f,10.0f}, {5.0f,0.0f,10.0f}, {5.0f,-0.05f,10.0f},
                        {5.0f,-0.05f,10.0f}, {5.0f,0.0f,10.0f}, {5.0f,0.0f,0.0f}, {5.0f,-0.05f,0.0f}};
    calcNormal(pierSV[1], pierSV[0], pierSV[2], normal);
    glBegin(GL_POLYGON);
        glNormal3fv(normal);
        glVertex3fv(pierSV[0]);
        glVertex3fv(pierSV[1]);
        glVertex3fv(pierSV[2]);
        glVertex3fv(pierSV[3]);
    glEnd();
    calcNormal(pierSV[5], pierSV[4], pierSV[6], normal);
    glBegin(GL_POLYGON);
        glNormal3fv(normal);
        glVertex3fv(pierSV[4]);
        glVertex3fv(pierSV[5]);
        glVertex3fv(pierSV[6]);
        glVertex3fv(pierSV[7]);
    glEnd();
    calcNormal(pierSV[9], pierSV[8], pierSV[10], normal);
    glBegin(GL_POLYGON);
        glNormal3fv(normal);
        glVertex3fv(pierSV[8]);
        glVertex3fv(pierSV[9]);
        glVertex3fv(pierSV[10]);
        glVertex3fv(pierSV[11]);
    glEnd();
    //fireworks box
    point3 fwBoxV[20] = {{-3.75f,2.5,10.0f}, {-3.75f,2.5,7.5f}, {3.75f,2.5,7.5f}, {3.75f,2.5,10.0f}, //top
                        {-3.75f,2.5,10.0f}, {-3.75f,0.0,10.0f}, {-3.75f,0.0,7.5f}, {-3.75f,2.5,7.5f}, //left
                        {-3.75f,2.5,7.5f}, {-3.75f,0.0,7.5f}, {3.75f,0.0,7.5f}, {3.75f,2.5,7.5f}, //front
                        {3.75f,2.5,7.5f}, {3.75f,0.0,7.5f}, {3.75f,0.0,10.0f}, {3.75f,2.5,10.0f}, //right
                        {3.75f,2.5,10.0f}, {3.75f,0.0,10.0f}, {-3.75f,0.0,10.0f}, {-3.75f,2.5,10.0f}}; //back
    calcNormal(fwBoxV[1], fwBoxV[0], fwBoxV[2], normal);
    glColor3fv(boxClr);
    glBegin(GL_POLYGON); //Box Top
        glNormal3f(0,1,0);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, boxClr);
        glMaterialfv(GL_FRONT, GL_AMBIENT, white);
        glMaterialfv(GL_FRONT, GL_SPECULAR, white);
        glVertex3fv(fwBoxV[0]);
        glVertex3fv(fwBoxV[1]);
        glVertex3fv(fwBoxV[2]);
        glVertex3fv(fwBoxV[3]);
    glEnd();
    calcNormal(fwBoxV[5], fwBoxV[4], fwBoxV[6], normal);
    glBegin(GL_POLYGON); //Box Left
        glNormal3fv(normal);
        glVertex3fv(fwBoxV[4]);
        glVertex3fv(fwBoxV[5]);
        glVertex3fv(fwBoxV[6]);
        glVertex3fv(fwBoxV[7]);
    glEnd();
    calcNormal(fwBoxV[9], fwBoxV[8], fwBoxV[10], normal);
    glBegin(GL_POLYGON); //Box Front
        glNormal3fv(normal);
        glVertex3fv(fwBoxV[8]);
        glVertex3fv(fwBoxV[9]);
        glVertex3fv(fwBoxV[10]);
        glVertex3fv(fwBoxV[11]);
    glEnd();
    calcNormal(fwBoxV[13], fwBoxV[12], fwBoxV[14], normal);
    glBegin(GL_POLYGON); //Box Right
        glNormal3fv(normal);
        glVertex3fv(fwBoxV[12]);
        glVertex3fv(fwBoxV[13]);
        glVertex3fv(fwBoxV[14]);
        glVertex3fv(fwBoxV[15]);
    glEnd();
    calcNormal(fwBoxV[17], fwBoxV[16], fwBoxV[18], normal);
    glBegin(GL_POLYGON); //Box Back
        glNormal3fv(normal);
        glVertex3fv(fwBoxV[16]);
        glVertex3fv(fwBoxV[17]);
        glVertex3fv(fwBoxV[18]);
        glVertex3fv(fwBoxV[19]);
    glEnd();

    glTranslatef(0.0f, 02.5f, 8.75f);

    glPushMatrix();
    glScalef(3, 3, 3);
    glTranslatef(10, 0, 10);
    glRotatef(90, 0, 1, 0);
    glColor3f(255, 255, 0);
    drawModels();
    glPopMatrix();

    //firework shell material properties
    glMaterialfv(GL_FRONT, GL_DIFFUSE, fenceClr);
    glMaterialfv(GL_FRONT, GL_AMBIENT, white);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 10);

    glPushMatrix();
    if (isFiring)
    {
        GLUquadricObj * quad;
        quad = gluNewQuadric();
        glTranslatef(x1, s, z1);
        prevLoc[0] = x1;
        prevLoc[1] = s;
        prevLoc[2] = z1;
        glRotatef(270, 1.0, 0, 0);
        gluCylinder(quad, 0.6, 0.6, 0.4, 24, 4);
        gluDisk(quad, 0, 0.6, 24, 5);
        glTranslatef(0, 0, 1);
        gluSphere(quad, 0.9, 24, 24);
    }
    glPopMatrix();


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
    //will need to disable lighting here for firework particles & UI KM
    glDisable(GL_LIGHTING);

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

    //if UI states are true draw UI now KM
    if(gState.uiSettings)
        displayUISettings();
    if(gState.uiOptions)
        displayUIOptions();
    if(gState.uiQuit)
        displayUIQuit();

    //will need to re-enable lighting here for rest of world KM
    glEnable(GL_LIGHTING);

    glutSwapBuffers();
}

void myInit()
{

    if (!LoadGLTextures())
    {
        printf("Textures not loaded");
    }

    createFirework();

    readModels();

    //set Sky colour
    //glClearColor(0.32f, 0.21f, 0.53f, 0.0f); pansy
    glClearColor(0.21f, 0.11f, 0.43, 0.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

     //Enable lighting
    glEnable (GL_LIGHTING); //
    glEnable (GL_LIGHT0); // sun
    //glEnable (GL_LIGHT1); // firework centroid
    glShadeModel(GL_SMOOTH);

    GLfloat fw_position[] = {x_loc,y_loc,z_loc,1}; // at firework centroid but following camera for now
    GLfloat fw_specular[] = {.1,.1,.1,1};
    GLfloat fw_ambient[] = {.1,.1,.1,1};
    GLfloat fw_diffuse[] = {1.0,1.0,1.0,1}; //white light for now
    glLightfv(GL_LIGHT1, GL_POSITION, fw_position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, fw_diffuse); //colour of the firework
    glLightfv(GL_LIGHT1, GL_SPECULAR, fw_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, fw_ambient);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 1.0);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.5);

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
    glutCreateWindow("ICT289 Project - Team 8");

    // Register Callbacks
    myInit();

    // OpenGL Init
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColorMaterial(GL_FRONT_AND_BACK, GL_EMISSION);

    // Enter GLUT event processing cycle
    glutMainLoop();

    return(0);
}
