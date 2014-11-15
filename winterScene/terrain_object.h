#pragma once
/* terrain_object.h
Example class to show how to create and render a height map
Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class terrain_object
{
public:
	terrain_object();
	~terrain_object();

	void calculateNoise(GLfloat freq, GLfloat scale);
	void createTerrain(GLuint xp, GLuint yp, GLfloat xs, GLfloat ys);
	void calculateNormals();

	void createObject();
	void drawObject(int drawmode, GLuint textureID);

	glm::vec3 *vertices;
	glm::vec3 *normals;
	std::vector<GLushort> elements;
	std::vector<glm::vec3> texCoords;
	GLfloat* noise;

	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint vbo_mesh_tex;
	GLuint ibo_mesh_elements;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
	GLuint attribute_t_coord;
	GLuint xsize;
	GLuint zsize;
	GLfloat width;
	GLfloat height;
	GLuint perlin_octaves;
	GLfloat height_scale;
};

