#pragma once

#include <glm/glm.hpp>
#include "wrapper_glfw.h"
#include <vector>
#include <stack>

class points
{
public:
	points(GLuint number, GLfloat dist, GLfloat sp);
	~points();

	void create();
	void draw();
	void animate();
	void updateParams(GLfloat dist, GLfloat sp);
	void drawBranch(int rotate, int color, GLuint texID, GLuint transID, GLuint colorID);
	void createTree();

	glm::vec3 *vertices;
	glm::vec3 *colours;
	glm::vec3 *velocity;
	std::stack<glm::mat4> lsystem_transform;
	std::vector<glm::vec3> cylinderStripPositions, cylinderTopPositions, cylinderBottomPositions;
	std::vector<GLfloat> cylinderStripColours, cylinderTopColours, cylinderBottomColours;
	std::vector<glm::vec3> cylinderStripNormals, cylinderTopNormals, cylinderBottomNormals;
	GLuint cylinderStripBufferObject, cylinderStripNormalObject, cylinderStripColourObject, cylinderStripTextureObject;
	GLuint cylinderTopBufferObject, cylinderTopNormalObject, cylinderTopColourObject, cylinderTopTextureObject;
	GLuint cylinderBottomBufferObject, cylinderBottomNormalObject, cylinderBottomColourObject, cylinderBottomTextureObject;
	std::vector<GLfloat> texCoords, stripTexCoords;
	bool *active;
	GLuint *bounceCount;

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
