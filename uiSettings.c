#include "uiSettings.h"


struct globalState tmpState;
struct uiBits {
    GLshort saveBL[2]; //x,y coords of bottom left of save button
    GLshort saveTR[2]; //x,y coords of top right of save button
    GLshort cancel[2];
    }uiBits;

void displayUISettings()
{
    //copy current settings into temporary holder so they can be altered and alteration confirmed.
    tmpState = gState;

    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    GLshort h=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = h/100;
    int i,j;

    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); //to prepare for return to 3D
    glLoadIdentity();
    gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //UI background 20% of screen gap from edges
    glColor3f(1,1,1); //white
    glBegin(GL_QUADS);
        glVertex2s(percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,h-percUnitH*20);
        glVertex2s(percUnitW*20,h-percUnitH*20);
    glEnd();

    glColor3f(0,0,1);
    displayText("SETTINGS: s to save, c to cancel (mouse not enabled yet)", percUnitW*22, percUnitH*78);

    //for colourpicker triangle
    struct triangle clrpick = {
        .top = {percUnitW*35.0,
                percUnitH*75.0},
        .left = {percUnitW*25.0,
                percUnitH*60.0},
        .right = {percUnitW*45.0,
                percUnitH*60.0},
        .mtl = {(clrpick.top[0]+clrpick.left[0])/2.0,
                (clrpick.top[1]+clrpick.left[1])/2.0},
        .mlr = {(clrpick.left[0]+clrpick.right[0])/2.0,
                (clrpick.left[1]+clrpick.right[1])/2.0},
        .mtr = {(clrpick.top[0]+clrpick.right[0])/2.0,
                (clrpick.top[1]+clrpick.right[1])/2.0},
        .centre = {clrpick.top[0]+(2.0/3.0)*(clrpick.mlr[0]-clrpick.top[0]),
                clrpick.top[1]+(2.0/3.0)*(clrpick.mlr[1]-clrpick.top[1])}
    };

    // colours for points
    flt3 top = {1,0,0}; //red
    flt3 left = {0,1,0}; //green
    flt3 right = {0,0,1}; //blue
    flt3 centre = {1,1,1};  //white
    flt3 mtl = {1,1,0}; //red-green
    flt3 mlr = {0,1,1}; //green-blue
    flt3 mtr = {1,0,1}; //red-blue
    // draw colour picker in multiple triangles with centre white to get lighter hues
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(clrpick.centre);
        glColor3fv(mtl);
        glVertex2fv(clrpick.mtl);
        glColor3fv(left);
        glVertex2fv(clrpick.left);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(clrpick.centre);
        glColor3fv(top);
        glVertex2fv(clrpick.top);
        glColor3fv(mtl);
        glVertex2fv(clrpick.mtl);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(clrpick.centre);
        glColor3fv(mtr);
        glVertex2fv(clrpick.mtr);
        glColor3fv(top);
        glVertex2fv(clrpick.top);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(clrpick.centre);
        glColor3fv(right);
        glVertex2fv(clrpick.right);
        glColor3fv(mtr);
        glVertex2fv(clrpick.mtr);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(clrpick.centre);
        glColor3fv(mlr);
        glVertex2fv(clrpick.mlr);
        glColor3fv(right);
        glVertex2fv(clrpick.right);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(clrpick.centre);
        glColor3fv(left);
        glVertex2fv(clrpick.left);
        glColor3fv(mlr);
        glVertex2fv(clrpick.mlr);
    glEnd();
    //check box for selection of random colour
    glBegin(GL_LINE_LOOP);
        glVertex2s(percUnitW*35, percUnitH*55);
        glVertex2s(percUnitW*35+20, percUnitH*55);
        glVertex2s(percUnitW*35+20, percUnitH*55+20);
        glVertex2s(percUnitW*35, percUnitH*55+20);
    glEnd();
    displayText("Random Colour", percUnitW*35+25, percUnitH*55+5);
    //if random colour selected draw cross
    if(tmpState.colourRand){
        glBegin(GL_LINE);
            glVertex2s(percUnitW*35, percUnitH*55);
            glVertex2s(percUnitW*35+20, percUnitH*55+20);
        glEnd();
        glBegin(GL_LINE);
            glVertex2s(percUnitW*35+20, percUnitH*55);
            glVertex2s(percUnitW*35, percUnitH*55+20);
        glEnd();
    }

    //for selection of angle: draw a box then distribute evenly within 9 circles of radius 10, in 3 rows of 3, filled in for chosen angle
    glBegin(GL_LINE_LOOP);
        glVertex2s(percUnitW*55, percUnitH*60);
        glVertex2s(percUnitW*75, percUnitH*60);
        glVertex2s(percUnitW*75, percUnitH*75);
        glVertex2s(percUnitW*55, percUnitH*75);
    glEnd();
    for (i=0; i<3;i++){
        for (j=0;j<3;j++){
            drawHollowCircle(percUnitW*60+i*40, percUnitH*63+j*40, 10);
            if(!tmpState.angleRand){

                if(tmpState.angle == (i*3)+j+1){
                        drawFilledCircle(percUnitW*60+i*40, percUnitH*63+j*40, 8);
                }
            }
            else{
                //if random angle selected draw cross
                glBegin(GL_LINE);
                    glVertex2s(percUnitW*60, percUnitH*55);
                    glVertex2s(percUnitW*60+20, percUnitH*55+20);
                glEnd();
                glBegin(GL_LINE);
                    glVertex2s(percUnitW*60+20, percUnitH*55);
                    glVertex2s(percUnitW*60, percUnitH*55+20);
                glEnd();
                //also need to draw a cross through the circles box to indicate out of use
                glBegin(GL_LINE);
                    glVertex2s(percUnitW*55, percUnitH*60);
                    glVertex2s(percUnitW*75, percUnitH*75);
                glEnd();
                glBegin(GL_LINE);
                    glVertex2s(percUnitW*75, percUnitH*60);
                    glVertex2s(percUnitW*55, percUnitH*75);
                glEnd();
            }
        }
    }
    //check box for selection of random angle
    glBegin(GL_LINE_LOOP);
        glVertex2s(percUnitW*60, percUnitH*55);
        glVertex2s(percUnitW*60+20, percUnitH*55);
        glVertex2s(percUnitW*60+20, percUnitH*55+20);
        glVertex2s(percUnitW*60, percUnitH*55+20);
    glEnd();
    displayText("Random Angle", percUnitW*60+25, percUnitH*55+5);

    //save button
    uiBits.saveBL[0] = percUnitW*70;
    uiBits.saveBL[1] = percUnitH*36;
    uiBits.saveTR[0] = percUnitW*78;
    uiBits.saveTR[1] = percUnitH*49;
    glColor3f(0, 0.5, 0);
    glBegin(GL_QUADS);
        glVertex2s(percUnitW*70, percUnitH*36);
        glVertex2s(percUnitW*78, percUnitH*36);
        glVertex2s(percUnitW*78, percUnitH*49);
        glVertex2s(percUnitW*70, percUnitH*49);
    glEnd();
    displayText("SAVE ('s')", percUnitW*72, percUnitH*42);

    //cancel button
    glColor3f(0.5, 0, 0);
    glBegin(GL_QUADS);
        glVertex2s(percUnitW*70, percUnitH*21);
        glVertex2s(percUnitW*78, percUnitH*21);
        glVertex2s(percUnitW*78, percUnitH*34);
        glVertex2s(percUnitW*70, percUnitH*34);
    glEnd();
    displayText("CANCEL ('c')", percUnitW*72, percUnitH*32);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // returning to 3D
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

void keysUISettings(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 's': //save
        gState = tmpState; //copy temp settings to gState
        gState.uiSettings = false;
        break;
    case 'c': //cancel
        gState.uiSettings = false;
        break;
    }
}

void mouseUISettings(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(x > uiBits.saveBL[0] && x < uiBits.saveTR[0] && y > uiBits.saveBL[1] && y < uiBits.saveTR[1]){
            gState = tmpState; //copy temp settings to gState
            gState.uiSettings = false;
            printf("mouse x = %d y = %d\n", x, y);
            printf("button BL x = %d y = %d\n", uiBits.saveBL[0],uiBits.saveBL[1]);
            printf("button TR x = %d y = %d\n", uiBits.saveTR[0],uiBits.saveTR[1]);
            // mouse coords read from top left to bottom right need to sort that...
        }
    //glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, tmpState.colour);
    }

}
