#include "uiSettings.h"


void displayUISettings()
{
    // getting window dimensions
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    uiS.height=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = uiS.height/100;

    glDisable(GL_DEPTH_TEST);
    glClear(GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); //to prepare for return to 3D
    glLoadIdentity();
    gluOrtho2D(0,w,0,uiS.height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //UI background 20% of screen gap from edges
    glColor3f(1,1,1); //white
    glBegin(GL_QUADS);
        glVertex2s(percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,percUnitH*20);
        glVertex2s(w-percUnitW*20,uiS.height-percUnitH*20);
        glVertex2s(percUnitW*20,uiS.height-percUnitH*20);
    glEnd();

    glColor3f(0,0,1);
    displayText("SETTINGS:", percUnitW*22, percUnitH*78, 'l');

    //draw chosen colour if not random
    if(!tmpState.colourRand){
        uiS.chosenClr[0] = percUnitW*25;
        uiS.chosenClr[1] = percUnitH*55;
        glColor3fv(tmpState.colour);
        glBegin(GL_QUADS);
            glVertex2sv(uiS.chosenClr);
            glVertex2s(uiS.chosenClr[0]+20, uiS.chosenClr[1]);
            glVertex2s(uiS.chosenClr[0]+20, uiS.chosenClr[1]+20);
            glVertex2s(uiS.chosenClr[0], uiS.chosenClr[1]+20);
        glEnd();
        displayText("Chosen Colour", uiS.chosenClr[0]+25, uiS.chosenClr[1]+2, 'm');
    }

    //for colourpicker triangle
    drawColourPicker(percUnitW, percUnitH);
    //check box for selection of random colour
    uiS.randClr[0]=percUnitW*35;
    uiS.randClr[1]=percUnitH*55;
    drawCheckBox(uiS.randClr[0],uiS.randClr[1],"Random Colour");
    //if random colour selected draw cross
    if(tmpState.colourRand){
        drawCheck(uiS.randClr[0],uiS.randClr[1]);
    }

    //for selection of angle: draw a box then distribute evenly within 9 circles of radius 10, in 3 rows of 3, filled in for chosen angle
    drawAnglePresets(percUnitW, percUnitH);

    //check box for selection of random angle
    uiS.randAng[0] = percUnitW*60;
    uiS.randAng[1] = percUnitH*55;
    drawCheckBox(uiS.randAng[0], uiS.randAng[1] ,"Random Angle");

    //slider bar for selection of lift charge/intial velocity
    uiS.velocityL[0]= percUnitW*23;
    uiS.velocityL[1]= percUnitH*42;
    uiS.velocityR[0]= percUnitW*65;
    uiS.velocityR[1]= percUnitH*42;
    snprintf(uiS.velocityMnTxt, 6, "%4.1f", tmpState.velocityMin);
    snprintf(uiS.velocityMxTxt, 6, "%4.1f", tmpState.velocityMax);
    displayText("Lift Charge giving initial velocity in m/s:", uiS.velocityL[0]-20, uiS.velocityL[1]+15, 'm');
    drawSlider(uiS.velocityL, uiS.velocityR);
    displayText(uiS.velocityMnTxt, uiS.velocityL[0]-30, uiS.velocityL[1]-4, 'm');
    displayText(uiS.velocityMxTxt, uiS.velocityR[0]+5, uiS.velocityR[1]-4, 'm');

    //x coords of chosen velocity determined by (chosenValue - minValue)*((xRight - xLeft)/(maxValue-minValue)) + xLeft
    uiS.velocityChX = ((tmpState.velocityCh - tmpState.velocityMin)*(((float)uiS.velocityR[0]-(float)uiS.velocityL[0])/(tmpState.velocityMax-tmpState.velocityMin))+(float)uiS.velocityL[0]);
    drawMarker(uiS.velocityChX, uiS.velocityL[1]);
    snprintf(uiS.velocityChTxt, 6, "%3.1f", tmpState.velocityCh);
    displayText(uiS.velocityChTxt, uiS.velocityChX-10, uiS.velocityL[1]-17, 'm');

    //slider bar for selection of fuser timer
    uiS.fuseL[0]= percUnitW*23;
    uiS.fuseL[1]= percUnitH*32;
    uiS.fuseR[0]= percUnitW*65;
    uiS.fuseR[1]= percUnitH*32;
    snprintf(uiS.fuseMnTxt, 6, "%5.1f", tmpState.fuseMin);
    snprintf(uiS.fuseMxTxt, 6, "%5.1f", tmpState.fuseMax);
    displayText("Fuse timer from launch to explosion in milliseconds:", uiS.fuseL[0]-20, uiS.fuseL[1]+15, 'm');
    drawSlider(uiS.fuseL, uiS.fuseR);
    displayText(uiS.fuseMnTxt, uiS.fuseL[0]-40, uiS.fuseL[1]-4, 'm');
    displayText(uiS.fuseMxTxt, uiS.fuseR[0]+5, uiS.fuseR[1]-4, 'm');

    //x coords of chosen fuse determined by (chosenValue - minValue)*((xRight - xLeft)/(maxValue-minValue)) + xLeft
    uiS.fuseChX = ((tmpState.fuseCh - tmpState.fuseMin)*(((float)uiS.fuseR[0]-(float)uiS.fuseL[0])/(tmpState.fuseMax-tmpState.fuseMin))+(float)uiS.fuseL[0]);
    drawMarker(uiS.fuseChX, uiS.fuseL[1]);
    snprintf(uiS.fuseChTxt, 6, "%5.1f", tmpState.fuseCh);
    displayText(uiS.fuseChTxt, uiS.fuseChX-10, uiS.fuseL[1]-17, 'm');


    //save button
    uiS.saveBL[0] = percUnitW*70;
    uiS.saveBL[1] = percUnitH*31;
    uiS.saveTR[0] = percUnitW*79;
    uiS.saveTR[1] = percUnitH*39;
    glColor3f(0, 0.5, 0);
    glBegin(GL_QUADS);
        glVertex2sv(uiS.saveBL);
        glVertex2s(uiS.saveTR[0], uiS.saveBL[1]);
        glVertex2sv(uiS.saveTR);
        glVertex2s(uiS.saveBL[0], uiS.saveTR[1]);
    glEnd();
    displayText("Save", uiS.saveBL[0]+5, uiS.saveBL[1]+5,'l');
    //underline S
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2s(uiS.saveBL[0]+6,uiS.saveBL[1]+3);
        glVertex2s(uiS.saveBL[0]+18,uiS.saveBL[1]+3);
    glEnd();
    glLineWidth(1);

    //cancel button
    uiS.cancelBL[0] = percUnitW*70;
    uiS.cancelBL[1] = percUnitH*22;
    uiS.cancelTR[0] = percUnitW*79;
    uiS.cancelTR[1] = percUnitH*30;
    glColor3f(0.6, 0, 0);
    glBegin(GL_QUADS);
        glVertex2sv(uiS.cancelBL);
        glVertex2s(uiS.cancelTR[0], uiS.cancelBL[1]);
        glVertex2sv(uiS.cancelTR);
        glVertex2s(uiS.cancelBL[0], uiS.cancelTR[1]);
    glEnd();
    displayText("Cancel", uiS.cancelBL[0]+5, uiS.cancelBL[1]+5,'l');
    //underline C
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2s(uiS.cancelBL[0]+6,uiS.cancelBL[1]+3);
        glVertex2s(uiS.cancelBL[0]+18,uiS.cancelBL[1]+3);
    glEnd();
    glLineWidth(1);

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
    int i;

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
    {
        //height-y to convert coordinates to match screen and mouse
        y = uiS.height - y;

        //save
        if(x > uiS.saveBL[0] && x < uiS.saveTR[0] && y > uiS.saveBL[1] && y < uiS.saveTR[1]){
            gState = tmpState; //copy temp settings to gState
            gState.uiSettings = false;
        }
        //cancel
        else if(x > uiS.cancelBL[0] && x < uiS.cancelTR[0] && y > uiS.cancelBL[1] && y < uiS.cancelTR[1]){
            gState.uiSettings = false;
        }

        // toggle random colour checkbox
        else if(x > uiS.randClr[0] && x < uiS.randClr[0] + 20 && y > uiS.randClr[1] && y < uiS.randClr[1] + 20){
            if(!tmpState.colourRand){
                tmpState.colourRand = true;
            }
            else{
                tmpState.colourRand = false;
            }
        }
        //area of radio buttons for angles
        else if(x> uiS.anglesBL[0] && x < uiS.anglesTR[0] && y > uiS.anglesBL[1] && y < uiS.anglesTR[1]){
            //checking each preset
            for(i=0;i<9;i++){
                // is click in area of this radio button?
                if(10 > sqrt(pow((x - uiS.anglesPS[0][i]),2) + (pow((y - uiS.anglesPS[1][i]),2)))){
                    tmpState.angle = i+1;
                }
            }
        }
        //toggle random angle checkbox
        else if(x > uiS.randAng[0] && x < uiS.randAng[0] + 20 && y > uiS.randAng[1] && y < uiS.randAng[1] + 20){
            if(!tmpState.angleRand){
                tmpState.angleRand = true;
            }
            else{
                tmpState.angleRand = false;
            }
        }
        //area of colour picker (triangle) formula of a line point1x-x = m(y-point1y) where m = point2y-point1y/point2x-point1x
        else if(y < uiS.clrpick.top[1] && y > uiS.clrpick.left[1]
                && ((float)x - uiS.clrpick.left[0] > (((uiS.clrpick.top[1] - uiS.clrpick.left[1]) / (uiS.clrpick.top[0] - uiS.clrpick.left[0])) * ((float)y - uiS.clrpick.left[1])))
                && ((float)x - uiS.clrpick.right[0] < (((uiS.clrpick.top[1] - uiS.clrpick.right[1]) / (uiS.clrpick.top[0] - uiS.clrpick.right[0])) * ((float)y - uiS.clrpick.right[1]))))
        {
            glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, tmpState.colour);
        }

        //area of lifter charge slider (launch velocity)
        else if(x <= uiS.velocityR[0] && x >= uiS.velocityL[0] && y < uiS.velocityL[1]+5 && y > uiS.velocityL[1]-5)
        {
            // chosen velocity given by ((x - minX) / (rangeX/rangeValue))+ minValue
            tmpState.velocityCh = ((x-uiS.velocityL[0])/((uiS.velocityR[0]-uiS.velocityL[0])/(tmpState.velocityMax-tmpState.velocityMin)))+tmpState.velocityMin;
        }

        //area of fuse time slider
        else if(x <= uiS.fuseR[0] && x >= uiS.fuseL[0] && y < uiS.fuseL[1]+5 && y > uiS.fuseL[1]-5)
        {
            // chosen fuse given by ((x - minX) / (rangeX/rangeValue))+ minValue
            tmpState.fuseCh = ((x-uiS.fuseL[0])/((uiS.fuseR[0]-uiS.fuseL[0])/(tmpState.fuseMax-tmpState.fuseMin)))+tmpState.fuseMin;
        }
    }// end if L-button clicked
}// end mouseUISettings

