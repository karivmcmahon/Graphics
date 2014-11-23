#pragma once
#include "wrapper_glfw.h"
#include <glm/glm.hpp>
#include <stack>
#include <vector>
class tree
{
public:
	tree();
	~tree();

	void createTree();
	void drawBranch(int rotate, int color, GLuint texID, GLuint transID, GLuint colorID);
	void trees(int level, GLuint texID, GLuint transID, GLuint colorID);
	void setUp();

	char lstring[100];
	std::stack<glm::mat4> lsystem_transform;
	int treelevel;
	GLfloat pi, twicePi;
	glm::mat4 model;
	GLuint treeVbo;
	std::vector<glm::vec3> cylinderStripPositions, cylinderTopPositions, cylinderBottomPositions;
	std::vector<GLfloat> cylinderStripColours, cylinderTopColours, cylinderBottomColours;
	std::vector<glm::vec3> cylinderStripNormals, cylinderTopNormals, cylinderBottomNormals;
	GLuint cylinderStripBufferObject, cylinderStripNormalObject, cylinderStripColourObject, cylinderStripTextureObject;
	GLuint cylinderTopBufferObject, cylinderTopNormalObject, cylinderTopColourObject, cylinderTopTextureObject;
	GLuint cylinderBottomBufferObject, cylinderBottomNormalObject, cylinderBottomColourObject, cylinderBottomTextureObject;
	std::vector<GLfloat> texCoords, stripTexCoords;


};