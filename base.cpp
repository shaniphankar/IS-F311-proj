#include <stdio.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <GL/glut.h>
#include "glRoutines.h"

void myDisplay(void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1000,1000);
	glutInitWindowPosition(100,150);
	glutCreateWindow ("Wrinkles");
	glutDisplayFunc(myDisplay);	
	myInit ();
	glutMainLoop();
}