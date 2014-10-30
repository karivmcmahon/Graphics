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
GLfloat coneRotation, elbowBasedMovement, robotRotation,armMoving,armUpDownMovement,  neckmovement, legmovement, fingerMovement, fingerPosition, x , y, vx, vy , vz, kneeMovement;
/* Uniforms*/
GLuint modelID, viewID,colourModeID;
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
void drawCube();
void drawHat();
void drawHead();
void drawNeck();
void drawBody();
void drawArm();
void drawLeg();
void createCone();
void movementConstraints();
void setUpCube();
void setupConeBuffers();

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{	
	coneRotation = 90;
	robotRotation = 0;
	neckmovement = 0;
	fingerMovement = 0;
	elbowBasedMovement = 0;
	legmovement = 0;
	armUpDownMovement = 0;
	armMoving = 0;
	x = 0;
	y = 0;
	vx = 0;
	vy = 0;
	vz = 0;
	fingerPosition = 45;
	colourmode = 2;
	numlats = 40;		// Number of latitudes in our sphere
	numlongs = 40;		// Number of longitudes in our sphere

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

	createCone();
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
	colourModeID = glGetUniformLocation(program, "colourmode");
	viewID = glGetUniformLocation(program, "view");
}

void createCube()
{

}

void createCone()
{
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
		glm::vec3 normal = (cos(0.75f) * glm::vec3(cos((angle * twicePi / 20) * 0.5f)), 0, sin((angle * twicePi / 20) * 0.5f) + sin((angle * twicePi / 20) * 0.5f) * glm::vec3(0, 1, 0));
		coneNormals.push_back(normal[0]);
		coneNormals.push_back(normal[1]);
		coneNormals.push_back(normal[2]);


		conePositions.push_back(x + (cos(angle * twicePi / 20) * 0.5f));
		conePositions.push_back(y + (sin(angle * twicePi / 20) * 0.5f));
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


		glm::vec3 normal = (cos(0.0f) * glm::vec3(cos((angle * twicePi / 20) * 0.5f)), 0, sin((angle * twicePi / 20) * 0.5f) + sin((angle * twicePi / 20) * 0.5f) * glm::vec3(0, 1, 0));
		coneNormals.push_back(normal[0]);
		coneNormals.push_back(normal[1]);
		coneNormals.push_back(normal[2]);


		conePositions.push_back(x + (cos(angle * twicePi / 20) * 0.5f));
		conePositions.push_back(y + (sin(angle * twicePi / 20) * 0.5f));
		conePositions.push_back(0.0f);


		coneColours.push_back(0.0f);
		coneColours.push_back(0.0f);
		coneColours.push_back(1.0f);
		coneColours.push_back(1.0f);



	}


	std::cout << conePositions.size() << "\n";
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
		View = glm::rotate(View, -vx, glm::vec3(1, 0, 0));
		View = glm::rotate(View, -vy, glm::vec3(0, 1, 0));
		model.top() = glm::rotate(model.top(), -robotRotation, glm::vec3(0, 1, 0));
		
		//View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));
	    glUniform1f(colourModeID, 0);
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
		drawRobot();
	model.pop();

	movementConstraints();	
	glUseProgram(0);

}

void movementConstraints()
{
	if (legmovement >= 45)
	{
		legmovement = 45;
	}
	if (legmovement <= -45)
	{
		legmovement = -45;
	}
	if (kneeMovement >= 45)
	{
		kneeMovement = 45;
	}
	if (kneeMovement <= -45)
	{
		kneeMovement = -45;
	}
	if (neckmovement >= 90)
	{
		neckmovement = 90;
	}
	if( neckmovement <= -90)
	{
		neckmovement = -90;
	}
	if (fingerMovement >= 20)
	{
		fingerMovement = 20;
	}
	if (fingerMovement <= 0)
	{
		fingerMovement = 0;
	}
	if (armUpDownMovement >= 170)
	{
		armUpDownMovement = 170;
	}
	if (armUpDownMovement <= 0)
	{
		armUpDownMovement = 0;
	}
	if (elbowBasedMovement <= 0)
	{
		elbowBasedMovement = 0;
	}
	if (elbowBasedMovement >= 120)
	{
		elbowBasedMovement = 120;
	}
	if (armMoving <= -60)
	{
		armMoving = -60;
	}
	if (armMoving >= 60)
	{
		armMoving = 60;
	}
}

