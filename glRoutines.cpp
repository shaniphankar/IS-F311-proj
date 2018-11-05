#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include "glRoutines.h"
using namespace std;

void plotPoint(float x,float y,float z)
{
	glBegin(GL_POINTS);
	glVertex3f(x,y,z);
	glEnd();
}

void myInit (void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	// glShadeModel(GL_SMOOTH);
	// glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	// glOrtho(-500.0, 500.0, -500.0, 500.0,-500.0,500.0);
}

