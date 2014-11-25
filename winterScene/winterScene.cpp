
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

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>



std::vector < int > randoms,randomsz;
GLuint program ,program2;
GLuint vao;			


/* Position and view globals */
GLfloat  vx, vy, vz;
GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID, textureID, textureID2,textureID3, textureID4,textureID5, textureID6, textureID7, texID;
GLuint colourmodeID, pointSizeID, modelID2, projectionID2, colourmodeID2, viewID2;
GLuint modelID3, projectionID3, colourmodeID3, viewID3;

points *point_anim;
GLfloat speed;
GLfloat maxdist;
GLfloat point_size;

GLfloat aspect_ratio;		
terrain_object terrain;
tree trees;
Skybox skybox;



/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	vx = vy = vz = 0;
	aspect_ratio = 1.3333f;
	
	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);
	// Create the vertex array object and make it current
	glBindVertexArray(vao);

	//Set up objects
	terrain.createTerrain(30,30, 100.f, 100.f);
	terrain.createObject();
	trees.createTree();
	skybox.createSkybox();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Load and build the vertex and fragment shaders */
	try
	{
		program = glw->LoadShader("winterScene.vert", "winterScene.frag");
		program2 = glw->LoadShader("point_sprites.vert", "point_sprites_analytic.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	try
	{
		//Frost bite 512 or hanging stone for treeS
		texID = SOIL_load_OGL_texture("bark2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID = SOIL_load_OGL_texture("snow4.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID3 = SOIL_load_OGL_texture("purplenebula_ft.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID4 = SOIL_load_OGL_texture("purplenebula_bk.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID5 = SOIL_load_OGL_texture("purplenebula_lf.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID6 = SOIL_load_OGL_texture("purplenebula_rt.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID7 = SOIL_load_OGL_texture("purplenebula_up.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		/* check for an error during the load process */
		if (textureID == 0 || texID == 0 || textureID3 == 0 || textureID4 == 0 || textureID5 == 0 || textureID6 == 0 || textureID7 == 0)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		int loc = glGetUniformLocation(program, "tex1");
		int loc2 = glGetUniformLocation(program, "tex1");
		if (loc >= 0) glUniform1i(loc, 0);
	}
	catch (std::exception &e)
	{
		printf("\nImage file loading failed.");
	}
	/* Define uniforms to send to vertex shader */
	modelID = glGetUniformLocation(program, "model");
	colourmodeID = glGetUniformLocation(program, "colourmode");
	viewID = glGetUniformLocation(program, "view");
	projectionID = glGetUniformLocation(program, "projection");
	
	
	modelID2 = glGetUniformLocation(program2, "model");
	colourmodeID2 = glGetUniformLocation(program2, "colourmode");
	viewID2 = glGetUniformLocation(program2, "view");
	projectionID2 = glGetUniformLocation(program2, "projection");
	pointSizeID = glGetUniformLocation(program2, "size");


	for (int i = 0; i < 20; i++)
	{
		randoms.push_back(rand() % 25);
		randomsz.push_back(rand() % 25 +1);
	}

	point_anim = new points(5000, maxdist, speed);
	point_anim->create();
	point_size = 1.5;
	
}

/* Called to update the display. Note that this function is called in the event loop in the wrapper
class because we registered display as a callback function */
void display()
{
	/* Define the background colour */
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/* Clear the colour and frame buffers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Enable depth test  */
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);

	/* Make the compiled shader program current */
	glUseProgram(program);

	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 Projection = glm::perspective(80.f, aspect_ratio, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 4), 
		glm::vec3(0, 0, 0), 
		glm::vec3(0, 1, 0)  
		);
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0));
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0));
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));
	glm::vec4 lightpos = View *  glm::vec4(light_x, light_y, light_z, 1.0);

	glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model)));
	
	//Skybox
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glDepthMask(0);
	skybox.renderSkybox(textureID3,textureID4, textureID5, textureID6, textureID7, textureID);
	glDepthMask(1);
	
	//Terrain
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	terrain.drawObject(0, textureID);
	
	//Trees
	for (GLuint x = 0; x < 2; x++)
	{
		trees.lsystem_transform.push(glm::mat4(1.0f));
		trees.lsystem_transform.top() = glm::translate(trees.lsystem_transform.top(), glm::vec3(randoms.at(x), terrain.getHeight(randoms.at(x), randomsz.at(x)), randomsz.at(x)));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &trees.lsystem_transform.top()[0][0]);
		trees.trees(3, texID, modelID, colourmodeID);

	}
	
	glUseProgram(0);

	//New shader in use for the snow
	glUseProgram(program2);
	glm::mat4 Projection2 = glm::perspective(30.0f, aspect_ratio, 0.1f, 100.0f); //Different projection for the snow
	model = glm::translate(model, glm::vec3(0, 0, 0));
	model = glm::scale(model, glm::vec3(1 * 5, 1 * 5, 1 * 5));
	glUniformMatrix4fv(modelID2, 1, GL_FALSE, &model[0][0]);
	glUniform1ui(colourmodeID2, 0);
	glUniform1f(pointSizeID, point_size);
	glUniformMatrix4fv(viewID2, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID2, 1, GL_FALSE, &Projection2[0][0]);
	point_anim->draw();
	point_anim->animate();
	glUseProgram(0);


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


	if (key == '1') vx += 1.0f;
	if (key == '2') vx -= 1.0f;
	if (key == '3') vy += 1.0f;
	if (key == '4') vy -= 1.0f;
	if (key == '5') vz += 1.0f;
	if (key == '6') vz -= 1.0f;
	
	
	

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

