/*
*  View Class for Assignment by Kari McMahon - October 2014
* Layout based on Iain Martin's lab 2 example
* /
/* Static Libs */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

#include "wrapper_glfw.h"
#include <iostream>
#include "Robot.cpp"
#include "robotScene.h"

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <vector>
#include <cmath>


GLuint program;
GLuint vao;
//View movement variables
GLfloat   vx, vy, vz, angle, speed, speed2;
//Uniforms
GLuint  viewID, projectionID, lightPosID;
GLfloat aspect_ratio = 1.3333f;
std::stack<glm::mat4> model;
Robot robot;
Shape shape;

void drawStarSky(GLfloat x, GLfloat y);
void movementConstraints();
void consoleOutput();


/*
* Initialisation method
*/
void init(GLWrapper *glw)
{
	speed = 0.5;
	speed2 = 0.10;
	//Sets up view movemenet variables
	vx = 0;
	vy = 0;
	vz = 0;
	angle = 0;


	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);
	//Sets up cube positions,normals and colurs then pass to buffer
	shape.createCube();
	//Set up cone positions, normals and colours then pass to buffer
	shape.createCone();
	//Set up bolt positions, normals and colours then pass to buffer
	shape.createBolt();
	//Set up star positions, colours and normals then pass to buffer
	shape.createStar();
	//Sets up sphere VBO
	shape.makeSphereVBO(40, 40);
	shape.createCylinder();
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
	robot.modelID = glGetUniformLocation(program, "model");
	robot.colourModeID = glGetUniformLocation(program, "colourmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	lightPosID = glGetUniformLocation(program, "lightpos");
	robot.emitModeID = glGetUniformLocation(program, "emitmode");

	consoleOutput();

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
		glm::vec3(0, 0, 4), // Camera is at (0,0,-4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up 
		);

	//Robot::model matrix : an identity matrix - Sets up the scene
	model.push(glm::mat4(1.0f));
	//Set scene
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0));
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0));
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));
	glm::vec4 lightpos = View * glm::vec4(2.0, 4.5, 4.0, 1.0); //light position
	glUniform1f(robot.colourModeID, 0);
	glUniformMatrix4fv(robot.modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniform4fv(lightPosID, 1, glm::value_ptr(lightpos));
	glUniform1ui(robot.emitModeID, 0);
	drawStarSky(0.5, 0.5);
	drawStarSky(-0.5, 0.5);
	drawStarSky(0.3, 0.9);
	drawStarSky(-0.3, 0.9);
	drawStarSky(0.9, 0.9);
	drawStarSky(-0.9, 0.9);

	//ROBOT DRAW
	model.push(model.top());
	model.top() = glm::rotate(model.top(), robot.robotRotation, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(robot.modelID, 1, GL_FALSE, &(model.top())[0][0]);
	robot.drawRobot();
	model.pop();


	//Pet Body Shape
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(0.6, -0.8, 0));
	model.top() = glm::scale(model.top(), glm::vec3(0.1, 0.1, 0.1));
	model.top() = glm::rotate(model.top(), angle, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(robot.modelID, 1, GL_FALSE, &(model.top())[0][0]);
	robot.drawPetRobot();
	model.pop();















	model.pop();


	robot.movingTail += speed;
	robot.movingLegs += speed2;
	if (robot.movingTail >= 15 || robot.movingTail <= -15) speed = -speed;
	if (robot.movingLegs >= 20 || robot.movingLegs <= -20) speed2 = -speed2;



	glUseProgram(0);

}

/**
Draws positions of stars to make the scene have a starry sky
**/
void drawStarSky(GLfloat x, GLfloat y)
{
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(x, y, 0));
	model.top() = glm::scale(model.top(), glm::vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(robot.modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniform1f(robot.colourModeID, 0);
	glUniform1ui(robot.emitModeID, 1);
	shape.drawStar();
	model.pop();
}

/**
Console output that displays controls for the application
**/
void consoleOutput()
{
	std::cout << "--------------CONTROLS-----------------" << "\n";
	std::cout << "\n";
	std::cout << "--------View----------" << "\n";
	std::cout << "Press up arrow key to move view upwards" << "\n";
	std::cout << "Press down arrow key to move view downwards" << "\n";
	std::cout << "Press left arrow key to rotate view left in the x direction" << "\n";
	std::cout << "Press right arrow key to rotate view right in the x direction" << "\n";
	std::cout << "Press K key to rotate view left in the z direction" << "\n";
	std::cout << "Press L key to rotate view right in the z direction" << "\n";
	std::cout << "\n";
	std::cout << "-------Robot---------" << "\n";
	std::cout << "Please note there are constraints on the robots movements so they seem realistic" << "\n";
	std::cout << "Press Q key to rotate robot left and W key to rotate robot right" << "\n";
	std::cout << "Press Y key and U key to move in the x direction" << "\n";
	std::cout << "Press A key to move arm upwards and S key to move arm downwards" << "\n";
	std::cout << "Press D key to move forearm forward in the x direction and F key move forearm in the x direction" << "\n";
	std::cout << "Press G key to move finger inwards and press H key to move finger outwards" << "\n";
	std::cout << "Press Z and X key to move legs in x direcion" << "\n";
	std::cout << "Press C and V to move lower leg in x direction" << "\n";
	std::cout << "Press E key to rotate robot's neck left and R key to rotate robot's neck right" << "\n";
	std::cout << "--------Cat--------" << "\n";
	std::cout << "Press O key to rotate cat right and P key to rotate cat left" << "\n";
}


/**
Checks on movement constraints - especially robot to make movements seem realistic
**/
void movementConstraints()
{
	robot.robotMovementConstraints();
}

/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);

}

/** Gets key responses **/
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	if (action != GLFW_PRESS) return;

	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (k == GLFW_KEY_UP)	vx += 5.0;
	if (k == GLFW_KEY_DOWN)	vx -= 5.0;
	if (k == GLFW_KEY_LEFT)	vy += 5.0;
	if (k == GLFW_KEY_RIGHT)vy -= 5.0;
	if (k == 'K')	vz += 5.0;
	if (k == 'L')	vz -= 5.0;
	if (k == 'O')	angle += 5.0;
	if (k == 'P') angle -= 5.0;
	robot.robotKeyMoves(k, action);
	movementConstraints(); //calls movement constraints to check key pressed makes variables still within constraints

}

/* An error callback function to output GLFW errors*/
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

/* Entry point of program */
int main(int argc, char* argv[])
{

	GLWrapper *glw = new GLWrapper(1024, 768, "Robots");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}


	glw->setErrorCallback(error_callback);

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();



	delete(glw);
	return 0;
}