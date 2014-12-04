/**
Class that draws the fire particles
Based on Iain Martins point sprite example and this tutorial http://talkera.org/opengl/fire-in-opengl-particle-engine/
Modified by Kari McMahon Nov 2014
**/
#include "firePoints.h"
#include "glm/gtc/random.hpp"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stack>

/* Constructor, set initial parameters*/
firePoints::firePoints(GLuint number)
{
	numpoints = number;
	pi = 3.1415926535898;
}

//Destructor
firePoints::~firePoints()
{
	delete[] colours;
	delete[] vertices;
}


/**
Creates the fire points
**/
void  firePoints::create()
{
	vertices = new glm::vec3[numpoints];
	colours = new glm::vec3[numpoints];
	velocity = new glm::vec3[numpoints];
	
	//	 Define random position and velocity 
	for (int i = 0; i < numpoints; i++)
	{
		vertices[i] = glm::vec3(glm::ballRand(glm::linearRand(0.0,0.03)));
		colours[i] = glm::vec3(1.0, glm::linearRand(0.3, 0.9), 0.0); //Create orange like colours
	}

	
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), colours, GL_STATIC_DRAW);

}

/**
Draws the fire points 
Parameters :
	int i - current point
	int prev - previous point
**/
void firePoints::draw(int i, int prev)
{
	
	/* Bind  vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_POINTS, prev, i);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
		
}

/**
Animate fire points to create a bonfire look
Parameters :
	int i - current point
**/
void firePoints::animate(int i)
{
	vertices[i].y += 0.015f * i / 300;
	if (vertices[i].y > 0.04) //if y reaches this point then create new rand pos
	{
		vertices[i] = glm::vec3(glm::ballRand(glm::linearRand(0.0,0.03)));
	}
	// Update the vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);
}





