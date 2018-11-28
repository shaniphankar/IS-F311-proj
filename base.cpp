#include <iostream>
#include "shader.h"
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "SOIL.h"
#include <vector>
#include <string>
using namespace std;
//! The angle specifies the amount with which we are rotating along x-axis about up axis 
double rotate_x=0.0f;
//! The angle specifies the amount with which we are rotating along y-axis about up axis
double rotate_y=0.0f;
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

//! Initializes a shader object using the user-defined class Shader
Shader shader;

//! Intializing the texture that is bound to the square
GLuint texture;
//! Intializing the normal texture. This provides us with the displacement to the normal vector at each pixel.
GLuint normal_texture;


/*! This function loads textures of the format .RAW. 
\param filename Contains name of the image to be loaded
\param width Width of the image
\param height Height of the Image
\return The texture which the input image has been bound.
*/
GLuint loadTextureRAW( const char * filename, int width, int height )  
{  
	GLuint texture;  
	unsigned char * data;  
	FILE * file;  
	file = fopen( filename, "rb" );  
	if ( file == NULL ) return 0;  
	data = (unsigned char *)malloc( width * height * 3 );  
	// cout<<data;
	fread( data, width * height * 3, 1, file );  
	fclose( file );    
	glGenTextures( 1, &texture ); 
	glBindTexture( GL_TEXTURE_2D, texture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );  
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );    
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);  
	free( data );
	return texture; 
}  

/*! This function makes use of Simple OpenGL Image Loader library to load images as textures. 
\param filename Contains name of the image to be loaded
\return The texture which the input image has been bound.
*/

GLuint loadTextureSOIL( const char * filename)  
{
	GLuint texture= SOIL_load_OGL_texture(filename,SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	return texture;
}

/*! This function detaches the texture that has is currenlty bound in the variable texture. 
\param texture The texture that is currently bound
*/
void FreeTexture( GLuint texture )  
{  
	glDeleteTextures( 1, &texture );  
}  

/*! This function draws a simple square using GL_Triangles. The square is in X-Y plane and is centered on (0,0,0). After drawing, the two textures are also bound to the square.
*/

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

/*! This function draws a simple square using GL_Triangles. The square is in X-Y plane and is centered on (0,0,0). After drawing, no texture is bound to the square.
*/
void drawSquare()
{
	glBegin(GL_TRIANGLES);
	glColor3f(0.196, 0.6, 0.8);
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glVertex3f(-1.0f,1.0f,0.0f);
	glColor3f(0.196, 0.6, 0.8);
	glVertex3f(1.0f,1.0f,0.0f);
	glVertex3f(-1.0f,-1.0f,0.0f);
	glVertex3f(1.0f,-1.0f,0.0f);
	glEnd();
}


/*! The function that we register as OpenGL's reshape function.
param w Contains the width of the window
param h Contains the height of the window
*/
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


/*! The function that we register as OpenGL's display function. In this function, the entire shading pipeline is executed.
*/
void myDisplay(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(cameraPos[0],cameraPos[1],cameraPos[2],cameraPos[0]+directionSight[0],cameraPos[1]+directionSight[1],cameraPos[2]+directionSight[2],upVec[0],upVec[1],upVec[2]);
	clock_t begin= clock();
	glRotatef(rotate_x,0.0f,1.0f,0.0f);
	glRotatef(rotate_y,1.0f,0.0f,0.0f);
	shader.bind();
	// drawSquare();
	drawSquare2D();
	shader.unbind();
	glRotatef(-1*rotate_x,1.0f,0.0f,0.0f);
	glRotatef(-1*rotate_y,0.0f,1.0f,0.0f);
	clock_t end = clock();
	cout<< double(end-begin)/CLOCKS_PER_SEC<<endl;
	glutSwapBuffers();
}

/*! The function that we use to give initial values to our OpenGL instance. We also use this to bind our chosen images as textures to the corresponding variables.
*/
void myinit()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
    glewInit();
    shader.init("shader.vert", "shader.frag");  
	texture = loadTextureSOIL("wbs_texture_01.jpg");
    // normal_texture=loadTextureSOIL("16_face.png");
    // normal_texture=loadTextureSOIL("J3QeZ.png");
	// normal_texture=loadTextureSOIL("8ckF1.jpg");
	// normal_texture=loadTextureSOIL("dotted-leather-normal-map_300x300.jpg");
	// normal_texture=loadTextureSOIL("normal-map.jpg");
	// normal_texture=loadTextureSOIL("normal_mapping_normal_map.png");
	normal_texture=loadTextureSOIL("normal wbs_texture_08.jpg");
	// texture = loadTextureRAW("colour_map.raw", 256, 256);
	// normal_texture = loadTextureRAW("normal_map.raw", 256, 256); 
  }


/*! The function that we register as OpenGL's reshape function.
param data Integer Unused in this instance
*/
void update(int data)
{
	glutTimerFunc(30,update,0);
	glutPostRedisplay();
}

/*! This function details how to process all the keys for which ASCII values exist
\param key The key that has been pressed.
\param x Integer parameter
\param y Integer parameter
*/
void processNormalKeys(unsigned char key, int x,int y)
{
	double sensitivityX=3.0f;
	double sensitivityY=3.0f;
	// double sensitivityFOV=1.0f;
	if(key==27)
	{
		exit(0);
	}
	if(key=='W'||key=='w')
	{
		pitch+=sensitivityY/2;
	}
	if(key=='A'||key=='a')
	{
		yaw-=sensitivityX/2;
	}
	if(key=='S'||key=='s')
	{
		pitch-=sensitivityY/2;
	}
	if(key=='D'||key=='d')
	{
		yaw+=sensitivityX/2;
	}
	if(key=='I'||key=='i')
	{
		rotate_y+=sensitivityY;
	}
	if(key=='J'||key=='j')
	{
		rotate_x-=sensitivityX;
	}
	if(key=='K'||key=='k')
	{
		rotate_y-=sensitivityY;
	}
	if(key=='L'||key=='l')
	{
		rotate_x+=sensitivityX;
	}
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
/*! This function details how to process all the keys for which ASCII values do not exist
\param key The key that has been pressed.
\param x Integer parameter
\param y Integer parameter
*/
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