/**
Header for snow points based on Iain Martins example (point sprites)
November 2014
**/

#pragma once

#include <glm/glm.hpp>
#include "wrapper_glfw.h"

class points
{
public:
	points(GLuint number);
	~points();

	void create();
	void draw();
	void animate();

	glm::vec3 *vertices;
	glm::vec3 *colours;
	glm::vec3 *velocity;

	GLuint numpoints;		// Number of particles
	GLuint vertex_buffer;
	GLuint colour_buffer;

	// Particle speed
	GLfloat speed;

	// Particle max distance fomr the origin before we change direction back to the centre
	GLfloat maxdist;
};