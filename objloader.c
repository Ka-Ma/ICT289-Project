#include "objloader.h"

const char* readModelFile(char *fTitle, const int index)
{
    char str[5];

        if((oFile = fopen(fTitle, "r")) == NULL)
        {
            return ("File not opened\n");
        }else
        {
            if (fgets(str, 50, oFile) != NULL)
            {
                if(strcmp(str, "OFF") != 0)
                {
                    // Get Vertex Lines and Face Lines
                    fscanf(oFile, "%d", &(objectList[index].vLines));
                    fscanf(oFile, "%d", &(objectList[index].fLines));
                    fscanf(oFile, "%d", &(objectList[index].uLines));

                }else
                {
                    return ("File not OFF\n");
                }
            }
        }

        objectList[index].coords = malloc(objectList[index].vLines * sizeof (struct Vector3));
        objectList[index].facets = malloc(objectList[index].fLines * sizeof (struct FaceVec));

        for(int i = 0; i < objectList[index].vLines; i++)
        {
        fscanf(oFile, "%lf", &(objectList[index].coords)[i].x);
        fscanf(oFile, "%lf", &(objectList[index].coords)[i].y);
        fscanf(oFile, "%lf", &(objectList[index].coords)[i].z);
        //printf("%.3f ",  (objectList[index].coords)[i].x);
        //printf("%.3f ",  (objectList[index].coords)[i].y);
        //printf("%.3f\n", (objectList[index].coords)[i].z);
        }

        int vertLine1, vertLine2, vertLine3;

        for(int i = 0; i < objectList[index].fLines; i++)
        {
            fscanf(oFile, "%d", &(objectList[index].facets)[i].faceType);

            if ((objectList[index].facets)[i].faceType == 3)
            {
                fscanf(oFile, "%d", &vertLine1);
                (objectList[index].facets)[i].coord1.x = (objectList[index].coords)[vertLine1].x;
                (objectList[index].facets)[i].coord1.y = (objectList[index].coords)[vertLine1].y;
                (objectList[index].facets)[i].coord1.z = (objectList[index].coords)[vertLine1].z;

                fscanf(oFile, "%d", &vertLine2);
                (objectList[index].facets)[i].coord2.x = (objectList[index].coords)[vertLine2].x;
                (objectList[index].facets)[i].coord2.y = (objectList[index].coords)[vertLine2].y;
                (objectList[index].facets)[i].coord2.z = (objectList[index].coords)[vertLine2].z;

                fscanf(oFile, "%d", &vertLine3);
                (objectList[index].facets)[i].coord3.x = (objectList[index].coords)[vertLine3].x;
                (objectList[index].facets)[i].coord3.y = (objectList[index].coords)[vertLine3].y;
                (objectList[index].facets)[i].coord3.z = (objectList[index].coords)[vertLine3].z;

                //printf("%d ",    (objectList[index].facets)[i].faceType);
                //printf("%d ", vertLine1);
                //printf("%d ", vertLine2);
                //printf("%d\n", vertLine3);
                //printf("%.3f ",  (objectList[index].facets)[i].x);
                //printf("%.3f ",  (objectList[index].facets)[i].y);
                //printf("%.3f\n", (objectList[index].facets)[i].z);
            }

        }

        return ("Model loaded\n");
}

void drawModelFile(const int index)
{
    //Generate model
    for(int i = 0; i < objectList[index].fLines; i++)
    {
        switch((objectList[index].facets)[i].faceType)
        {
        case 3://Triangles
            glBegin(GL_TRIANGLES);
            glVertex3f((objectList[index].facets)[i].coord1.x, (objectList[index].facets)[i].coord1.y, (objectList[index].facets)[i].coord1.z);
            glVertex3f((objectList[index].facets)[i].coord2.x, (objectList[index].facets)[i].coord2.y, (objectList[index].facets)[i].coord2.z);
            glVertex3f((objectList[index].facets)[i].coord3.x, (objectList[index].facets)[i].coord3.y, (objectList[index].facets)[i].coord3.z);
            glEnd();
            break;

        case 4://Quads
            break;
        }
    }
}
