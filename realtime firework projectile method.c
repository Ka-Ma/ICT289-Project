#include math.h //include math for sin, cos and pow functions

#define TIMERSECS 33 //timer tick for realtime calcs
#define GRAV -24.5 //gravity
/* It is assumed that 1 meter = approx 2.5 in-world units, seeing as the view point has a y of 5 (assumes view point is at head)*/
#define THETA 80 //Launch angle for tubes not middle, middle tube is 0 degrees (directly upwards)

typedef double coord[3];//defining a coordinate type
typedef coord fw[MAX_TIME]; //MAX_TIME is max fuse time within settings;

fw fire[10];//max 10 fireworks, defining array of firework 'objects'
int cont[10]; //whether that firework is at fuse time
double vel[10], fuse[10];//lauch velocity and fuse time for each firework

int startTime; //set through glutGet(GLUT_ELAPSED_TIME) in call func

void animate(int * val) //val is an array consisting of which tube, and the number of existing fireworks +1
{
	//calculating time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int elapsedTime = currTime - startTime;
	
	if(elapsedTime!=fuse){
		//defining coordinate variables for temp storage
		double s, r, x, z; //vertical displacement and horizontal displacement
		
		//Calc the y coord (vertical displacement)
		s = Math.pow(((vel[1] * (elapsedTime/1000)) + (0.5*GRAV*(elapsedTime/1000))),2.0);
		
		//call depending on which firing tube
		if(val[0]==0){
			//Calc the horizontal displacement assuming terminal velocity is infinite
			r = vel[1] * Math.cos(THETA) * (elapsedTime/1000);
			//assuming tube facing angles are hard coded as angle[tube number] with the z axis as 0 degrees
			x = r * Math.cos(angle[val[0]]);
			z = r * Math.cos((90-angle[val[0]]));
			
			//assign coords to array
			fire[val[1]][elapsedTime/1000] = {x,s,z};
		} else {
			//firework goes directly up
			fire[val[1]][elapsedTime/1000] = {0,s,0};
		}
		
		//call function again (recursively)
		glutTimerFunc(TIMERSECS, animate, val);
	} else{
		//call firework detonation func
	}
}