void drawColourPicker(GLshort percUnitW, GLshort percUnitH)
{
    //set coordinates to this windows dimensions
    uiS.clrpick.top[0] = percUnitW*35.0;
    uiS.clrpick.top[1] = percUnitH*75.0;
    uiS.clrpick.left[0] = percUnitW*25.0;
    uiS.clrpick.left[1] = percUnitH*60.0;
    uiS.clrpick.right[0] = percUnitW*45.0;
    uiS.clrpick.right[1] = percUnitH*60.0;
    uiS.clrpick.mtl[0] = (uiS.clrpick.top[0]+uiS.clrpick.left[0])/2.0;
    uiS.clrpick.mtl[1] = (uiS.clrpick.top[1]+uiS.clrpick.left[1])/2.0;
    uiS.clrpick.mlr[0] = (uiS.clrpick.left[0]+uiS.clrpick.right[0])/2.0;
    uiS.clrpick.mlr[1] = (uiS.clrpick.left[1]+uiS.clrpick.right[1])/2.0;
    uiS.clrpick.mtr[0] = (uiS.clrpick.top[0]+uiS.clrpick.right[0])/2.0;
    uiS.clrpick.mtr[1] = (uiS.clrpick.top[1]+uiS.clrpick.right[1])/2.0;
    uiS.clrpick.centre[0] = uiS.clrpick.top[0]+(2.0/3.0)*(uiS.clrpick.mlr[0]-uiS.clrpick.top[0]);
    uiS.clrpick.centre[1] = uiS.clrpick.top[1]+(2.0/3.0)*(uiS.clrpick.mlr[1]-uiS.clrpick.top[1]);

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
        glVertex2fv(uiS.clrpick.centre);
        glColor3fv(mtl);
        glVertex2fv(uiS.clrpick.mtl);
        glColor3fv(left);
        glVertex2fv(uiS.clrpick.left);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiS.clrpick.centre);
        glColor3fv(top);
        glVertex2fv(uiS.clrpick.top);
        glColor3fv(mtl);
        glVertex2fv(uiS.clrpick.mtl);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiS.clrpick.centre);
        glColor3fv(mtr);
        glVertex2fv(uiS.clrpick.mtr);
        glColor3fv(top);
        glVertex2fv(uiS.clrpick.top);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiS.clrpick.centre);
        glColor3fv(right);
        glVertex2fv(uiS.clrpick.right);
        glColor3fv(mtr);
        glVertex2fv(uiS.clrpick.mtr);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiS.clrpick.centre);
        glColor3fv(mlr);
        glVertex2fv(uiS.clrpick.mlr);
        glColor3fv(right);
        glVertex2fv(uiS.clrpick.right);
    glEnd();
    glBegin(GL_TRIANGLES);
        glColor3fv(centre);
        glVertex2fv(uiS.clrpick.centre);
        glColor3fv(left);
        glVertex2fv(uiS.clrpick.left);
        glColor3fv(mlr);
        glVertex2fv(uiS.clrpick.mlr);
    glEnd();
}// end drawColourPicker

