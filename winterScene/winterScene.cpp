/*
 poslight.cpp
 Demonstrates a positional light with attenuation
 Displays a cube and a sphere and a small sphere to show the light position
 Includes controls to move the light source and rotate the view
 Iain Martin October 2014
*/

/* Link to static libraries, could define these as linker inputs in the project settings instead
if you prefer */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "SOIL.lib")

/* Include the header to the GLFW wrapper class which
   also includes the OpenGL extension initialisation*/
#include "wrapper_glfw.h"
#include "object_ldr.h"
#include "terrain_object.h"
#include "SOIL.h"
#include <iostream>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


GLuint program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */



/* Position and view globals */
GLfloat  vx, vy, vz;
GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID, textureID;
GLuint colourmodeID, emitmodeID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/
terrain_object terrain;


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
	terrain.createTerrain(10, 10, 40.f, 40.f);
	terrain.createObject();
	/* Load and build the vertex and fragment shaders */
	try
	{
		program = glw->LoadShader("poslight.vert", "poslight.frag");
	}
	catch (std::exception &e)
	{
		std::cout << "Caught exception: " << e.what() << std::endl;
		std::cin.ignore();
		exit(0);
	}

	try
	{
		textureID = SOIL_load_OGL_texture("snow.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		
		/* check for an error during the load process */
		if (textureID == 0)
		{
			printf("SOIL loading error: '%s'\n", SOIL_last_result());
		}
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
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

	/* Make the compiled shader program current */
	glUseProgram(program);

	// Define the model transformations for the cube
	glm::mat4 model = glm::mat4(1.0f);
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(35.0f, aspect_ratio, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0, 0, 4), // Camera is at (0,0,4), in World Space
		glm::vec3(0, 0, 0), // and looks at the origin
		glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	View = glm::rotate(View, -vx, glm::vec3(1, 0, 0));
	View = glm::rotate(View, -vy, glm::vec3(0, 1, 0));
	View = glm::rotate(View, -vz, glm::vec3(0, 0, 1));
	glm::vec4 lightpos = View *  glm::vec4(light_x, light_y, light_z, 1.0);

	// Define the normal matrix
	glm::mat3 normalmatrix = glm::transpose(glm::inverse(glm::mat3(View * model)));
	// Send our uniforms variables to the currently bound shader,
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	terrain.drawObject(0, textureID);

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
	GLWrapper *glw = new GLWrapper(1024, 768, "Lab2: Hello 3D");;

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

