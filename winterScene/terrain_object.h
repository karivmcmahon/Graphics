
/* terrain_object.h
Modified verision of Iain Martin's header
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
	GLfloat getHeight(GLuint x, GLuint z);

	glm::vec3 *vertices;
	glm::vec3 *normals;
	std::vector<GLushort> elements;
	std::vector<glm::vec3> texCoords;
	std::vector<glm::vec3> pos;
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
	GLfloat frequency;
	GLfloat scalee;
};

