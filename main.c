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

#define ESCAPE          27
#define MAX_FILE_NAME   20
#define MODEL_ONE       0

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
int cont[] ={0};
int startTime;

int wRes = 1600; int hRes = 1000;
// Angle of rotation for camera direction
float angle = 0.0f;
// Actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = -5.0f, z = 0.0f;
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

void animateFW(int * val) //val[0] = number of fireworks; val[1] = tube number;
{
    if(cont[val[0]]!=0)
    {
        int currTime = glutGet(GLUT_ELAPSED_TIME);
        int elapsedTime = currTime - startTime;

        if(elapsedTime<gState.fuseCh)
        {
            double s, r, x, z; //vertical displacement and horizontal displacement

            //Calc the y coord (vertical displacement)
            s = pow(((gState.velocityCh * (elapsedTime/1000)) + (0.5*GRAV*(elapsedTime/1000))),2.0);

            if(val[1]!=4) //4, as 5-1 (due to first being 0)
            {
                //Calc the horizontal displacement assuming terminal velocity is infinite
                r = gState.velocityCh * Math.cos(THETA) * (elapsedTime/1000);

                //switch case for angle calcs
                switch(val[1])
                {
                    case 0:
                        x = -1.0 * r * cos(corners);
                        z = r * sin(corners);
                        break;
                    case 1:
                        x = 0;
                        z = r;
                        break;
                    case 2:
                        x = r * cos(corners);
                        z = r * sin(corners);
                        break;
                    case 3:
                        x = -r;
                        z = 0;
                        break;
                    case 5:
                        x = r;
                        z = 0;
                        break;
                    case 6:
                        x = -r * cos(corners);
                        z = -r * sin(corners);
                        break;
                    case 7:
                        x = 0;
                        z = -r;
                        break;
                    case 8:
                        x = r * cos(corners);
                        z = -r * cos(corners);
                        break;
                    default:
                        break;
                }
            }

            //generate firework at {x,s,z}

            glutTimerFunc(TIMERSECS, animateFW, val);
        }
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
    SetLastLocation(x, 5.0f, z, player_col);     //Store player's last location for collision resolution

    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;

    UpdateDynamic(x - PLAYER_X, x + PLAYER_X, 5.0f - PLAYER_Y, 5.0f + PLAYER_Y, z - PLAYER_Z, z + PLAYER_Z, player_col);      //Add modified player Xmin, Xmax, Ymin, Ymax, Zmin, Zmax to dynamic AABB array
    isColliding = CheckCollisions(player_col);       //Check player (0) collisions

    if (isColliding)
    {
        x = ReturnXLoc(player_col);      //Return saved x location
        //y = ReturnYLoc(player_col);      //Return saved y location
        z = ReturnZLoc(player_col);      //Return saved z location
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
    player_col = AddToDynamic(x - PLAYER_X, x + PLAYER_X, 5.0f - PLAYER_Y, 5.0f + PLAYER_Y, z - PLAYER_Z, z + PLAYER_Z);

    //Static AABBs
    //Red test box
    //AddToStatic(45.0, 55.0, 0.0, 20.0, 50.0, 70.0); //Add Object Xmin, Xmax, Ymin, Ymax, Zmin, Zmax for AABB collisions
    AddToStatic(-100.0,100.0,0.0,10,-110.0,-100.0); //Collision stuff for Back fence
    AddToStatic(-100.0,100.0,0.0,10,100.0,110.0); //Collision stuff for Front fence
    AddToStatic(-110.0,-100.0,0.0,10,-100.0,100.0); //Collision stuff for left fence
    AddToStatic(100.0,110.0,0.0,10,-100.0,100.0); //Collision stuff for right fence

    //Collisions with water edge not including pier? don't know if it has to be so complex
    float pierLeft = (-2.5*cos(corners)), pierRight = -pierLeft;
    float i;
    for(i=-100.0f;i<=100.0f;i=i+0.1)
    {
        float xMin = (i*cos(corners)),zMin = (-i*sin(corners));
        if((i<(pierLeft))||(i>pierRight))
        {
            AddToStatic(xMin,(xMin+0.1),0.0,10,zMin,(zMin+0.1)); //Creates 10unit high 0.1x0.1 blocks to impede movement
        }
    }

    //Pier collisions
    float pierLeftz = (-2.5*sin(corners));
    for(i=0.0f;i<=10.0f;i=i+0.1)
    {
        float xMax = (pierLeft+(i*cos(corners))), zMax = (pierLeftz+(i*sin(corners)));
        AddToStatic(xMax-0.1,xMax,0.0,10.0,zMax-0.1,zMax); //Creates 10unit high 0.1x0.1 blocks to impede movement
    }
    float pierRightz = -pierLeftz;
    for(i=0.0f;i<=10.0f;i=i+0.1)
    {
        float xMin = (pierRight+(i*cos(corners))), zMin = (pierRightz+(i*sin(corners)));
        AddToStatic(xMin, xMin+0.1,0.0,10.0,zMin,zMin+0.1); //Creates 10unit high 0.1x0.1 blocks to impede movement
    }
    //Collisions for end of pier; Includes FW box
    int itvl = 50, j;
    float pierLeftx = (pierLeft+(7.5*cos(corners))), pierRightx = (pierRight+(10*cos(corners)));
    float interv = (pierRightx-pierLeftx)/(float)itvl;
    float pierRightz2 = (pierRightz+(7.5*sin(corners))), pierLeftz2 = (pierLeftz+(10*sin(corners)));
    float intervz = (pierRightz2-pierLeftz2)/(float)itvl;

    for(j=0;j<=50;j++)
    {
        float xx = pierLeftx+interv, zz=pierLeftz2+intervz;
        AddToStatic(xx,xx+0.1,0.0,10.0,zz,zz+0.1); //Creates 10unit high 0.1x0.1 blocks to impede movement
    }

}

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

    gluLookAt(x, 5.0f, z,
              x + lx, 5.0f, z + lz,
              0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.21f, 0.0f);
    //grass floor
    glBegin(GL_POLYGON);
        glVertex3f(-1000.0f, 0.0f, -1000.0f);
        glVertex3f(1000.0f, 0.0f, -1000.0f);
        glVertex3f(-1000.0f, 0.0f, 1000.0f);
        //glVertex3f(100.0f, 0.0f, 100.0f);
    glEnd();
    //water floor
    glColor3f(0.21f, 0.11f, 0.5f);
    glBegin(GL_POLYGON);
        glVertex3f(-1000.0f, -0.05f, 1000.0f);
        glVertex3f(1000.0f, -0.05f, 1000.0f);
        glVertex3f(1000.0f, -0.05f, -1000.0f);
    glEnd();
    //fence bits
    glColor3f(0.32f, 0.18f, 0.14f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f,0.0f,-100.0f);
        glVertex3f(-100.0f,0.0f,100.0f);
        glVertex3f(-100.0f,2.5f,100.0f);
        glVertex3f(-100.0f,2.5f,-100.0f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(-100.0f,0.0f,100.0f);
        glVertex3f(100.0f,0.0f,100.0f);
        glVertex3f(100.0f,2.5f,100.0f);
        glVertex3f(-100.0f,2.5f,100.0f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(100.0f,0.0f,100.0f);
        glVertex3f(100.0f,0.0f,-100.0f);
        glVertex3f(100.0f,2.5f,-100.0f);
        glVertex3f(100.0f,2.5f,100.0f);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(-100.0f,0.0f,-100.0f);
        glVertex3f(100.0f,0.0f,-100.0f);
        glVertex3f(100.0f,2.5f,-100.0f);
        glVertex3f(-100.0f,2.5f,-100.0f);
    glEnd();
    //Pier
    float z1 = (2.5*sin(corners)),x2 = (2.5*cos(corners)),z2=-z1,x1=-x2; //first set of pier coords
    float z3 = (z2+(10*cos(corners))), x3 = (x2+(10*sin(corners))), z4 = (z1+(10*cos(corners))), x4=(x1+(10*sin(corners))); //second set of pier coords
    glBegin(GL_POLYGON); //Pier top
        glVertex3f(x1,0.0f,z1);
        glVertex3f(x2,0.0f,z2);
        glVertex3f(x3,0.0f,z4);
        glVertex3f(x4,0.0f,z4);
    glEnd();
    //pier sides
    glBegin(GL_POLYGON);
        glVertex3f(x1,-0.05f,z1);
        glVertex3f(x1,0.0f,z1);
        glVertex3f(x4,0.0f,z4);
        glVertex3f(x4,-0.05f,z4);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x4,-0.05f,z4);
        glVertex3f(x4,0.0f,z4);
        glVertex3f(x3,0.0f,z3);
        glVertex3f(x3,-0.05f,z3);
    glEnd();
    glBegin(GL_POLYGON);
        glVertex3f(x3,-0.05f,z3);
        glVertex3f(x3,0.0f,z3);
        glVertex3f(x2,0.0f,z2);
        glVertex3f(x2,-0.05f,z2);
    glEnd();
    //fireworks box
    float c1 = (x4+(1.25*sin(corners))), c2 = (c1-(2.5*sin(corners))), c3 = (c2+(2.5*cos(corners))), c4 = (c3+(2.5*cos(corners)));
    float cz1 = (z4-(1.25*cos(corners))), cz2 = (cz1-(2.5*cos(corners))), cz3 = (cz2-(2.5*sin(corners))), cz4 = (cz3+(2.5*sin(corners)));
    glBegin(GL_POLYGON); //Box Top
        glVertex3f(c1,2.5,cz1);
        glVertex3f(c2,2.5,cz2);
        glVertex3f(c3,2.5,cz3);
        glVertex3f(c4,2.5,cz4);
    glEnd();
    glBegin(GL_POLYGON); //Box Left
        glVertex3f(c1,2.5,cz1);
        glVertex3f(c1,0.0,cz1);
        glVertex3f(c2,0.0,cz2);
        glVertex3f(c2,2.5,cz2);
    glEnd();
    glBegin(GL_POLYGON); //Box Front
        glVertex3f(c2,2.5,cz2);
        glVertex3f(c2,0.0,cz2);
        glVertex3f(c3,0.0,cz3);
        glVertex3f(c3,2.5,cz3);
    glEnd();
    glBegin(GL_POLYGON); //Box Right
        glVertex3f(c3,2.5,cz3);
        glVertex3f(c3,0.0,cz3);
        glVertex3f(c4,0.0,cz4);
        glVertex3f(c4,2.5,cz4);
    glEnd();
    glBegin(GL_POLYGON); //Box Back
        glVertex3f(c4,2.5,cz4);
        glVertex3f(c4,0.0,cz4);
        glVertex3f(c1,0.0,cz1);
        glVertex3f(c1,2.5,cz1);
    glEnd();

    /*
        //Red test box for collisions -- Leave as example for now
        glColor3f(0.5f, 0.0f, 0.0f);
        glBegin(GL_QUADS);
            glVertex3f(45.0f, 0.0f, 50.0f);
            glVertex3f(55.0f, 0.0f, 50.0f);
            glVertex3f(55.0f, 20.0f, 50.0f);
            glVertex3f(45.0f, 20.0f, 50.0f);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(55.0f, 0.0f, 50.0f);
            glVertex3f(55.0f, 0.0f, 70.0f);
            glVertex3f(55.0f, 20.0f, 70.0f);
            glVertex3f(55.0f, 20.0f, 50.0f);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(45.0f, 0.0f, 70.0f);
            glVertex3f(55.0f, 0.0f, 70.0f);
            glVertex3f(55.0f, 20.0f, 70.0f);
            glVertex3f(45.0f, 20.0f, 70.0f);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3f(45.0f, 0.0f, 50.0f);
            glVertex3f(45.0f, 0.0f, 70.0f);
            glVertex3f(45.0f, 20.0f, 70.0f);
            glVertex3f(45.0f, 20.0f, 50.0f);
        glEnd();
*/
    for(int i = -3; i < 3; i++)
        for(int j = -3; j < 3; j++)
    {
        glPushMatrix();

        glTranslatef(i * 20.0, 0, j * 20.0);
        glScalef(0.05f, 0.05f, 0.05f);

        //drawHouse();          Commented out to remove all house instances from project. Leaves blank "test" ground

        glPopMatrix();
    }

    // readModels();      Commented out as we don't want bones either KM

    // drawModels();      Commented out as we don't want bones either KM

    frame++;
    time = glutGet(GLUT_ELAPSED_TIME);

    if (time - timebase > 1000)
    {
        fps = frame*1000.0/(time-timebase);
        timebase = time;
        frame = 0;
        glutPostRedisplay();
    }


    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, wRes, hRes, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    rasterInt(fps, 50, 50, 200, 0, 0);
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
