/**
Class that handles overall scene
Class originally developed by Iain Martin - Modified by Kari McMahon
November 2014
**/
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SOIL.lib")

#include "wrapper_glfw.h"
#include "object_ldr.h"
#include "Skybox.h"
#include "terrain_object.h"
#include "SOIL.h"
#include "tree.h"
#include <iostream>
#include <stdlib.h>
#include "points.h"
#include "firePoints.h"
#include <stack>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

//programs for shader
GLuint program, program2;
//vertex array buffer
GLuint vao;
//model
glm::mat4 model;
object_ldr cabin;


GLfloat  vx, vy, vz; //View variables
GLfloat xm, zm; //Test variables
GLfloat light_x, light_y, light_z,x,z; // Light pos
GLfloat speed; //Animation speed

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID, tex_matrixID, terrainID; // Shader 1 uniforms
GLuint textureID, textureID2, textureID3, textureID4, textureID5, textureID6, textureID7, texID, textureID8; //Texture id's
GLuint colourmodeID, pointSizeID, modelID2, projectionID2, colourmodeID2, viewID2; //Shader 2 uniforms
GLuint modelID3, projectionID3, colourmodeID3, viewID3;

points *point_anim; //instance of point animation for snow
firePoints *firePoint; //instance of fire point for fire animation


GLfloat tangle_x, tangle_y, tangle_z;
std::vector<glm::vec3> pondTopPositions, pondTopNormals;
std::vector<GLfloat> texCoords;
GLuint pondObject, pondNormalObject, pondTextureObject;
GLfloat aspect_ratio;
terrain_object terrain;
tree trees;
Skybox skybox;

void createPond();
void drawPond();
void consoleOutput();

