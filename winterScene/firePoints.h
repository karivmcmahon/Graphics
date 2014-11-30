/**
Header for fire points based on Iain Martin's example poin sprites
November 2014
**/
#pragma once
#include <glm/glm.hpp>
#include "wrapper_glfw.h"
#include <vector>
#include <stack>

class firePoints
{
public:
	firePoints(GLuint number);
	~firePoints();

	void create();
	void draw(int i,int prev);
	void animate(int i);

	glm::vec3 *vertices;
	glm::vec3 *colours;
	glm::vec3 *velocity;
	std::stack<glm::mat4> m;

	GLuint numpoints;		// Number of particles
	GLuint vertex_buffer;
	GLuint colour_buffer;
	GLuint tex_buffer;

	// Particle speed
	GLfloat speed;
	GLfloat pi;

	// Particle max distance fomr the origin before we change direction back to the centre
	GLfloat maxdist;


};