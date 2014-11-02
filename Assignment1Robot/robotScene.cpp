/*
*  Class for Assignment by Kari McMahon - October 2014
*  Layout based on Iain Martin's lab 2 ecample
*/

/* Static Libs */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

#include "wrapper_glfw.h"
#include <iostream>

#include "robotScene.h"
#include "Shape.cpp"
/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <vector>
#include <cmath>


GLuint program;
GLuint vao;

GLfloat  boltRotation, elbowBasedMovement, robotRotation, armMoving, armUpDownMovement, neckmovement, legmovement, fingerMovement, fingerPosition,  vx, vy, vz, kneeMovement;
GLuint modelID, viewID, colourModeID, projectionID, lightPosID, emitModeID;
GLfloat aspect_ratio = 1.3333f;
std::stack<glm::mat4> model;		
GLuint colourmode;
	
void drawRobot();
void drawHat();
void drawHead();
void drawNeck();
void drawBody();
void drawArm();
void drawLeg();
void drawEye(GLfloat x);
void drawLips();
void drawBoltButton(GLfloat y);
void drawBoltEars(GLfloat x);
void drawStarSky(GLfloat x, GLfloat y);

void movementConstraints();

GLfloat robotScene::coneRotation;
Shape s;

/*
* Initialisation method
*/
void init(GLWrapper *glw)
{
	robotScene::coneRotation = 90;
	boltRotation = 90;
	robotRotation = 0;
	neckmovement = 0;
	fingerMovement = 0;
	elbowBasedMovement = 0;
	legmovement = 0;
	armUpDownMovement = 0;
	armMoving = 0;
	vx = 0;
	vy = 0;
	vz = 0;
	fingerPosition = 45;
	colourmode = 2;
		
	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);
	//Sets up cube positions,normals and colurs then pass to buffer
	s.createCube();
	//Set up cone positions, normals and colours then pass to buffer
	s.createCone();
	//Set up bolt positions, normals and colours then pass to buffer
	s.createBolt();
	//Set up star positions, colours and normals then pass to buffer
	s.createStar();
	//Sets up sphere VBO
	s.makeSphereVBO(40, 40);
	try
	{
		program = glw->LoadShader("robotScene.vert", "robotScene.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	colourModeID = glGetUniformLocation(program, "colourmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightPosID = glGetUniformLocation(program, "lightpos");
	emitModeID = glGetUniformLocation(program, "emitmode");
}


/**
Updates display
**/
void display()
{
	glEnable(GL_DEPTH_TEST);
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);

	// Projection matrix 
	glm::mat4 Projection = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f);

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, -4), // Camera is at (0,0,-4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up 
		);

	//Model matrix : an identity matrix - Sets up the scene
	model.push(glm::mat4(1.0f));
		//SCENE
		View = glm::rotate(View, -vx, glm::vec3(1, 0, 0));
		View = glm::rotate(View, -vy, glm::vec3(0, 1, 0));
		View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));
		glm::vec4 lightpos = View * glm::vec4(2.0, 4.5, -3.0, 1.0);
		//model.top() = glm::rotate(model.top(), -robotRotation, glm::vec3(0, 1, 0));
		drawStarSky(0.5, 0.5);
		drawStarSky(-0.5, 0.5);
		drawStarSky(0.3, 0.9);
		drawStarSky(-0.3, 0.9);
		drawStarSky(0.9, 0.9);
		drawStarSky(-0.9, 0.9);
		glUniform1f(colourModeID, 0);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
		glUniform4fv(lightPosID, 1, glm::value_ptr(lightpos));
		glUniform1ui(emitModeID, 0);

		    //ROBOT
			model.push(model.top());
				model.top() = glm::rotate(model.top(), -robotRotation, glm::vec3(0, 1, 0));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
				drawRobot();
			model.pop();
	model.pop();
	//Calls movement checks - to make movement seem realistic
	movementConstraints();
	glUseProgram(0);

}

void drawStarSky(GLfloat x, GLfloat y)
{
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(x, y, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 0);
		glUniform1ui(emitModeID, 1);
		s.drawStar();
	model.pop();
}

/**
Checks on robot movement's to make the robot's movements seem realistic
**/
void movementConstraints()
{
	if (legmovement >= 45)	legmovement = 45;
	if (legmovement <= -45)	legmovement = -45;
	if (kneeMovement >= 45)	kneeMovement = 45;
	if (kneeMovement <= -45) kneeMovement = -45;
	if (neckmovement >= 90)	 neckmovement = 90;
	if (neckmovement <= -90) neckmovement = -90;
	if (fingerMovement >= 20) fingerMovement = 20;
	if (fingerMovement <= 0) fingerMovement = 0;
	if (armUpDownMovement >= 170) armUpDownMovement = 170;
	if (armUpDownMovement <= 0)	armUpDownMovement = 0;
	if (elbowBasedMovement <= 0) elbowBasedMovement = 0;
	if (elbowBasedMovement >= 120) elbowBasedMovement = 120;
	if (armMoving <= -60) armMoving = -60;
	if (armMoving >= 60) armMoving = 60;
}