/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	vx = vy = vz = xm = zm = 0;
	aspect_ratio = 1.3333f;
	speed = 2.0;

	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Set up terrain
	terrain.createTerrain(30, 30, 100.f, 100.f);
	terrain.createObject();
	//Set up tree
	trees.createTree();
	//Set up skybox
	skybox.createSkybox();
	//Set up firepoints
	firePoint = new firePoints(3000);
	firePoint->create();
	//Create pond
	createPond();
	//Set up snow points
	point_anim = new points(5000);
	point_anim->create();
	cabin.load_obj("farm.obj");
	//cabin.smoothNormals();
	cabin.createObject();


	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	try
	{
		program = glw->LoadShader("winterScene.vert", "winterScene.frag"); //Shader for scene
		program2 = glw->LoadShader("point_sprites.vert", "point_sprites_analytic.frag"); //Shader for point animations
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	try
	{
		//Texture loading for scene
		texID = SOIL_load_OGL_texture("bark2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID = SOIL_load_OGL_texture("snow4.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID2 = SOIL_load_OGL_texture("water3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID3 = SOIL_load_OGL_texture("purplenebula_ft.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID4 = SOIL_load_OGL_texture("purplenebula_bk.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID5 = SOIL_load_OGL_texture("purplenebula_lf.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID6 = SOIL_load_OGL_texture("purplenebula_rt.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID7 = SOIL_load_OGL_texture("purplenebula_up.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID8 = SOIL_load_OGL_texture("bark.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		/* check for an error during the load process */
		if (textureID == 0 || texID == 0 || textureID3 == 0 || textureID4 == 0 || textureID5 == 0 || textureID6 == 0 || textureID7 == 0 || textureID8 == 0)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
	
		int loc = glGetUniformLocation(program, "tex1");
		int loc2 = glGetUniformLocation(program, "tex1");
		if (loc >= 0) glUniform1i(loc, 0);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}

	//Shader 1 uniforms
	modelID = glGetUniformLocation(program, "model");
	colourmodeID = glGetUniformLocation(program, "colourmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	tex_matrixID = glGetUniformLocation(program, "texMatrix");
	normalmatrixID = glGetUniformLocation(program, "normalmatrix");
	terrainID = glGetUniformLocation(program, "terrainmode");
	lightposID = glGetUniformLocation(program, "lightpos");
	//Shader 2 uniforms
	modelID2 = glGetUniformLocation(program2, "model");
	colourmodeID2 = glGetUniformLocation(program2, "colourmode");
	viewID2 = glGetUniformLocation(program2, "view");
	projectionID2 = glGetUniformLocation(program2, "projection");
	pointSizeID = glGetUniformLocation(program2, "size");

	consoleOutput();
}

/* Called to update the display. */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);

	//Use shader 1
	glUseProgram(program);
	model = glm::mat4(1.0f);
	// Projection matrix
	glm::mat4 Projection = glm::perspective(80.0f, aspect_ratio, 0.1f, 100.0f);
	//View
	glm::mat4 View = glm::lookAt(
		glm::vec3(0,0,4),
		glm::vec3(0,0,0),
		glm::vec3(0, 1, 0)  
		);
	//View rotations
	View = glm::rotate(View, vx, glm::vec3(1, 0, 0));
	View = glm::rotate(View, vy, glm::vec3(0, 1, 0));
	//Light pos
	glm::vec4 lightpos = View *  glm::vec4(0, 0, 0, 1.0);
	//Normal matrix
	glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model)));
	//Tex transform marix
	glm::mat4 tex_transform = glm::mat4(1.0f);
	
	//Skybox creation
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(tex_matrixID, 1, GL_FALSE, &tex_transform[0][0]);
	glUniform1ui(terrainID, 1);
	glDepthMask(0);
	skybox.renderSkybox(textureID3, textureID4, textureID5, textureID6, textureID7, textureID);
	glDepthMask(1);

	//Terrain
	tex_transform = glm::mat4(1.0f);
	glm::mat4 model2 = glm::mat4(1.0f);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniform1ui(terrainID, 1);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model2[0][0]);
	glUniformMatrix4fv(tex_matrixID, 1, GL_FALSE, &tex_transform[0][0]);
	glUniform1ui(terrainID, 1);
	terrain.drawObject(0, textureID);

	//Tree 
	tex_transform = glm::mat4(1.0f);
	trees.lsystem_transform.push(glm::mat4(1.0f));
	trees.lsystem_transform.top() = glm::translate(trees.lsystem_transform.top(), glm::vec3(-3, (terrain.getHeight(-3, -8) + 1.5), -8));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &trees.lsystem_transform.top()[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniformMatrix4fv(tex_matrixID, 1, GL_FALSE, &tex_transform[0][0]);
	glUniform1ui(terrainID, 0);
	trees.trees(3, texID, modelID, colourmodeID);

	//Pond
	glm::mat4 modelPond = glm::mat4(1.0f);
	modelPond = glm::translate(modelPond, glm::vec3(3, (terrain.getHeight(3, -6) + 4.5), -6));
	modelPond = glm::scale(modelPond, glm::vec3(8, 1, 5));
	modelPond = glm::rotate(modelPond, 90.f, glm::vec3(1, 0, 0));
	tex_transform = glm::mat4(1.0f);
	tex_transform = glm::rotate(tex_transform, -tangle_x, glm::vec3(1, 0, 0));
	tex_transform = glm::rotate(tex_transform, -tangle_y, glm::vec3(0, 1, 0));
	tex_transform = glm::rotate(tex_transform, -tangle_z, glm::vec3(0, 0, 1));
	glUniformMatrix4fv(tex_matrixID, 1, GL_FALSE, &tex_transform[0][0]);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &modelPond[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniform1ui(terrainID, 0);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	drawPond();

	//Fire logs
	tex_transform = glm::mat4(1.0f);
	trees.lsystem_transform.push(glm::mat4(1.0f));
	trees.lsystem_transform.top() = glm::translate(trees.lsystem_transform.top(), glm::vec3(0, (terrain.getHeight(0, 5)), 5));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &trees.lsystem_transform.top()[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniformMatrix4fv(tex_matrixID, 1, GL_FALSE, &tex_transform[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniform1ui(terrainID, 0);
	trees.drawBranch(1, 0, texID, modelID, colourmodeID2);
	trees.drawBranch(2, 0, texID, modelID, colourmodeID2);
	trees.drawBranch(3, 0, texID, modelID, colourmodeID2);
	trees.drawBranch(4, 0, texID, modelID, colourmodeID2);

	glm::mat4 cabinmodel = glm::mat4(1.0f);
	cabinmodel = glm::translate(cabinmodel, glm::vec3(-7.6, terrain.getHeight(-7.6,4.7) - 2.0f,4.7));
	cabinmodel = glm::scale(cabinmodel, glm::vec3(0.1, 0.1, 0.05));
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &cabinmodel[0][0]);
	glUniformMatrix4fv(tex_matrixID, 1, GL_FALSE, &tex_transform[0][0]);
	glUniform1ui(terrainID, 0);
	cabin.drawObject(textureID8);
	glUseProgram(0);

	//Use shader 2
	glUseProgram(program2);

	//Snow drawing
	glm::mat4 Projection2 = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f); //Different projection for the snow
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(1 * 5, 1 * 5, 1 * 5));
	glUniformMatrix4fv(modelID2, 1, GL_FALSE, &model[0][0]);
	glUniform1ui(colourmodeID2, 0);
	glUniform1f(pointSizeID, 1);
	glUniformMatrix4fv(viewID2, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID2, 1, GL_FALSE, &Projection2[0][0]);
	point_anim->draw();
	point_anim->animate();

	//Draw fire
	for (int i = 0; i < firePoint->numpoints; i++)
	{
		firePoint->m.push(glm::mat4(1.0f));
		firePoint->m.top() = glm::translate(firePoint->m.top(), glm::vec3(0, terrain.getHeight(0, 4.6), 4.6));
		firePoint->m.top() = glm::scale(firePoint->m.top(), glm::vec3(1 * 15, 1 * 25, 1 * 5));
		glUniformMatrix4fv(viewID2, 1, GL_FALSE, &View[0][0]);
		glUniformMatrix4fv(projectionID2, 1, GL_FALSE, &Projection[0][0]);
		glUniformMatrix4fv(modelID2, 1, GL_FALSE, &firePoint->m.top()[0][0]);
		glUniform1f(pointSizeID, 4);
		firePoint->draw(i, i - 1);
		firePoint->animate(i);
		firePoint->m.pop();
	}

	glUseProgram(0);



	//Pond animations
	tangle_x += speed;
	if (tangle_x >= 50 || tangle_x <= -50) speed = -speed;
}

/**
Displays control in console
**/
void consoleOutput()
{
	std::cout << "Winter Scene controls" << std::endl;
	std::cout << "Press 1 to move camera down " << std::endl;
	std::cout << "Press 2 to move camera up " << std::endl;
	std::cout << "Press 3 to move camera right " << std::endl;
	std::cout << "Press 4 to move camera left " << std::endl;
}
/**
Sets up pond buffers
**/
void createPond()
{
	pondTopPositions.push_back(glm::vec3(0, 0, 2.5));
	texCoords.push_back(0.5);
	texCoords.push_back(0.5);
	texCoords.push_back(0.0f);
	for (int i = 0; i <= 32; i++)
	{
		double angle = (2 * 3.1415926535898 / 32) * i;
		double x = cos(angle);
		double y = sin(angle);
		pondTopPositions.push_back(glm::vec3(float(x) * 0.5, float(y) * 0.5, 2.5f));
		pondTopNormals.push_back(glm::vec3(0, 0, 1));
		texCoords.push_back((float(x) + 1.0) * 0.5);
		texCoords.push_back((float(y) + 1.0) * 0.5);
		texCoords.push_back(0.0f);
	}

	glGenBuffers(1, &pondObject);
	glBindBuffer(GL_ARRAY_BUFFER, pondObject);
	glBufferData(GL_ARRAY_BUFFER, pondTopPositions.size() * sizeof(glm::vec3), &pondTopPositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &pondNormalObject);
	glBindBuffer(GL_ARRAY_BUFFER, pondNormalObject);
	glBufferData(GL_ARRAY_BUFFER, pondTopNormals.size() * sizeof(glm::vec3), &pondTopNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &pondTextureObject);
	glBindBuffer(GL_ARRAY_BUFFER, pondTextureObject);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec3), &texCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

/**
Draws pond
**/
void drawPond()
{
	glBindBuffer(GL_ARRAY_BUFFER, pondObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, pondTextureObject);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindTexture(GL_TEXTURE_2D, textureID2);
	glDrawArrays(GL_TRIANGLE_FAN, 0, pondTopPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);
}

	
/* Called whenever the window is resized. The new window size is given, in pixels. */
static void reshape(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	aspect_ratio = ((float)w / 640.f*4.f) / ((float)h / 480.f*3.f);
}

/* change view angle, exit upon ESC */
static void keyCallback(GLFWwindow* window, int key, int s, int action, int mods)
{
	/* Enable this call if you want to disable key responses to a held down key*/
	//if (action != GLFW_PRESS) return;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == '1')	vx += 1.0f;	
	if (key == '2') vx -= 1.0f;
	if (key == '3') vy += 1.0f;
	if (key == '4') vy -= 1.0f;
	if (key == 'A') x -= 0.1f;
	if (key == 'Z') x += 0.1f;
	if (key == 'S') z -= 0.1f;
	if (key == 'X') z += 0.1f;
	std::cout << "X " << x << std::endl;
	std::cout << "Z " << z << std::endl;


}



/* Entry point of program */
int main(int argc, char* argv[])
{
	GLWrapper *glw = new GLWrapper(1024, 768, "Winter Scene");;

	if (!ogl_LoadFunctions())
	{
		fprintf(stderr, "ogl_LoadFunctions() failed. Exiting\n");
		return 0;
	}

	glw->setRenderer(display);
	glw->setKeyCallback(keyCallback);
	glw->setReshapeCallback(reshape);

	init(glw);

	glw->eventLoop();

	delete(glw);
	return 0;
}