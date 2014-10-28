/*
 Lab2start.cpp
 Creates a cube and defines a uniform variable to pass a transformation
 to the vertx shader.
 Use this example as a start to lab2 or extract bits and add to
 an example of your own to practice working with 3D transformations
 and uniform variables.
 Iain Martin October 2014
*/

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include <iostream>

/* GLM core */
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>
#include <vector>
#include <cmath>

GLuint positionBufferObject, normalsBufferObject, colourObject, coneBufferObj, coneColourObj, coneNormalObj;
GLuint sphereBufferObject, sphereNormals, sphereColours;
GLuint elementbuffer;
GLuint program;
GLuint vao;

/* Position and view globals */
GLfloat angle_x, armangle, angle_x_inc, anglex, coneangle, xr, x, y, armymoving, elbowmoving, shoulderymoving, neckangle, legmovement,fingermoving;

/* Uniforms*/
GLuint modelID, viewID;
GLfloat pi = 3.1415926535898;
std::stack<glm::mat4> model;
std::vector<GLfloat> conePositions;
std::vector<GLfloat> coneColours;
std::vector<GLfloat> coneNormals;
GLuint drawmode;			// Defines drawing mode of sphere as points, lines or filled polygons
GLuint colourmode;
GLuint numlats, numlongs;	//Define the resolution of the sphere object
GLuint numspherevertices;
void makeUnitSphere(GLfloat *pVertices, GLuint numlats, GLuint numlongs);
GLuint makeSphereVBO(GLuint numlats, GLuint numlongs);
void drawSphere();
void drawRobot();
void setupCubeBuffers();
void disableBuffers();
void setUpCube();
void setupConeBuffers();

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	coneangle = -90;
	angle_x = 90;
	anglex = 0;
	neckangle = 0;
	fingermoving = 0;
	angle_x_inc = 0;
	legmovement = 0;
	armangle = 0;
	armymoving = 0;
	shoulderymoving = 0;
	elbowmoving = 0;
	xr = 1.0f;
	x = 0;
	y = 0;
	colourmode = 0;
	numlats = 20;		// Number of latitudes in our sphere
	numlongs = 20;		// Number of longitudes in our sphere

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);

	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	GLfloat vertexPositions[] =
	{
		-0.25f, 0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, -0.25f,

		0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, -0.25f,
		-0.25f, 0.25f, -0.25f,
		-0.25f, 0.25f, 0.25f,

		-0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, 0.25f,
		0.25f, -0.25f, -0.25f,

		0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, -0.25f,
		-0.25f, -0.25f, 0.25f,

		-0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, -0.25f,
		0.25f, 0.25f, 0.25f,

		0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, 0.25f,
		-0.25f, 0.25f, -0.25f,
	};

	/* Define an array of colours */
	float vertexColours[] = {
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,
		0.0f, 0.0f, 0.8f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,

		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,


	};

	GLfloat normals[] =
	{
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
		0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
	};


	/* Create a vertex buffer object to store vertices */
	glGenBuffers(1, &positionBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create a vertex buffer object to store vertex colours */
	glGenBuffers(1, &colourObject);
	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &normalsBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);




	conePositions.push_back(0.0f);
	conePositions.push_back(0.0f);
	conePositions.push_back(0.75f);
	
	

	coneColours.push_back(0.0f);
	coneColours.push_back(0.0f);
	coneColours.push_back(1.0f);
	coneColours.push_back(1.0f);

	GLfloat twicePi = 2.0f * pi;
	for (GLfloat angle = 0.0; angle <= 90; angle++)
	{


		// SEPERATE LOOP FOR NORMAL AFTER ALL POINTS RETRIEVED ??
		glm::vec3 normal = (cos(0.75f) * glm::vec3(cos((angle * twicePi / 20) * 3.0f)), 0, sin((angle * twicePi / 20) * 3.0f) + sin((angle * twicePi / 20) * 3.0f) * glm::vec3(0, 1, 0));
		coneNormals.push_back(normal[0]);
		coneNormals.push_back(normal[1]);
		coneNormals.push_back(normal[2]);


		conePositions.push_back(x + (cos(angle * twicePi / 20) * 3.0f));
		conePositions.push_back(y + (sin(angle * twicePi / 20) * 3.0f));
		conePositions.push_back(0.0f);
		

		coneColours.push_back(0.0f);
		coneColours.push_back(0.0f);
		coneColours.push_back(1.0f);
		coneColours.push_back(1.0f);



	}

	std::cout << conePositions.size() << "\n";

	conePositions.push_back(0.0f);
	conePositions.push_back(0.0f);
	conePositions.push_back(0.0f);
	

	for (GLfloat angle = 0.0; angle <= 90; angle++)
	{


		glm::vec3 normal = (cos(0.0f) * glm::vec3(cos((angle * twicePi / 20) * 3.0f)), 0, sin((angle * twicePi / 20) * 3.0f) + sin((angle * twicePi / 20) * 3.0f) * glm::vec3(0, 1, 0));
		coneNormals.push_back(normal[0]);
		coneNormals.push_back(normal[1]);
	     coneNormals.push_back(normal[2]);


		 conePositions.push_back(x + (cos(angle * twicePi / 20) * 3.0f));
		 conePositions.push_back(y + (sin(angle * twicePi / 20) * 3.0f));
		conePositions.push_back(0.0f);
		

		coneColours.push_back(0.0f);
		coneColours.push_back(0.0f);
		coneColours.push_back(1.0f);
		coneColours.push_back(1.0f);



	}


	std::cout << conePositions.size() << "\n";

	/* Create a vertex buffer object to store vertices */
	glGenBuffers(1, &coneBufferObj);
	glBindBuffer(GL_ARRAY_BUFFER, coneBufferObj);
	glBufferData(GL_ARRAY_BUFFER, conePositions.size(), &conePositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create a vertex buffer object to store vertex colours */
	glGenBuffers(1, &coneColourObj);
	glBindBuffer(GL_ARRAY_BUFFER, coneColourObj);
	glBufferData(GL_ARRAY_BUFFER, coneColours.size(), &coneColours[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Create a vertex buffer object to store vertex colours */
	glGenBuffers(1, &coneNormalObj);
	glBindBuffer(GL_ARRAY_BUFFER, coneNormalObj);
	glBufferData(GL_ARRAY_BUFFER, coneNormals.size(), &coneNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	numspherevertices = makeSphereVBO(numlats, numlongs);

	try
	{
		program = glw->LoadShader("lab2.vert", "lab2.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
}

//Called to update the display.
//You should call glfwSwapBuffers() after all of your rendering to display what you rendered.
void display()
{
	glEnable(GL_DEPTH_TEST);
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(program);
	

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 0.1), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	//Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	//model.top() = glm::translate(model.top(), glm::vec3(-0.2, 1, 1	));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawRobot();
	model.pop();

	std::cout << angle_x_inc << "\n";
	/*if (angle_x_inc >= 170)
	{
		angle_x_inc = 170;
	}
	if (angle_x_inc <= -90)
	{
		angle_x_inc = -90;
	}*/
	
	
	glUseProgram(0);



	
	

}

void drawRobot()
{


	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 0.1), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);

	model.push(model.top());
	model.top() = glm::rotate(model.top(), -neckangle, glm::vec3(0, 1, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, 0, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	disableBuffers();

	//HAT
	setupConeBuffers();

	model.push(glm::mat4(1.0f));
	model.top() = glm::translate(model.top(), glm::vec3(0, 0.7, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.02, 0.2, 0.3));
	model.top() = glm::rotate(model.top(), -neckangle, glm::vec3(0, 1, 0));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::rotate(model.top(), -angle_x, glm::vec3(1, 0, 0));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 276);
	model.pop();


	glFrontFace(GL_CW);

	model.push(glm::mat4(1.0f));
	model.top() = glm::translate(model.top(), glm::vec3(0, 0.8, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.02, 0.2, 0.3));
	model.top() = glm::rotate(model.top(), -neckangle, glm::vec3(0, 1, 0));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::rotate(model.top(), -angle_x, glm::vec3(1, 0, 0));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLE_FAN, 276, 552);
	model.pop();

	disableBuffers();




	setupCubeBuffers();
	//HEAD
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(0, 0.50, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.8, 0.8, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();

	//NECK
	model.push(glm::mat4(model.top()));
	model.top() = glm::translate(model.top(), glm::vec3(0, 0.23, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.25, 0.25, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();

	model.pop();

	////BODY
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(0.0, -0.09, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.6, 1, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();

	

	//Transform for whole left arm
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(-0.20, 0.10, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	disableBuffers();


	//LEFT SHOULDER SOCKET
	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();
	
	//LEFT SHOULDER
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), -armymoving, glm::vec3(0, 0, 1));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.16, 0)); 
	model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();
	disableBuffers();

	//TRANSFORM FOR LEFT ARM
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), -armymoving, glm::vec3(0, 0, 1));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.27, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);

	//LEFT ELBOW SPHERE
	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();
	
	//ARM
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), angle_x_inc, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();
	
	
	//TRANSFORM FOR HAND
	model.push(model.top());
	model.top() = glm::rotate(model.top(), angle_x_inc, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.20, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	disableBuffers();

	//// LEFT HAND SPHERE
	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();

	//FINGER
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), -fingermoving, glm::vec3(0, 0, 1));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.2, 0.1));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();


	model.pop();
	model.pop();
	model.pop();

	//Transform for right arm
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(0.20, 0.10, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	disableBuffers();


	//RIGHT SHOULDER SOCKET
	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();

	//RIGHT SHOULDER
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), armymoving, glm::vec3(0, 0, 1));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.16, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();
	disableBuffers();

	//TRANSFORM FOR RIGHT ARM
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), armymoving, glm::vec3(0, 0, 1));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.27, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);

	//RIGHT ELBOW SPHERE
	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();
	setupCubeBuffers();

	//RIGHT ARM
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), angle_x_inc, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();


	//TRANSFORM RIGHT HAND
	model.push(model.top());
	model.top() = glm::rotate(model.top(), angle_x_inc, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.20, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	disableBuffers();

	//// RIGHT HAND SPHERE
	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();

	//FINGER
	setupCubeBuffers();
	model.push(model.top());
	model.top() = glm::rotate(model.top(), -fingermoving, glm::vec3(0, 0, 1));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.2, 0.1));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();

	//Transform for hand
	//model.push(model.top());
	//model.top() = glm::rotate(model.top(), armymoving, glm::vec3(0, 0, 1));
	//model.top() = glm::rotate(model.top(), angle_x_inc, glm::vec3(1, 0, 0));
	//model.top() = glm::translate(model.top(), glm::vec3(0, -0.44, 0)); //rotating in clockwise direction around x-axis
	//glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	//glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	//disableBuffers();
	//
	////Hand sphere
	//model.push(model.top());
	//model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	//glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	//glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	//drawSphere();
	//model.pop();

	////Finger
	//setupCubeBuffers();
	//model.push(model.top());
	//model.top() = glm::rotate(model.top(), fingermoving, glm::vec3(0, 0, 1));
	//model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0)); //rotating in clockwise direction around x-axis
	//model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.2, 0.1));
	//glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	//glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	//glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();
	model.pop();
	model.pop();
	
	//Transform for right  leg
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(0.1, -0.4, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	disableBuffers();
	
	//RIGHT LEG CONNECTOR
	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();
	setupCubeBuffers();

	//// RIGHT LEG
	model.push(model.top());
	model.top() = glm::rotate(model.top(), legmovement, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.3, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.2, 1, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();

	model.push(model.top());
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();
	setupCubeBuffers();

	////RIGHT SHOE
	model.push(model.top());
	model.top() = glm::rotate(model.top(), legmovement, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.5, -0.05)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.5));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();
	model.pop();
	disableBuffers();

	//Transform for left leg
	model.push(model.top());
	model.top() = glm::translate(model.top(), glm::vec3(-0.1, -0.4, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	disableBuffers();
	
	//LEFT SPHERE CONNECTOR
	model.push(model.top());
	//model.top() = glm::translate(model.top(), glm::vec3(-0.1, -0.4, 0));
	model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	drawSphere();
	model.pop();
	setupCubeBuffers();



	//// LEFT LEG
	model.push(model.top());
	model.top() = glm::rotate(model.top(), -legmovement, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.3, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.2, 1, 0.3));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();


	////LEFT SHOE
	model.push(model.top());
	model.top() = glm::rotate(model.top(), -legmovement, glm::vec3(1, 0, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.5, -0.05)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.5));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	model.pop();

	


}

void setupCubeBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);

	/* glVertexAttribPointer(index, size, type, normalised, stride, pointer)
	index relates to the layout qualifier in the vertex shader and in
	glEnableVertexAttribArray() and glDisableVertexAttribArray() */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, colourObject);
	glEnableVertexAttribArray(1);

	/* glVertexAttribPointer(index, size, type, normalised, stride, pointer)
	index relates to the layout qualifier in the vertex shader and in
	glEnableVertexAttribArray() and glDisableVertexAttribArray() */
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	


}

void disableBuffers()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}

void setupConeBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, coneBufferObj);
	glEnableVertexAttribArray(0);

	/* glVertexAttribPointer(index, size, type, normalised, stride, pointer)
	index relates to the layout qualifier in the vertex shader and in
	glEnableVertexAttribArray() and glDisableVertexAttribArray() */
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, coneColourObj);
	glEnableVertexAttribArray(1);

	/* glVertexAttribPointer(index, size, type, normalised, stride, pointer)
	index relates to the layout qualifier in the vertex shader and in
	glEnableVertexAttribArray() and glDisableVertexAttribArray() */
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glFrontFace(GL_CW);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, coneNormalObj);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

void disableConeBuffers()
{

}



/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int k, int s, int action, int mods)
{
	if (action != GLFW_PRESS) return;

	if (k == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (k == 'Q')
	{
		anglex += 10.0f;
		armangle += 0.02f;
		//angle_x += 10.0f;
	}
	if (k == 'W')
	{
		anglex -= 10.0f; 
		armangle -= 0.02f;
		//angle_x -= 10.0f;
	}
	if (k == 'A')
	{
		armymoving += 5 % 360;
	}
	if (k == 'S')
	{
		armymoving -= 5 % 360;
	}
	if (k == 'Z')
	{
		angle_x_inc += 5;
	}
	if (k == 'X')
	{
		angle_x_inc -= 5;
	}
	if (k == 'C')
	{
		neckangle += 5;
	}
	if (k == 'V')
	{
		neckangle -= 5;
	}
	if (k == 'B')
	{
		legmovement += 5;
	}
	if (k == 'N')
	{
		legmovement -= 5;
	}
	if (k == 'F')
	{
		fingermoving += 5;
	}
	if (k == 'G')
	{
		fingermoving -= 5;
	}

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

/* Make a sphere from two triangle fans (one at each pole) and triangle strips along latitudes */
GLuint makeSphereVBO(GLuint numlats, GLuint numlongs)
{
	GLuint i, j;
	/* Calculate the number of vertices required in hemisphere */
	GLuint numvertices = 2 + ((numlats - 1) * (numlongs + 1));
	GLfloat* pVertices = new GLfloat[numvertices * 3];
	GLfloat* pColours = new GLfloat[numvertices * 4];
	makeUnitSphere(pVertices, numlats, numlongs);

	/* Define colours as the x,y,z components of the sphere vertices */
	for (i = 0; i < numvertices; i++)
	{
		pColours[i * 4] = pVertices[i * 3];
		pColours[i * 4 + 1] = pVertices[i * 3 + 1];
		pColours[i * 4 + 2] = pVertices[i * 3 + 2];
		pColours[i * 4 + 3] = 1.f;
	}

	/* Generate the vertex buffer object */
	glGenBuffers(1, &sphereBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, sphereBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the normals in a buffer object */
	glGenBuffers(1, &sphereNormals);
	glBindBuffer(GL_ARRAY_BUFFER, sphereNormals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Store the colours in a buffer object */
	glGenBuffers(1, &sphereColours);
	glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/* Calculate the number of indices in our index array and allocate memory for it */
	GLuint numindices = ((numlongs * 2) + 2) * (numlats - 1) + (numlongs + 2);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index
	GLuint start = 1;		// Start index for each latitude row
	for (j = 0; j < numlats - 2; j++)
	{
		for (i = 0; i < numlongs; i++)
		{
			pindices[index++] = start + i;
			pindices[index++] = start + i + numlongs;
		}
		pindices[index++] = start; // close the triangle strip loop by going back to the first vertex in the loop
		pindices[index++] = start + numlongs; // close the triangle strip loop by going back to the first vertex in the loop

		start += numlongs;
	}

	// Define indices for the last triangle fan for the south pole region
	pindices[index++] = numvertices - 1;
	for (i = 0; i < numlongs; i++) pindices[index++] = numvertices - 1 - i;

	// Generate a buffer for the indices
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete pindices;
	delete pColours;
	delete pVertices;
	return numvertices;
}


/************************************************************/
/* Functions define primitive objects						*/
void makeUnitSphere(GLfloat *pVertices, GLuint numlats, GLuint numlongs)
{
	GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
	GLuint vnum = 0;
	GLfloat x, y, z, lat_radians, lon_radians;

	/* Define north pole */
	pVertices[0] = 0; pVertices[1] = 0; pVertices[2] = 1.f;
	vnum++;

	GLfloat latstep = 180.f / numlats;
	GLfloat longstep = 360.f / numlongs;

	/* Define vertices along latitude lines */
	for (GLfloat lat = 90.f - latstep; lat > -90.f; lat -= latstep)
	{
		lat_radians = lat * DEG_TO_RADIANS;
		for (GLfloat lon = -180.f; lon < 180.f; lon += longstep)
		{
			lon_radians = lon * DEG_TO_RADIANS;

			x = cos(lat_radians) * cos(lon_radians);
			y = cos(lat_radians) * sin(lon_radians);
			z = sin(lat_radians);

			/* Define the vertex */
			pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
			vnum++;
		}
	}
	/* Define south pole */
	pVertices[vnum * 3] = 0; pVertices[vnum * 3 + 1] = 0; pVertices[vnum * 3 + 2] = -1.f;
}

/* Draws the sphere form the previously defined vertex and index buffers */
void drawSphere()
{
	GLuint i;

	/* Draw the vertices as GL_POINTS */
	glBindBuffer(GL_ARRAY_BUFFER, sphereBufferObject);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	/* Bind the sphere normals */
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, sphereNormals);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	/* Bind the sphere colours */
	glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

	glPointSize(3.f);

	// Enable this line to show model in wireframe
	if (drawmode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (drawmode == 2)
	{
		glDrawArrays(GL_POINTS, 0, numspherevertices);
	}
	else
	{
		/* Draw the pole regions as triangle fans */
		glDrawArrays(GL_TRIANGLE_FAN, 0, numlongs + 1);

		/* Draw the latitude triangle strips */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		GLuint lat_offset = 4 * (numlongs * 2 + 2);

		for (i = 0; i < numlats - 2; i++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset*i));
		}

		glDrawElements(GL_TRIANGLE_FAN, numlongs + 1, GL_UNSIGNED_INT, (GLvoid*)(lat_offset*i + 1));
	}

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

}



