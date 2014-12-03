#pragma once
/* object_ldr.h
Example class to show the start of an .obj mesh object file
loader
Iain Martin November 2014
*/

#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class object_ldr
{
public:
	object_ldr();
	~object_ldr();

	void load_obj(const char* filename);
	void drawObject(GLuint texID);
	void createObject();
	void smoothNormals();

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> out_vertices;
	std::vector<glm::vec3> out_uv;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> texture;
	std::vector<GLushort> velements;
	std::vector<GLushort> telements;
	std::vector<GLushort> nelements;



	GLuint vbo_mesh_vertices;
	GLuint vbo_mesh_normals;
	GLuint vbo_mesh_textures;
	GLuint ibo_mesh_velements;
	GLuint ibo_mesh_telements;
	GLuint ibo_mesh_nelements;
	GLuint attribute_v_coord;
	GLuint attribute_v_normal;
};
