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
GLuint program;
GLuint vao;

/* Position and view globals */
GLfloat angle_x, armangle, angle_x_inc, anglex, coneangle, xr, x ,y;
/* Uniforms*/
GLuint modelID, viewID;
GLfloat pi = 3.1415926535898;
std::stack<glm::mat4> model;
std::vector<GLfloat> conePositions;
std::vector<GLfloat> coneColours;
std::vector<GLfloat> coneNormals;

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	coneangle = -90;
	angle_x = 90;
	anglex = 0;
	angle_x_inc = 0;
	armangle = 180;
	xr = 1.0f;
	x = 0;
	y = 0;
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
		glm::vec3 normal = (cos(0.75f) * glm::vec3(cos(angle), 0, sin(angle)) + sin(angle) * glm::vec3(0, 1, 0));
		coneNormals.push_back(normal[0]);
		coneNormals.push_back(normal[1]);
		coneNormals.push_back(normal[2]);


		conePositions.push_back(x + (cos(angle * twicePi / 20) * 5.0f));
		conePositions.push_back(y + (sin(angle * twicePi / 20) * 5.0f));
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


		glm::vec3 normal = (cos(0.0f) * glm::vec3(cos(angle), 0, sin(angle)) + sin(angle) * glm::vec3(0, 1, 0));
		coneNormals.push_back(normal[0]);
		coneNormals.push_back(normal[1]);
	     coneNormals.push_back(normal[2]);


		 conePositions.push_back(x + (cos(angle * twicePi / 20) * 5.0f));
		 conePositions.push_back(y + (sin(angle * twicePi / 20) * 5.0f));
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

	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 0.1), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);



	 //Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.8, -0.8, 0.3));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.50, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	// Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.25, -0.25, 0.3));
	model.top() = glm::translate(model.top(), glm::vec3(0, -0.55, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.6, 1, 0.3));
	model.top() = glm::translate(model.top(), glm::vec3(0.01, -0.12, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);
	   
	// Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.2, 1, 0.3));
	model.top() = glm::translate(model.top(), glm::vec3(-1, -0.12, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.2, 1, 0.3));
	model.top() = glm::translate(model.top(), glm::vec3(1.1, -0.12, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);



	// Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.20, 0.9, 0.3));
	model.top() = glm::translate(model.top(), glm::vec3(0.50, -0.64, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Model matrix : an identity matrix (model will be at the origin)
	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.20, 0.9, 0.3));
	model.top() = glm::translate(model.top(), glm::vec3(-0.50, -0.64, 0)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.20, 0.2, -0.4));
	model.top() = glm::translate(model.top(), glm::vec3(-0.50, -4.25, 0.06)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);

	model.push(glm::mat4(1.0f));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::scale(model.top(), glm::vec3(-0.20, 0.2, -0.4));
	model.top() = glm::translate(model.top(), glm::vec3(0.50, -4.25, 0.06)); //rotating in clockwise direction around x-axis
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLES, 0, 36);


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
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

	model.push(glm::mat4(1.0f));
	//model.top() = glm::rotate(model.top(), xr, glm::vec3(1, 0, 0));
	//model.top() = glm::rotate(model.top(), xr, glm::vec3(0, 1, 0));
    model.top() = glm::translate(model.top(), glm::vec3(0, 0.6, 0)); //rotating in clockwise direction around x-axis
	
	model.top() = glm::scale(model.top(), glm::vec3(0.02, 0.2, 0.3));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	
	model.top() = glm::rotate(model.top(), -angle_x, glm::vec3(1, 0, 0));

	
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
	glDrawArrays(GL_TRIANGLE_FAN, 0, 1086);

	glFrontFace(GL_CW);

	model.push(glm::mat4(1.0f));
	//model.top() = glm::rotate(model.top(), xr, glm::vec3(1, 0, 0));
	//model.top() = glm::rotate(model.top(), xr, glm::vec3(0, 1, 0));
	model.top() = glm::translate(model.top(), glm::vec3(0, 0.6, 0)); //rotating in clockwise direction around x-axis
	model.top() = glm::scale(model.top(), glm::vec3(0.02, 0.2, 0.3));
	model.top() = glm::rotate(model.top(), -anglex, glm::vec3(0, 1, 0));
	model.top() = glm::rotate(model.top(), -angle_x, glm::vec3(1, 0, 0));
	
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	model.pop();
   glDrawArrays(GL_TRIANGLE_FAN, 1086, 2172);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glUseProgram(0);

	
	

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