/**
Draws robots cone hats
**/
void drawHat()
{
	//HAT
	model.push(glm::mat4(1.0f));
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.7, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.2, 0.1));
		model.top() = glm::rotate(model.top(), -neckmovement, glm::vec3(0, 1, 0));
		model.top() = glm::rotate(model.top(), -robotRotation, glm::vec3(0, 1, 0));
		model.top() = glm::rotate(model.top(), -(robotScene::coneRotation), glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 2);
		glUniform1ui(emitModeID, 0);
		s.drawCone();
	model.pop();
	
}

/**
Draws robots head shape
**/
void drawHead()
{
	//HEAD
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.50, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.8, 0.8, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 0);
		s.drawCube();
	model.pop();
	drawEye(0.1);
	drawEye(-0.1);
	drawLips();
	drawBoltEars(0.22);
	drawBoltEars(-0.22);
	
}

/**
Draws robots neck
**/
void drawNeck()
{
	//NECK
	model.push(glm::mat4(model.top()));
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.24, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.25, 0.25, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 0);
		s.drawCube();
	model.pop();
}
/**
Draws  robots body
**/
void drawBody()
{
	////BODY
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0.0, -0.07, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.6, 1, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		s.drawCube();
	model.pop();
	drawBoltButton(-0.07);
	drawBoltButton(0.05);
	drawBoltButton(-0.20);
	
}

void drawBoltButton(GLfloat y)
{
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0, y, -0.1));
		model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.05, 0.05));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 1);
		s.drawBolt();
	model.pop();
}
/**
Draws robots arm
**/
void drawArm(GLfloat x, GLfloat side)
{

	//Transform for arm
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(x, 0.10, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 0);
		
		//SHOULDER SOCKET
		model.push(model.top());
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			s.drawSphere();
		model.pop();

		//SHOULDER
		model.push(model.top());
			if (side == 0)
			{
				model.top() = glm::rotate(model.top(), -armMoving, glm::vec3(1, 0, 0));
				model.top() = glm::rotate(model.top(), -armUpDownMovement, glm::vec3(0, 0, 1));
			}
			else
			{
				model.top() = glm::rotate(model.top(), armMoving, glm::vec3(1, 0, 0));
				model.top() = glm::rotate(model.top(), armUpDownMovement, glm::vec3(0, 0, 1));
			}
			model.top() = glm::translate(model.top(), glm::vec3(0, -0.12, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			s.drawCube();
		model.pop();


		//TRANSFORM FOR ARM
		model.push(model.top());
			if (side == 0)
			{
				model.top() = glm::rotate(model.top(), -armMoving, glm::vec3(1, 0, 0));
				model.top() = glm::rotate(model.top(), -armUpDownMovement, glm::vec3(0, 0, 1));
			}
			else
			{
				model.top() = glm::rotate(model.top(), armMoving, glm::vec3(1, 0, 0));
				model.top() = glm::rotate(model.top(), armUpDownMovement, glm::vec3(0, 0, 1));
			}
			model.top() = glm::translate(model.top(), glm::vec3(0, -0.27, 0)); //rotating in clockwise direction around x-axis
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);

			//ELBOW SPHERE
			model.push(model.top());
				model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
				s.drawSphere();
			model.pop();

			//ARM
			model.push(model.top());
				model.top() = glm::rotate(model.top(), elbowBasedMovement, glm::vec3(1, 0, 0));
				model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0));
				model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
				s.drawCube();
			model.pop();


			//TRANSFORM FOR HAND
			model.push(model.top());
				model.top() = glm::rotate(model.top(), elbowBasedMovement, glm::vec3(1, 0, 0));
				model.top() = glm::translate(model.top(), glm::vec3(0, -0.20, 0));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);


				//// HAND SPHERE
				model.push(model.top());
					model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					s.drawSphere();
				model.pop();

				//FINGER
				model.push(model.top());
					if (side == 0)
					{
						model.top() = glm::rotate(model.top(), -fingerMovement, glm::vec3(0, 0, 1));
					}
					else
					{
						model.top() = glm::rotate(model.top(), fingerMovement, glm::vec3(0, 0, 1));
					}
					model.top() = glm::translate(model.top(), glm::vec3(0, -0.09, 0));
					model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.15, 0.1));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					s.drawCube();
				model.pop();

				//FINGER 2
				model.push(model.top());
					if (side == 0)
					{
						model.top() = glm::rotate(model.top(), fingerMovement, glm::vec3(0, 0, 1));
						model.top() = glm::rotate(model.top(), -fingerPosition, glm::vec3(0, 0, 1));
					}
					else
					{
						model.top() = glm::rotate(model.top(), -fingerMovement, glm::vec3(0, 0, 1));
						model.top() = glm::rotate(model.top(), fingerPosition, glm::vec3(0, 0, 1));
					}
					model.top() = glm::translate(model.top(), glm::vec3(0.01, -0.09, 0));
					model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.15, 0.1));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					s.drawCube();
				model.pop();
		model.pop();
	model.pop();
