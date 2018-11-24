#include <iostream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <vector>
#include <string>
using namespace std;
//! The angle specifies the magnitude to which we are looking up or down. Calculated w.r.t x-axis 
double pitch=0.0f;
//! The angle specifies the magnitude to which we are looking left or right. Calculated w.r.t y-axis 
double yaw=-90.0f;
//! This angle defines how much we can see of the scene.
double fov=45.0f;
//! 3D vector that contains the position of camera i.e the PRP
glm::dvec3 cameraPos=glm::dvec3(0.0f,0.0f,4.0f);
//! 3D vector that contains the direction of PRP w.r.t VRP
glm::dvec3 directionSight=glm::dvec3(0.0f,0.0f,-1.0f);
//! 3D vector that contains the direction that defines what direction is UP
glm::dvec3 upVec=glm::dvec3(0.0f,1.0f,0.0f);
//*! This function draws an empty Brown Cube which was the original prototype for our classroom*/

unsigned int texture;
unsigned int normal_texture;

unsigned int loadTexture(const char *filename,int width,int height)
{
	unsigned int textureTemp;
	unsigned char *data;
	FILE* file;
	file= fopen(filename, "rb");
	if(file== NULL)
		return 0;
	data= (unsigned char *)malloc(width*height*3);
	fread(data,width*height*3,1,file);
	fclose(file);
	glGenTextures(1,&textureTemp);
	glBindTexture(GL_TEXTURE_2D,textureTemp);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
	free(data);
	return textureTemp;
}

void freeTexture(unsigned int texture)
{
	glDeleteTextures(1,&texture);
}

void drawEmptyClass()
{
	glBegin(GL_QUADS);
	// glColor3f(0.5f,0.35f,0.05f);//TOP
	// glVertex3f(1.0f,1.0f,-1.0f);
	// glVertex3f(-1.0f,1.0f,-1.0f);
	// glVertex3f(-1.0f,1.0f,0.0f);
	// glVertex3f(1.0f,1.0f,0.0f);
	glColor3f(0.196, 0.6, 0.8);//BOTTOM
	
	glVertex3f(1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	// glColor3f(0.5f,0.35f,0.05f);//BACK
	// glVertex3f(1.0f,1.0f,-1.0f);
	// glVertex3f(-1.0f,1.0f,-1.0f);
	// glVertex3f(-1.0f,-1.0f,-1.0f);
	// glVertex3f(1.0f,-1.0f,-1.0f);
	// glColor3f(0.5f,0.35f,0.05f);//FRONT
	// glVertex3f(1.0f,1.0f,0.0f);
	// glVertex3f(-1.0f,1.0f,0.0f);
	// glVertex3f(-1.0f,-1.0f,0.0f);
	// glVertex3f(1.0f,-1.0f,0.0f);
	// glColor3f(0.5f,0.35f,0.05f);//LEFT
	// glVertex3f(-1.0f,1.0f,-1.0f);
	// glVertex3f(-1.0f,1.0f,0.0f);
	// glVertex3f(-1.0f,-1.0f,0.0f);
	// glVertex3f(-1.0f,-1.0f,-1.0f);
	// glColor3f(0.5f,0.35f,0.05f);//RIGHT
	// glVertex3f(1.0f,1.0f,0.0f);
	// glVertex3f(1.0f,1.0f,-1.0f);
	// glVertex3f(1.0f,-1.0f,-1.0f);
	// glVertex3f(1.0f,-1.0f,0.0f);
	glEnd();
	glBegin(GL_LINES);
	glColor3f(0.0f,0.0f,0.0f);//BACK TOP
	glVertex3f(1.0f,1.0f,-1.0f);
	glVertex3f(-1.0f,1.0f,-1.0f);
	glColor3f(0.0f,0.0f,0.0f);//BACK BOTTOM
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glColor3f(0.0f,0.0f,0.0f);//BACK LEFT
	glVertex3f(-1.0f,1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glColor3f(0.0f,0.0f,0.0f);//BACK RIGHT
	glVertex3f(1.0f,1.0f,-1.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glColor3f(0.0f,0.0f,0.0f);//FRONT TOP
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);//FRONT BOTTOM
	glVertex3f(-1.0f,-1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);//FRONT LEFT
	glVertex3f(-1.0f,1.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);//FRONT RIGHT
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);//TOP LEFT
	glVertex3f(-1.0f,1.0f,0.0f); 
	glVertex3f(-1.0f,1.0f,-1.0f);
	glColor3f(0.0f,0.0f,0.0f);//BOTTOM LEFT
	glVertex3f(-1.0f,-1.0f,-1.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);//TOP RIGHT
	glVertex3f(1.0f,1.0f,-1.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glColor3f(0.0f,0.0f,0.0f);//BOTTOM RIGHT
	glVertex3f(1.0f,-1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,-1.0f);
	glEnd();
}

void drawSquare()
{
	// glActiveTexture(GL_TEXTURE0);
	// glEnable(GL_TEXTURE_2D);
	// int texture_location = glGetUniformLocation
	glBegin(GL_QUADS);
	glColor3f(0.196, 0.6, 0.8);
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	glEnd();
}

void drawTeapot()
{
	glColor3f(0.196, 0.6, 0.8);
	glutSolidTeapot(1);
}
void changeSize(int w, int h) 
{
	if (h == 0)
		h = 1;
	float ratio =  w * 1.0 / h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(fov,ratio,1,100);
	glMatrixMode(GL_MODELVIEW);
}

void myDisplay(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraPos[0],cameraPos[1],cameraPos[2],cameraPos[0]+directionSight[0],cameraPos[1]+directionSight[1],cameraPos[2]+directionSight[2],upVec[0],upVec[1],upVec[2]);
	// drawSquare();
	drawTeapot();
	glutSwapBuffers();
}

void myinit()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	texture= loadTexture("colour_map.raw",256,256);
	normal_texture=loadTexture("normal_map.raw",256,256);
}