void drawHat()
{
	//HAT
	setupConeBuffers();
	model.push(glm::mat4(1.0f));
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.7, 0)); 
		model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.2, 0.1));
		model.top() = glm::rotate(model.top(), -neckmovement, glm::vec3(0, 1, 0));
		model.top() = glm::rotate(model.top(), -robotRotation, glm::vec3(0, 1, 0));
		model.top() = glm::rotate(model.top(), -coneRotation, glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 276);
	model.pop();
	glFrontFace(GL_CW);
	model.push(glm::mat4(1.0f));
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.8, 0)); 
		model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.3, 0.1));
		model.top() = glm::rotate(model.top(), -neckmovement, glm::vec3(0, 1, 0));
		model.top() = glm::rotate(model.top(), -robotRotation, glm::vec3(0, 1, 0));
		model.top() = glm::rotate(model.top(), -coneRotation, glm::vec3(1, 0, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glDrawArrays(GL_TRIANGLE_FAN, 276, 552);
	model.pop();
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void drawHead()
{
	//HEAD
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.50, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.8, 0.8, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		drawCube();
	model.pop();
}

void drawNeck()
{
	//NECK
	model.push(glm::mat4(model.top()));
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.24, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.25, 0.25, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 0);
		drawCube();
	model.pop();
}

void drawBody()
{
	////BODY
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0.0, -0.07, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.6, 1, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		
		drawCube();
	model.pop();
}

void drawArm(GLfloat x, GLfloat side)
{
	
	//Transform for arm
	model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(x, 0.10, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		
		//SHOULDER SOCKET
		model.push(model.top());
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			drawSphere();
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
			drawCube();
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
					drawSphere();
				model.pop();

				//ARM
				model.push(model.top());
					model.top() = glm::rotate(model.top(), elbowBasedMovement, glm::vec3(1, 0, 0));
					model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0));
					model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					drawCube();
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
						drawSphere();
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
						drawCube();
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
						drawCube();
					model.pop();

			model.pop();
		model.pop();
	model.pop();
}

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
			drawSphere();
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

			//model.top() = glm::translate(model.top(), glm::vec3(0, -0.15, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.5, 0.3));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			drawCube();
		model.pop();

		model.push(model.top());
		
		model.top() = glm::translate(model.top(), glm::vec3(0, -0.17, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);

		//LEG CONNECTOR
		model.push(model.top());

		//model.top() = glm::translate(model.top(), glm::vec3(0, -0.17, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			drawSphere();
		model.pop();

		
		
		model.push(model.top());

			model.top() = glm::rotate(model.top(), -kneeMovement, glm::vec3(1, 0, 0));
		
		model.top() = glm::translate(model.top(), glm::vec3(0, -0.17, 0));
		model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.5, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		drawCube();
		model.pop();

		////SHOE
		model.push(model.top());
		model.top() = glm::rotate(model.top(), -kneeMovement, glm::vec3(1, 0, 0));
		model.top() = glm::translate(model.top(), glm::vec3(0, -0.27, -0.05));
		model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.5));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		drawCube();
		model.pop();
		model.pop();
		model.pop();
	model.pop();
}

