#include "uiSettings.h"


void displayUISettings()
{
    // getting window dimensions
    GLshort w=glutGet(GLUT_WINDOW_WIDTH);
    uiBits.height=glutGet(GLUT_WINDOW_HEIGHT);
    GLshort percUnitW = w/100;
    GLshort percUnitH = uiBits.height/100;

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
    displayText("SETTINGS:", percUnitW*22, percUnitH*78, 'l');

    //draw chosen colour if not random
    if(!tmpState.colourRand){
        uiBits.chosenClr[0] = percUnitW*25;
        uiBits.chosenClr[1] = percUnitH*55;
        glColor3fv(tmpState.colour);
        glBegin(GL_QUADS);
            glVertex2sv(uiBits.chosenClr);
            glVertex2s(uiBits.chosenClr[0]+20, uiBits.chosenClr[1]);
            glVertex2s(uiBits.chosenClr[0]+20, uiBits.chosenClr[1]+20);
            glVertex2s(uiBits.chosenClr[0], uiBits.chosenClr[1]+20);
        glEnd();
        displayText("Chosen Colour", uiBits.chosenClr[0]+25, uiBits.chosenClr[1]+2, 'm');
    }

    //for colourpicker triangle
    drawColourPicker(percUnitW, percUnitH);
    //check box for selection of random colour
    uiBits.randClr[0]=percUnitW*35;
    uiBits.randClr[1]=percUnitH*55;
    drawCheckBox(uiBits.randClr[0],uiBits.randClr[1],"Random Colour");
    //if random colour selected draw cross
    if(tmpState.colourRand){
        drawCheck(uiBits.randClr[0],uiBits.randClr[1]);
    }

    //for selection of angle: draw a box then distribute evenly within 9 circles of radius 10, in 3 rows of 3, filled in for chosen angle
    drawAnglePresets(percUnitW, percUnitH);

    //check box for selection of random angle
    uiBits.randAng[0] = percUnitW*60;
    uiBits.randAng[1] = percUnitH*55;
    drawCheckBox(uiBits.randAng[0], uiBits.randAng[1] ,"Random Angle");

    //slider bar for selection of lift charge/intial velocity
    uiBits.velocityL[0]= percUnitW*23;
    uiBits.velocityL[1]= percUnitH*42;
    uiBits.velocityR[0]= percUnitW*65;
    uiBits.velocityR[1]= percUnitH*42;
    snprintf(uiBits.velocityMnTxt, 6, "%4.1f", tmpState.velocityMin);
    snprintf(uiBits.velocityMxTxt, 6, "%4.1f", tmpState.velocityMax);
    displayText("Lift Charge giving initial velocity in m/s:", uiBits.velocityL[0]-20, uiBits.velocityL[1]+15, 'm');
    drawSlider(uiBits.velocityL, uiBits.velocityR);
    displayText(uiBits.velocityMnTxt, uiBits.velocityL[0]-30, uiBits.velocityL[1]-4, 'm');
    displayText(uiBits.velocityMxTxt, uiBits.velocityR[0]+5, uiBits.velocityR[1]-4, 'm');

    //x coords of chosen velocity determined by (chosenValue - minValue)*((xRight - xLeft)/(maxValue-minValue)) + xLeft
    uiBits.velocityChX = ((tmpState.velocityCh - tmpState.velocityMin)*(((float)uiBits.velocityR[0]-(float)uiBits.velocityL[0])/(tmpState.velocityMax-tmpState.velocityMin))+(float)uiBits.velocityL[0]);
    drawMarker(uiBits.velocityChX, uiBits.velocityL[1]);
    snprintf(uiBits.velocityChTxt, 6, "%3.1f", tmpState.velocityCh);
    displayText(uiBits.velocityChTxt, uiBits.velocityChX-10, uiBits.velocityL[1]-17, 'm');

    //slider bar for selection of fuser timer
    uiBits.fuseL[0]= percUnitW*23;
    uiBits.fuseL[1]= percUnitH*32;
    uiBits.fuseR[0]= percUnitW*65;
    uiBits.fuseR[1]= percUnitH*32;
    snprintf(uiBits.fuseMnTxt, 6, "%5.1f", tmpState.fuseMin);
    snprintf(uiBits.fuseMxTxt, 6, "%5.1f", tmpState.fuseMax);
    displayText("Fuse timer from launch to explosion in milliseconds:", uiBits.fuseL[0]-20, uiBits.fuseL[1]+15, 'm');
    drawSlider(uiBits.fuseL, uiBits.fuseR);
    displayText(uiBits.fuseMnTxt, uiBits.fuseL[0]-40, uiBits.fuseL[1]-4, 'm');
    displayText(uiBits.fuseMxTxt, uiBits.fuseR[0]+5, uiBits.fuseR[1]-4, 'm');

    //x coords of chosen fuse determined by (chosenValue - minValue)*((xRight - xLeft)/(maxValue-minValue)) + xLeft
    uiBits.fuseChX = ((tmpState.fuseCh - tmpState.fuseMin)*(((float)uiBits.fuseR[0]-(float)uiBits.fuseL[0])/(tmpState.fuseMax-tmpState.fuseMin))+(float)uiBits.fuseL[0]);
    drawMarker(uiBits.fuseChX, uiBits.fuseL[1]);
    snprintf(uiBits.fuseChTxt, 6, "%5.1f", tmpState.fuseCh);
    displayText(uiBits.fuseChTxt, uiBits.fuseChX-10, uiBits.fuseL[1]-17, 'm');


    //save button
    uiBits.saveBL[0] = percUnitW*70;
    uiBits.saveBL[1] = percUnitH*31;
    uiBits.saveTR[0] = percUnitW*79;
    uiBits.saveTR[1] = percUnitH*39;
    glColor3f(0, 0.5, 0);
    glBegin(GL_QUADS);
        glVertex2sv(uiBits.saveBL);
        glVertex2s(uiBits.saveTR[0], uiBits.saveBL[1]);
        glVertex2sv(uiBits.saveTR);
        glVertex2s(uiBits.saveBL[0], uiBits.saveTR[1]);
    glEnd();
    displayText("Save", uiBits.saveBL[0]+5, uiBits.saveBL[1]+5,'l');
    //underline S
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2s(uiBits.saveBL[0]+6,uiBits.saveBL[1]+3);
        glVertex2s(uiBits.saveBL[0]+18,uiBits.saveBL[1]+3);
    glEnd();
    glLineWidth(1);

    //cancel button
    uiBits.cancelBL[0] = percUnitW*70;
    uiBits.cancelBL[1] = percUnitH*22;
    uiBits.cancelTR[0] = percUnitW*79;
    uiBits.cancelTR[1] = percUnitH*30;
    glColor3f(0.6, 0, 0);
    glBegin(GL_QUADS);
        glVertex2sv(uiBits.cancelBL);
        glVertex2s(uiBits.cancelTR[0], uiBits.cancelBL[1]);
        glVertex2sv(uiBits.cancelTR);
        glVertex2s(uiBits.cancelBL[0], uiBits.cancelTR[1]);
    glEnd();
    displayText("Cancel", uiBits.cancelBL[0]+5, uiBits.cancelBL[1]+5,'l');
    //underline C
    glLineWidth(2);
    glBegin(GL_LINES);
        glVertex2s(uiBits.cancelBL[0]+6,uiBits.cancelBL[1]+3);
        glVertex2s(uiBits.cancelBL[0]+18,uiBits.cancelBL[1]+3);
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
        y = uiBits.height - y;

        //save
        if(x > uiBits.saveBL[0] && x < uiBits.saveTR[0] && y > uiBits.saveBL[1] && y < uiBits.saveTR[1]){
            gState = tmpState; //copy temp settings to gState
            gState.uiSettings = false;
        }
        //cancel
        else if(x > uiBits.cancelBL[0] && x < uiBits.cancelTR[0] && y > uiBits.cancelBL[1] && y < uiBits.cancelTR[1]){
            gState.uiSettings = false;
        }

        // toggle random colour checkbox
        else if(x > uiBits.randClr[0] && x < uiBits.randClr[0] + 20 && y > uiBits.randClr[1] && y < uiBits.randClr[1] + 20){
            if(!tmpState.colourRand){
                tmpState.colourRand = true;
            }
            else{
                tmpState.colourRand = false;
            }
        }
        //area of radio buttons for angles
        else if(x> uiBits.anglesBL[0] && x < uiBits.anglesTR[0] && y > uiBits.anglesBL[1] && y < uiBits.anglesTR[1]){
            //checking each preset
            for(i=0;i<9;i++){
                // is click in area of this radio button?
                if(10 > sqrt(pow((x - uiBits.anglesPS[0][i]),2) + (pow((y - uiBits.anglesPS[1][i]),2)))){
                    tmpState.angle = i+1;
                }
            }
        }
        //toggle random angle checkbox
        else if(x > uiBits.randAng[0] && x < uiBits.randAng[0] + 20 && y > uiBits.randAng[1] && y < uiBits.randAng[1] + 20){
            if(!tmpState.angleRand){
                tmpState.angleRand = true;
            }
            else{
                tmpState.angleRand = false;
            }
        }
        //area of colour picker (triangle) formula of a line point1x-x = m(y-point1y) where m = point2y-point1y/point2x-point1x
        else if(y < uiBits.clrpick.top[1] && y > uiBits.clrpick.left[1]
                && ((float)x - uiBits.clrpick.left[0] > (((uiBits.clrpick.top[1] - uiBits.clrpick.left[1]) / (uiBits.clrpick.top[0] - uiBits.clrpick.left[0])) * ((float)y - uiBits.clrpick.left[1])))
                && ((float)x - uiBits.clrpick.right[0] < (((uiBits.clrpick.top[1] - uiBits.clrpick.right[1]) / (uiBits.clrpick.top[0] - uiBits.clrpick.right[0])) * ((float)y - uiBits.clrpick.right[1]))))
        {
            glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, tmpState.colour);
        }

        //area of lifter charge slider (launch velocity)
        else if(x <= uiBits.velocityR[0] && x >= uiBits.velocityL[0] && y < uiBits.velocityL[1]+5 && y > uiBits.velocityL[1]-5)
        {
            // chosen velocity given by ((x - minX) / (rangeX/rangeValue))+ minValue
            tmpState.velocityCh = ((x-uiBits.velocityL[0])/((uiBits.velocityR[0]-uiBits.velocityL[0])/(tmpState.velocityMax-tmpState.velocityMin)))+tmpState.velocityMin;
        }

        //area of fuse time slider
        else if(x <= uiBits.fuseR[0] && x >= uiBits.fuseL[0] && y < uiBits.fuseL[1]+5 && y > uiBits.fuseL[1]-5)
        {
            // chosen fuse given by ((x - minX) / (rangeX/rangeValue))+ minValue
            tmpState.fuseCh = ((x-uiBits.fuseL[0])/((uiBits.fuseR[0]-uiBits.fuseL[0])/(tmpState.fuseMax-tmpState.fuseMin)))+tmpState.fuseMin;
        }
    }// end if L-button clicked
}// end mouseUISettings

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
}// end drawColourPicker

