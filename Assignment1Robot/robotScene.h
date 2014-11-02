/**
robotScene.h
By Kari McMahon in October 2014
*/
#include <string>

/* Inlcude GL_Load and GLFW */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

class robotScene
{
	public:
		robotScene();
		~robotScene();
		/* Position and view globals */
public:
		static GLfloat angle_x, angle_x_inc;
};