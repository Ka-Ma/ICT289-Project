#include <stdio.h>
#include <stdlib.h>
#include <GL/freeglut.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>

#include "objloader.h"
#include "globalState.h" // global variables for fireworks settings and UI display states KM
#include "uiKeysHandler.h"
#include "uiMouseHandler.h"
#include "uiSettings.h"
#include "uiOptions.h"
#include "uiQuit.h"

#define ESCAPE          27
#define MAX_FILE_NAME   20
#define MODEL_ONE       0

int wRes = 1600; int hRes = 1000;
// Angle of rotation for camera direction
float angle = 0.0f;
// Actual vector representing the camera's direction
float lx = 0.0f, lz = -1.0f;
// XZ position of the camera
float x = 0.0f, z = 5.0f;
// Key states. Will be 0 when no keys are being pressed
float deltaAngle = 0.0f;
float deltaMove = 0;
int xOrigin = -1;

int t;
int frame = 0, time, timebase = 0;
int fps;

bool filesUnRead = true;

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
    x += deltaMove * lx * 0.1f;
    z += deltaMove * lz * 0.1f;
}

void specialKeyPress(int key, int x, int y)
{
    float fraction = 0.1f;

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
    char used;
    printf("pressed %c uiKeysHander is %d used %c", key, uiKeysHandler(key, x, y), used);

    if(!uiKeysHandler(key, x, y)) //filter for UI -KM
    {
        switch (key)
        {
        case ESCAPE:
            printf("Exited...");
            exit(1);
            break;
        case 'w': deltaMove = 0.5f; break;
        case 's': deltaMove = -0.5f; break;
        case 'a': deltaAngle = -0.01f; break;
        case 'd': deltaAngle = 0.01f; break;
        case 'f':
            if(gState.uiSettings)
                gState.uiSettings = false;
            else
                gState.uiSettings = true;
            break;
        }
    }
    else
    {
        printf("uiKeysHandler is true\n");
        scanf(" %c", &used); //need to remove char that is used from buffer but it's not getting to the else
    }
    glutPostRedisplay();

}

void drawHouse()
{

    typedef GLfloat point3[3];

    /* define coordinates for a  rectangle - the main "building block"*/
    point3 vertices[10]= {{0.0, 0.0, -1.0},{0.0, 150.0, -1.0},
        {60.0, 150.0, -1.0},{60.0, 0.0, -1.0}, {60.0, 100.0, -1.0},
        {160.0, 150.0, -1.0}, {160.0, 100.0, -1.0}, {160.0, 0.0, -1.0},
        {220.0, 150.0, -1.0}, {220.0, 0.0, -1.0}
    };

    glColor3f(0.5f, 0.0f, 0.0f);

    glBegin(GL_QUADS);
    glVertex3fv(vertices[3]);
    glVertex3fv(vertices[0]);
    glVertex3fv(vertices[1]);
    glVertex3fv(vertices[2]);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3fv(vertices[6]);
    glVertex3fv(vertices[4]);
    glVertex3fv(vertices[2]);
    glVertex3fv(vertices[5]);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3fv(vertices[9]);
    glVertex3fv(vertices[7]);
    glVertex3fv(vertices[5]);
    glVertex3fv(vertices[8]);
    glEnd();

    // Right Wall
    glBegin(GL_QUADS);
    glVertex3f(220.0, 150.0, -1.0);
    glVertex3f(220.0, 150.0, -200.0);
    glVertex3f(220.0, 0.0, -200.0);
    glVertex3f(220.0, 0.0, -1.0);
    glEnd();

    // Back Wall
    glBegin(GL_QUADS);
    glVertex3f(0.0, 0.0, -200.0);
    glVertex3f(0.0, 150.0, -200.0);
    glVertex3f(220.0, 150.0, -200.0);
    glVertex3f(220.0, 0.0, -200.0);
    glEnd();

    // Left Wall
    glBegin(GL_QUADS);
    glVertex3f(0.0, 0.0, -1.0);
    glVertex3f(0.0, 150.0, -1.0);
    glVertex3f(0.0, 150.0, -200.0);
    glVertex3f(0.0, 0.0, -200.0);
    glEnd();

    glColor3f(0.5f, 0.0f, 0.5f);

    // Door
    glBegin(GL_QUADS);
    glVertex3f(60.0, 0.0, -1.0);
    glVertex3f(60.0, 100.0, -1.0);
    glVertex3f(160.0, 100.0, -1.0);
    glVertex3f(160.0, 0.0, -1.0);
    glEnd();

    glColor3f(0.0f, 0.0f, 0.5f);

    // Roof Front
    glBegin(GL_TRIANGLES);
    glVertex3fv(vertices[1]);
    glVertex3f(220.0, 150.0, -1.0);
    glVertex3f(110.0, 200.0, -1.0);
    glEnd();

    // Roof Back
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0, 150.0, -200.0);
    glVertex3f(220.0, 150.0, -200.0);
    glVertex3f(110.0, 200.0, -200.0);
    glEnd();

    // Roof Top Left
    glBegin(GL_QUADS);
    glVertex3f(0.0, 150.0, -1.0);
    glVertex3f(0.0, 150.0, -200.0);
    glVertex3f(110.0, 200.0, -200.0);
    glVertex3f(110.0, 200.0, -1.0);
    glEnd();

    // Roof Top Right
    glBegin(GL_QUADS);
    glVertex3f(220.0, 150.0, -1.0);
    glVertex3f(220.0, 150.0, -200.0);
    glVertex3f(110.0, 200.0, -200.0);
    glVertex3f(110.0, 200.0, -1.0);
    glEnd();

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

void renderScene(void)
{
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

    glColor3f(0.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
        glVertex3f(-100.0f, 0.0f, -100.0f);
        glVertex3f(-100.0f, 0.0f, 100.0f);
        glVertex3f(100.0f, 0.0f, 100.0f);
        glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

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

    // Register callbacks
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    //glutIdleFunc(renderScene);
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