model.pop();
}

/** 
Draws leg for robot
**/
void drawLeg(GLfloat x, GLfloat side)
{
	//Transform for leg
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(x, -0.37, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		//LEG CONNECTOR
		model.push(model.top());
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			s.drawSphere();
		model.pop();

		model.push(model.top());
		if (side == 0)
		{
			model.top() = glm::rotate(model.top(), legmovement, glm::vec3(1, 0, 0));
		}
		else
		{
			model.top() = glm::rotate(model.top(), -legmovement, glm::vec3(1, 0, 0));
		}
		model.top() = glm::translate(model.top(), glm::vec3(0, -0.15, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);

			//// LEG
			model.push(model.top());
				model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.5, 0.3));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
				s.drawCube();
			model.pop();

			model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(0, -0.17, 0));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);

			//LEG CONNECTOR
			model.push(model.top());
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			s.drawSphere();
			model.pop();

			model.push(model.top());
			model.top() = glm::rotate(model.top(), -kneeMovement, glm::vec3(1, 0, 0));
			model.top() = glm::translate(model.top(), glm::vec3(0, -0.17, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.5, 0.3));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			s.drawCube();
			model.pop();

			////SHOE
			model.push(model.top());
			model.top() = glm::rotate(model.top(), -kneeMovement, glm::vec3(1, 0, 0));
			model.top() = glm::translate(model.top(), glm::vec3(0, -0.27, -0.05));
			model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.5));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			s.drawCube();
			model.pop();
	model.pop();
model.pop();
model.pop();
}

void drawEye(GLfloat x)
{
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(x, 0.55, -0.05));
		model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 1);
		s.drawCube();
	model.pop();
}

void drawLips()
{
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.40, -0.05));
		model.top() = glm::scale(model.top(), glm::vec3(0.25, 0.05, 0.2));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 2);
		s.drawCube();
	model.pop();
}

void drawBoltEars(GLfloat x)
{
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(x, 0.5, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.05, 0.1));
		model.top() = glm::rotate(model.top(), boltRotation, glm::vec3(0, 1, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 1);
		s.drawBolt();
	model.pop();
}

/**
Builds robot shape
**/
void drawRobot()
{
	model.push(model.top());
		model.top() = glm::rotate(model.top(), -neckmovement, glm::vec3(0, 1, 0));
		model.top() = glm::translate(model.top(), glm::vec3(0, 0, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		drawHat();
		drawHead();
		drawNeck();
	model.pop();
	drawBody();
	drawArm(0.20, 1);
	drawArm(-0.20, 0);
	drawLeg(0.1, 1);
	drawLeg(-0.1, 0);

}









/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);

}

static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	if (action != GLFW_PRESS) return;

	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (k == 'Q')	robotRotation += 10.0f;
	if (k == 'W')	robotRotation -= 10.0f;
	if (k == 'A')	armUpDownMovement += 5 % 360;
	if (k == 'S')	armUpDownMovement -= 5 % 360;
	if (k == 'Z')	elbowBasedMovement += 5;
	if (k == 'X')	elbowBasedMovement -= 5;
	if (k == 'C')	neckmovement += 5;
	if (k == 'V')	neckmovement -= 5;
	if (k == 'B')	legmovement += 5;
	if (k == 'N')	legmovement -= 5;
	if (k == 'I')	kneeMovement += 5;
	if (k == 'O')	kneeMovement -= 5;
	if (k == 'F')	fingerMovement += 5;
	if (k == 'G')	fingerMovement -= 5;
	if (k == 'Y')	armMoving += 5;
	if (k == 'U')	armMoving -= 5;
	if (k == GLFW_KEY_UP)	vx += 5.0;
	if (k == GLFW_KEY_DOWN)	vx -= 5.0;
	if (k == GLFW_KEY_LEFT)	vy += 5.0;
	if (k == GLFW_KEY_RIGHT)vy -= 5.0;
	if (k == 'K')	vz += 5.0;
	if (k == 'L')	vz -= 5.0;
	
}

/* An error callback function to output GLFW errors*/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Lab2: Hello 3D");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	/* Note it you might want to move this call to the wrapper class */
	glw->setErrorCallback(error_callback);

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}






