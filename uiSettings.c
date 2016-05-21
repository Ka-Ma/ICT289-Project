#include "uiSettings.h"

void displayUISettings()
{
    //copy current settings into temporary holder so they can be altered and alteration confirmed.
    tmpState = gState;

    // getting window dimensions
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    uiBits.height=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = uiBits.height/100;

    //counters
    int i,j;

    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); //to prepare for return to 3D
    glLoadIdentity();
    gluOrtho2D(0,w,0,uiBits.height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //UI background 20% of screen gap from edges
    glColor3f(1,1,1); //white
    glBegin(GL_QUADS);
        glVertex2s(percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,uiBits.height-percUnitH*20);
        glVertex2s(percUnitW*20,uiBits.height-percUnitH*20);
    glEnd();

    glColor3f(0,0,1);
    displayText("SETTINGS: s to save, c to cancel", percUnitW*22, percUnitH*78);

    //for colourpicker triangle
    drawColourPicker(percUnitW, percUnitH);
    //check box for selection of random colour
    drawCheckBox(percUnitW*35,percUnitH*55,"Random Colour");
    //if random colour selected draw cross
    if(tmpState.colourRand){
        drawCheck(percUnitW*35,percUnitH*55);
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
                drawCheck(percUnitW*60, percUnitH*55);
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
    drawCheckBox(percUnitW*60, percUnitH*55,"Random Angle");

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
    uiBits.cancelBL[0] = percUnitW*70;
    uiBits.cancelBL[1] = percUnitH*21;
    uiBits.cancelTR[0] = percUnitW*78;
    uiBits.cancelTR[1] = percUnitH*34;
    glColor3f(0.5, 0, 0);
    glBegin(GL_QUADS);
        glVertex2s(percUnitW*70, percUnitH*21);
        glVertex2s(percUnitW*78, percUnitH*21);
        glVertex2s(percUnitW*78, percUnitH*34);
        glVertex2s(percUnitW*70, percUnitH*34);
    glEnd();
    displayText("CANCEL ('c')", percUnitW*72, percUnitH*32);

    // returning to 3D
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
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
        //height-y to convert coordinates to match screen and mouse
        if(x > uiBits.saveBL[0] && x < uiBits.saveTR[0] && uiBits.height-y > uiBits.saveBL[1] && uiBits.height-y < uiBits.saveTR[1]){
            gState = tmpState; //copy temp settings to gState
            gState.uiSettings = false;
        }
        if(x > uiBits.cancelBL[0] && x < uiBits.cancelTR[0] && uiBits.height-y > uiBits.cancelBL[1] && uiBits.height-y < uiBits.cancelTR[1]){
            gState.uiSettings = false;
        }
        //if area of radio buttons for angles

        //if area of colour picker
            //glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, tmpState.colour);

        //if area of lifter charge (launch velocity)
    }

}

void drawColourPicker(GLshort percUnitW, GLshort percUnitH)
{
    //set coordinates to this windows dimensions
    uiBits.clrpick.top[0] = percUnitW*35.0;
    uiBits.clrpick.top[1] = percUnitH*75.0;
    uiBits.clrpick.left[0] = percUnitW*25.0;
    uiBits.clrpick.left[1] = percUnitH*60.0;
    uiBits.clrpick.right[0] = percUnitW*45.0;
    uiBits.clrpick.right[1] = percUnitH*60.0;
    uiBits.clrpick.mtl[0] = (uiBits.clrpick.top[0]+uiBits.clrpick.left[0])/2.0;
    uiBits.clrpick.mtl[1] = (uiBits.clrpick.top[1]+uiBits.clrpick.left[1])/2.0;
    uiBits.clrpick.mlr[0] = (uiBits.clrpick.left[0]+uiBits.clrpick.right[0])/2.0;
    uiBits.clrpick.mlr[1] = (uiBits.clrpick.left[1]+uiBits.clrpick.right[1])/2.0;
    uiBits.clrpick.mtr[0] = (uiBits.clrpick.top[0]+uiBits.clrpick.right[0])/2.0;
    uiBits.clrpick.mtr[1] = (uiBits.clrpick.top[1]+uiBits.clrpick.right[1])/2.0;
    uiBits.clrpick.centre[0] = uiBits.clrpick.top[0]+(2.0/3.0)*(uiBits.clrpick.mlr[0]-uiBits.clrpick.top[0]);
    uiBits.clrpick.centre[1] = uiBits.clrpick.top[1]+(2.0/3.0)*(uiBits.clrpick.mlr[1]-uiBits.clrpick.top[1]);

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
        glVertex2fv(uiBits.clrpick.centre);
        glColor3fv(mtl);
        glVertex2fv(uiBits.clrpick.mtl);
        glColor3fv(left);
        glVertex2fv(uiBits.clrpick.left);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiBits.clrpick.centre);
        glColor3fv(top);
        glVertex2fv(uiBits.clrpick.top);
        glColor3fv(mtl);
        glVertex2fv(uiBits.clrpick.mtl);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiBits.clrpick.centre);
        glColor3fv(mtr);
        glVertex2fv(uiBits.clrpick.mtr);
        glColor3fv(top);
        glVertex2fv(uiBits.clrpick.top);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiBits.clrpick.centre);
        glColor3fv(right);
        glVertex2fv(uiBits.clrpick.right);
        glColor3fv(mtr);
        glVertex2fv(uiBits.clrpick.mtr);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiBits.clrpick.centre);
        glColor3fv(mlr);
        glVertex2fv(uiBits.clrpick.mlr);
        glColor3fv(right);
        glVertex2fv(uiBits.clrpick.right);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiBits.clrpick.centre);
        glColor3fv(left);
        glVertex2fv(uiBits.clrpick.left);
        glColor3fv(mlr);
        glVertex2fv(uiBits.clrpick.mlr);
    glEnd();
}