void drawAnglePresets(GLshort percUnitW, GLshort percUnitH)
{
    int i,j;
    GLshort gapW = ((percUnitW*20)-30)/4;
    GLshort gapH = ((percUnitH*20)-30)/4;

    //set angle area box
    uiBits.anglesBL[0] = percUnitW*55;
    uiBits.anglesBL[1] = percUnitH*60;
    uiBits.anglesTR[0] = percUnitW*75;
    uiBits.anglesTR[1] = percUnitH*75;

    //draw angle area box
    glBegin(GL_LINE_LOOP);
        glVertex2sv(uiBits.anglesBL);
        glVertex2s(uiBits.anglesTR[0], uiBits.anglesBL[1]);
        glVertex2sv(uiBits.anglesTR);
        glVertex2s(uiBits.anglesBL[0], uiBits.anglesTR[1]);
    glEnd();

    for (i=0; i<3;i++){
        for (j=0;j<3;j++){
            //set angle preset location
            uiBits.anglesPS[0][i*3+j] = percUnitW*60+i*gapW; // x
            uiBits.anglesPS[1][i*3+j] = percUnitH*63+j*gapH; // y
            // draw radio button for angle preset
            drawHollowCircle(uiBits.anglesPS[0][i*3+j], uiBits.anglesPS[1][i*3+j], 10);
            if(!tmpState.angleRand){
                if(tmpState.angle == (i*3)+j+1){
                        drawFilledCircle(uiBits.anglesPS[0][i*3+j], uiBits.anglesPS[1][i*3+j], 8);
                }
            }
            else{
                //if random angle selected draw cross
                drawCheck(uiBits.randAng[0], uiBits.randAng[1]);
                //also need to draw a cross through the circles box to indicate out of use
                glBegin(GL_LINES);
                    glVertex2sv(uiBits.anglesBL);
                    glVertex2sv(uiBits.anglesTR);
                glEnd();
                glBegin(GL_LINES);
                    glVertex2s(uiBits.anglesTR[0], uiBits.anglesBL[1]);
                    glVertex2s(uiBits.anglesBL[0], uiBits.anglesTR[1]);
                glEnd();
            } // end if
        }// end for
    }// end for
}// end drawPresetAngles