void drawAnglePresets(GLshort percUnitW, GLshort percUnitH)
{
    int i,j;
    GLshort gapW = ((percUnitW*20)-30)/4;
    GLshort gapH = ((percUnitH*20)-30)/4;

    //set angle area box
    uiS.anglesBL[0] = percUnitW*55;
    uiS.anglesBL[1] = percUnitH*60;
    uiS.anglesTR[0] = percUnitW*75;
    uiS.anglesTR[1] = percUnitH*75;

    //draw angle area box
    glBegin(GL_LINE_LOOP);
        glVertex2sv(uiS.anglesBL);
        glVertex2s(uiS.anglesTR[0], uiS.anglesBL[1]);
        glVertex2sv(uiS.anglesTR);
        glVertex2s(uiS.anglesBL[0], uiS.anglesTR[1]);
    glEnd();

    for (i=0; i<3;i++){
        for (j=0;j<3;j++){
            //set angle preset location
            uiS.anglesPS[0][i*3+j] = percUnitW*60+i*gapW; // x
            uiS.anglesPS[1][i*3+j] = percUnitH*63+j*gapH; // y
            // draw radio button for angle preset
            drawHollowCircle(uiS.anglesPS[0][i*3+j], uiS.anglesPS[1][i*3+j], 10);
            if(!tmpState.angleRand){
                if(tmpState.angle == (i*3)+j+1){
                        drawFilledCircle(uiS.anglesPS[0][i*3+j], uiS.anglesPS[1][i*3+j], 8);
                }
            }
            else{
                //if random angle selected draw cross
                drawCheck(uiS.randAng[0], uiS.randAng[1]);
                //also need to draw a cross through the circles box to indicate out of use
                glBegin(GL_LINES);
                    glVertex2sv(uiS.anglesBL);
                    glVertex2sv(uiS.anglesTR);
                glEnd();
                glBegin(GL_LINES);
                    glVertex2s(uiS.anglesTR[0], uiS.anglesBL[1]);
                    glVertex2s(uiS.anglesBL[0], uiS.anglesTR[1]);
                glEnd();
            } // end if
        }// end for
    }// end for
}// end drawPresetAngles