void drawRobot()
{
	model.push(model.top());
		model.top() = glm::rotate(model.top(), -neckmovement, glm::vec3(0, 1, 0));
		model.top() = glm::translate(model.top(), glm::vec3(0, 0, 0)); 
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		
		drawHat();
		drawHead();
		//HEAD
		model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0.1, 0.55, -0.05));
		model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 1);
		drawCube();
		model.pop();

		//HEAD
		model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(-0.1, 0.55, -0.05));
		model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.3));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 1);
		drawCube();
		model.pop();

		//HEAD
		model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(0, 0.40, -0.05));
		model.top() = glm::scale(model.top(), glm::vec3(0.25, 0.05, 0.2));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 2);
		drawCube();
		model.pop();

		drawNeck();
	model.pop();

	drawBody();
	drawArm(0.20, 1);
	drawArm(-0.20, 0);
	drawLeg(0.1, 1);
	drawLeg(-0.1, 0);

}

void drawCube()
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

	glDrawArrays(GL_TRIANGLES, 0, 36);
	
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
		robotRotation += 10.0f;


	}
	if (k == 'W')
	{
		robotRotation -= 10.0f;

	}
	if (k == 'A')
	{
		armUpDownMovement += 5 % 360;
	}
	if (k == 'S')
	{
		armUpDownMovement -= 5 % 360;
	}
	if (k == 'Z')
	{
		elbowBasedMovement += 5;
	}
	if (k == 'X')
	{
		elbowBasedMovement -= 5;
	}
	if (k == 'C')
	{
		neckmovement += 5;
	}
	if (k == 'V')
	{
		neckmovement -= 5;
	}
	if (k == 'B')
	{
		legmovement += 5;
	}
	if (k == 'N')
	{
		legmovement -= 5;
	}
	if (k == 'T')
	{
		kneeMovement += 5;
	}
	if (k == 'O')
	{
		kneeMovement -= 5;
	}
	if (k == 'F')
	{
		fingerMovement += 5;
	}
	if (k == 'G')
	{
		fingerMovement -= 5;
	}
	if (k == 'Y')
	{
		armMoving += 5;
	}
	if (k == 'U')
	{
		armMoving -= 5;
	}
	if (k == GLFW_KEY_UP)
	{
		vx += 5.0;
	}
	if (k == GLFW_KEY_DOWN)
	{
		vx -= 5.0;
	}
	if (k == GLFW_KEY_LEFT)
	{
		vy += 5.0;
	}
	if (k == GLFW_KEY_RIGHT)
	{
		vy -= 5.0;
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
/* This version uses indexed vertex buffers for both the fans at the poles and the latitude strips */
GLuint makeSphereVBO(GLuint numlats, GLuint numlongs)
{
	GLuint i, j;
	/* Calculate the number of vertices required in sphere */
	GLuint numvertices = 2 + ((numlats - 1) * numlongs);
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
	GLuint numindices = ((numlongs * 2) + 2) * (numlats - 1) + ((numlongs + 2) * 2);
	GLuint* pindices = new GLuint[numindices];

	// fill "indices" to define triangle strips
	GLuint index = 0;		// Current index

	// Define indices for the first triangle fan for one pole
	for (i = 0; i < numlongs + 1; i++)
	{
		pindices[index++] = i;
	}
	pindices[index++] = 1;	// Join last triangle in the triangle fan

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
	for (i = numvertices - 1; i > (numvertices - numlongs - 2); i--)
	{
		pindices[index++] = i;
	}
	pindices[index] = numvertices - 2;	// Tie up last triangle in fan

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


/* Define the vertex positions for a sphere. The array of vertices must have previosuly
been created.
*/
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
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

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
		/* Bind the indexed vertex buffer */
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		/* Draw the north pole regions as a triangle  */
		glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(0));

		/* Calculate offsets into the indexed array. Note that we multiply offsets by 4
		because it is a memory offset the indices are type GLuint which is 4-bytes */
		GLuint lat_offset_jump = (numlongs * 2) + 2;
		GLuint lat_offset_start = numlongs + 2;
		GLuint lat_offset_current = lat_offset_start * 4;

		/* Draw the triangle strips of latitudes */
		for (i = 0; i < numlats - 2; i++)
		{
			glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
			lat_offset_current += (lat_offset_jump * 4);
		}
		/* Draw the south pole as a triangle fan */
		glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
	}
}