void update(int data)
{
	glutTimerFunc(30,update,0);
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x,int y)
{
	double sensitivityX=1.5f;
	double sensitivityY=1.5f;
	// double sensitivityFOV=1.0f;
	if(key==27)
	{
		exit(0);
	}
	if(key=='W'||key=='w')
	{
		pitch+=sensitivityY;
	}
	if(key=='A'||key=='a')
	{
		yaw-=sensitivityX;
	}
	if(key=='S'||key=='s')
	{
		pitch-=sensitivityY;
	}
	if(key=='D'||key=='d')
	{
		yaw+=sensitivityX;
	}
	// if(key=='+')
	// {
	// 	fov+=sensitivityFOV;
	// }
	// if(key=='-')
	// {
	// 	fov-=sensitivityFOV;
	// }
	// if(fov<=1.0f)
	// 	fov=1.0f;
	// if(fov>=90.0f)
	// 	fov=90.0f;
    if(pitch > 179.0f)
        pitch = 179.0f;
    if(pitch < -179.0f)
        pitch = -179.0f;
    if(yaw > 179.9f)
        yaw = 179.0f;
    if(yaw < -179.0f)
        yaw = -179.0f;
	glm::dvec3 tempDir;
	tempDir[0]=cos(glm::radians(yaw))*cos(glm::radians(pitch));
	tempDir[1]=sin(glm::radians(pitch));
	tempDir[2]=sin(glm::radians(yaw))*cos(glm::radians(pitch));
	directionSight=glm::normalize(tempDir);
}

void processSpecialKeys(int key, int x,int y)
{
	double fraction=0.1f;
	if(key==GLUT_KEY_LEFT)
	{
		cameraPos-=fraction*glm::normalize(glm::cross(directionSight,upVec));
	}
	if(key==GLUT_KEY_RIGHT)
	{
		cameraPos+=fraction*glm::normalize(glm::cross(directionSight,upVec));	
	}
	if(key==GLUT_KEY_UP)
	{
		cameraPos+=fraction*directionSight;
	}
	if(key==GLUT_KEY_DOWN)
	{
		cameraPos-=fraction*directionSight;
	}
}
int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1000,1000);
	glutCreateWindow("Classroom");
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(30,update,0);
	myinit();
	glutMainLoop();
	return 1;
}