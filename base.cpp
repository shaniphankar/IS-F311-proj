#include <iostream>
#include "shader.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "SOIL.h"
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

Shader shader;

GLuint texture;
GLuint normal_texture;

GLuint loadTextureRAW( const char * filename, int width, int height )  
{  
	GLuint texture;  
	unsigned char * data;  
	FILE * file;  
	  
	//The following code will read in our RAW file  
	file = fopen( filename, "rb" );  
	  
	if ( file == NULL ) return 0;  
	data = (unsigned char *)malloc( width * height * 3 );  
	fread( data, width * height * 3, 1, file );  
	  
	fclose( file );  
	  
	glGenTextures( 1, &texture ); //generate the texture with the loaded data  
	glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it’s array  
	  
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE ); //set texture environment parameters  
	  
	//And if you go and use extensions, you can use Anisotropic filtering textures which are of an  
	//even better quality, but this will do for now.  
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
	  
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
	  
	//Here we are setting the parameter to repeat the texture instead of clamping the texture  
	//to the edge of our shape.  
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );  
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );  
	  
	//Generate the texture  
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  
	  
	free( data ); //free the texture  
	  
	return texture; //return whether it was successfull  
}  

void FreeTexture( GLuint texture )  
{  
	glDeleteTextures( 1, &texture );  
}  

void drawSquare2D()
{
	glActiveTexture(GL_TEXTURE0);  
	glEnable(GL_TEXTURE_2D);  
	int texture_location = glGetUniformLocation(shader.id(), "color_texture");  
	glUniform1i(texture_location, 0);  
	glBindTexture(GL_TEXTURE_2D, texture);  

	glActiveTexture(GL_TEXTURE1);  
	glEnable(GL_TEXTURE_2D);  
	int normal_location = glGetUniformLocation(shader.id(), "normal_texture");  
	glUniform1i(normal_location, 1);  
	glBindTexture(GL_TEXTURE_2D, normal_texture);  

	glBegin(GL_TRIANGLES);
	glColor3f(0.196, 0.6, 0.8);
	glTexCoord2f(1.0f,1.0f);
	glVertex2f(1.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex2f(-1.0f,-1.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex2f(-1.0f,1.0f);
	glColor3f(0.196, 0.6, 0.8);
	glTexCoord2f(1.0f,1.0f);
	glVertex2f(1.0f,1.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex2f(-1.0f,-1.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex2f(1.0f,-1.0f);
	glEnd();


	glActiveTexture(GL_TEXTURE1);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D);  
	  
	glActiveTexture(GL_TEXTURE0);  
	glBindTexture(GL_TEXTURE_2D, 0);  
	glDisable(GL_TEXTURE_2D); 
}

void drawSquare()
{
	// glActiveTexture(GL_TEXTURE0);
	// glEnable(GL_TEXTURE_2D);
	// int texture_location = glGetUniformLocation
	glBegin(GL_TRIANGLES);
	glColor3f(0.196, 0.6, 0.8);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glTexCoord2f(0.0f,1.0f);
	glVertex3f(-1.0f,1.0f,0.0f);
	glColor3f(0.196, 0.6, 0.8);
	glTexCoord2f(1.0f,1.0f);
	glVertex3f(1.0f,1.0f,0.0f);
	glTexCoord2f(0.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glTexCoord2f(1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	glEnd();
}

void drawTeapot()
{
	glColor3f(0.196, 0.6, 0.8);
	glutWireTeapot(1);
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
	shader.bind();
	drawSquare2D();
	shader.unbind();
	glutSwapBuffers();
}

void myinit()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glewInit();
    shader.init("shader.vert", "shader.frag");  
	texture = loadTextureRAW("colour_map.raw", 256, 256);  
	normal_texture = loadTextureRAW("normal_map.raw", 256, 256); 
  }

void update(int data)
{
	glutTimerFunc(30,update,0);
	glutPostRedisplay();
}

void processNormalKeys(unsigned char key, int x,int y)
{
	double sensitivityX=1.5f;
	double sensitivityY=.15f;
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