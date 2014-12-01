/**
Header for the skybox
Created by Kari McMahon - November 2014
**/
#pragma once

#include "wrapper_glfw.h"
#include <vector>
#include <glm/glm.hpp>

class Skybox
{
public:
	GLuint quad_vbo_front, quad_vbo_back, quad_vbo_left, quad_vbo_right, quad_vbo_top, quad_vbo_bottom;
	GLuint quad_textures, quad_normals;
	
	Skybox();
	~Skybox();
	void createSkybox();
	void renderSkybox(GLuint textureFront, GLuint textureBack, GLuint textureLeft, GLuint textureRight, GLuint textureTop, GLuint textureBottom);
};