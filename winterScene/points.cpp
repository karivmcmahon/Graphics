#include "points.h"
#include "glm/gtc/random.hpp"
#include <iostream>
/* Constructor, set initial parameters*/
points::points(GLuint number, GLfloat dist, GLfloat sp)
{
	numpoints = number;
	maxdist = dist;
	speed = sp;
}


points::~points()
{
	delete [] colours;
	delete[] vertices;
}

void points::updateParams(GLfloat dist, GLfloat sp)
{
	maxdist = dist;
	speed = sp;
}


void  points::create()
{
	vertices = new glm::vec3[numpoints];
	colours = new glm::vec3[numpoints];
	velocity = new glm::vec3[numpoints];

	/* Define random position and velocity */
	for (int i = 0; i < numpoints; i++)
	{
		vertices[i] = glm::ballRand(1.f);
		colours[i] = glm::vec3(1.0f,1.0f,1.0f);
		velocity[i] = glm::vec3(0.0f, 0.0001f, 0.0f);
	}

	/* Create the vertex buffer object */
	/* and the vertex buffer positions */
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), colours, GL_STATIC_DRAW);
}


void points::draw()
{
	/* Bind  vertices. Note that this is in attribute index 0 */
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Bind cube colours. Note that this is in attribute index 1 */
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	/* Draw our points*/
	glDrawArrays(GL_POINTS, 0, numpoints);
}


void points::animate()
{
	for (int i = 0; i < numpoints; i++)
	{
		
		
		if (vertices[i].y <= -0.6)
		{
			vertices[i] == glm::vec3(0, 0, 0);
		}
	else
		{
			// Add velocity to the vertices 
			vertices[i] -= velocity[i] * 4.f;
		}


		
		
	}
	
	// Update the vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);
}
