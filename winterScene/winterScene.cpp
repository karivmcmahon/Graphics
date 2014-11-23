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
#include "tree.h"
#include <iostream>

/* Include GLM core and matrix extensions*/
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

GLuint quad_vbo, quad_normals, quad_colours, quad_tex_coords;
GLuint quad_vbo2, quad_normals2, quad_colours2, quad_tex_coords2;
GLuint quad_vbo3, quad_normals3, quad_colours3, quad_tex_coords3;
GLuint quad_vbo4, quad_normals4, quad_colours4, quad_tex_coords4;
GLuint quad_vbo5, quad_normals5, quad_colours5, quad_tex_coords5;
GLuint quad_vbo6, quad_normals6, quad_colours6, quad_tex_coords6;
GLuint program;		/* Identifier for the shader prgoram */
GLuint vao;			/* Vertex array (Containor) object. This is the index of the VAO that will be the container for
					   our buffer objects */

GLfloat zoom;

/* Position and view globals */
GLfloat  vx, vy, vz;
GLfloat light_x, light_y, light_z;

/* Uniforms*/
GLuint modelID, viewID, projectionID, lightposID, normalmatrixID, textureID, textureID2,textureID3, textureID4,textureID5, textureID6, textureID7, texID;
GLuint colourmodeID, emitmodeID;

GLfloat aspect_ratio;		/* Aspect ratio of the window defined in the reshape callback*/
terrain_object terrain;
tree trees;
void renderSkybox();
/*
This function is called before entering the main rendering loop.
Use it for all your initialisation stuff
*/
void init(GLWrapper *glw)
{
	vx = vy = vz = 0;
	aspect_ratio = 1.3333f;
	zoom = 80;
	// Generate index (name) for one vertex array object
	glGenVertexArrays(1, &vao);
	// Create the vertex array object and make it current
	glBindVertexArray(vao);
	terrain.createTerrain(30,30, 200.f, 200.f);
	terrain.createObject();
	trees.createTree();
	

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
		//Frost bite 512 or hanging stone for treeS
		texID = SOIL_load_OGL_texture("bark2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID = SOIL_load_OGL_texture("snow4.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID2 = SOIL_load_OGL_texture("star3.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID3 = SOIL_load_OGL_texture("purplenebula_ft.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID4 = SOIL_load_OGL_texture("purplenebula_bk.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID5 = SOIL_load_OGL_texture("purplenebula_lf.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID6 = SOIL_load_OGL_texture("purplenebula_rt.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		textureID7 = SOIL_load_OGL_texture("purplenebula_up.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
		/* check for an error during the load process */
		if (textureID == 0)
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

	glGenBuffers(1, &quad_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	
	//FRONT
	static const GLfloat quad_data[] =
	{
		// Vertex positions
		50.0f, 50.0f,  50.0f,
		50.0f, -10.0f, 50.0f,
		-50.0f, 50.0f, 50.0f,
		-50.0f, -10.0f,  50.0f,

		// Normals
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		// Texture coordinates. Note we only need two per vertex but have a
		// redundant third to fit the texture coords in the same buffer for this simple object
		0.0f, 1.0f, 0,
		0.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		1.0f, 0.0f, 0,
	};

	// Copy the data into the buffer. See how this example combines the vertices, normals and texture
	// coordinates in the same buffer and uses the last parameter of  glVertexAttribPointer() to
	// specify the byte offset into the buffer for each vertex attribute set.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data), quad_data, GL_STATIC_DRAW);

	//BACK
	glGenBuffers(1, &quad_vbo2);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo2);
	// Create data for our quad with vertices, normals and texturee coordinates 
	static const GLfloat quad_data2[] =
	{
		-50.0f, 50.0f, -50.0f,
		-50.0f, -10.0f, -50.0f,
		50.0f, 50.0f, -50.0f,
		50.0f, -10.0f,- 50.0f,

		// Normals
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		// Texture coordinates. Note we only need two per vertex but have a
		// redundant third to fit the texture coords in the same buffer for this simple object
		0.0f, 1.0f, 0,
		0.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		1.0f, 0.0f, 0,
	};

	// Copy the data into the buffer. See how this example combines the vertices, normals and texture
	// coordinates in the same buffer and uses the last parameter of  glVertexAttribPointer() to
	// specify the byte offset into the buffer for each vertex attribute set.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data2), quad_data2, GL_STATIC_DRAW);
	//LEFT
	glGenBuffers(1, &quad_vbo3);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo3);
	// Create data for our quad with vertices, normals and texturee coordinates 
	static const GLfloat quad_data3[] =
	{
		-50.0f, 50.0f, 50.0f,
		-50.0f, -10.0f, 50.0f,
		-50.0f, 50.0f, -50.0f,
		-50.0f, -10.0f, -50.0f,

		// Normals
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		// Texture coordinates. Note we only need two per vertex but have a
		// redundant third to fit the texture coords in the same buffer for this simple object
		0.0f, 1.0f, 0,
		0.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		1.0f, 0.0f, 0,
	};

	// Copy the data into the buffer. See how this example combines the vertices, normals and texture
	// coordinates in the same buffer and uses the last parameter of  glVertexAttribPointer() to
	// specify the byte offset into the buffer for each vertex attribute set.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data3), quad_data3, GL_STATIC_DRAW);

	//RIGHT
	glGenBuffers(1, &quad_vbo4);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo4);
	// Create data for our quad with vertices, normals and texturee coordinates 
	static const GLfloat quad_data4[] =
	{
		// Vertex positions
		50.0f, 50.0, -50.0f,
		50.0f, -10.0, -50.0f,
		50.0f, 50.0, 50.0f,
		50.0f, -10.0, 50.0f,

		// Normals
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		// Texture coordinates. Note we only need two per vertex but have a
		// redundant third to fit the texture coords in the same buffer for this simple object
		0.0f, 1.0f, 0,
		0.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		1.0f, 0.0f, 0,
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data4), quad_data4, GL_STATIC_DRAW);
	//TOP
	glGenBuffers(1, &quad_vbo5);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo5);
	// Create data for our quad with vertices, normals and texturee coordinates 
	static const GLfloat quad_data5[] =
	{
		// Vertex positions
		-50.0f, 50.0, -50.0f,
		50.0f, 50.0, -50.0f,
		-50.0f, 50.0, 50.0f,
		50.0f, 50.0, 50.0f,


		// Normals
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		// Texture coordinates. Note we only need two per vertex but have a
		// redundant third to fit the texture coords in the same buffer for this simple object
		0.0f, 1.0f, 0,
		0.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		1.0f, 0.0f, 0,
	};

	// Copy the data into the buffer. See how this example combines the vertices, normals and texture
	// coordinates in the same buffer and uses the last parameter of  glVertexAttribPointer() to
	// specify the byte offset into the buffer for each vertex attribute set.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data5), quad_data5, GL_STATIC_DRAW);

	glGenBuffers(1, &quad_vbo6);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo6);
	// Create data for our quad with vertices, normals and texturee coordinates 
	static const GLfloat quad_data6[] =
	{
		// Vertex positions
		50.0f, -10.0, -50.0f,
		-50.0f, -10.0, -50.0f,
		50.0f, -10.0, 50.0f,
		-50.0f, -10.0, 50.0f,


		// Normals
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,

		// Texture coordinates. Note we only need two per vertex but have a
		// redundant third to fit the texture coords in the same buffer for this simple object
		0.0f, 1.0f, 0,
		0.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		1.0f, 0.0f, 0,
	};

	// Copy the data into the buffer. See how this example combines the vertices, normals and texture
	// coordinates in the same buffer and uses the last parameter of  glVertexAttribPointer() to
	// specify the byte offset into the buffer for each vertex attribute set.
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_data6), quad_data6, GL_STATIC_DRAW);


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
	//trees.lsystem_transform.push(glm::mat4(1.0f));
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(zoom, aspect_ratio, 0.1f, 100.0f);
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
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);
	renderSkybox();
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projectionID, 1, GL_FALSE, &Projection[0][0]);
	glUniformMatrix3fv(normalmatrixID, 1, GL_FALSE, &normalmatrix[0][0]);
	glUniform4fv(lightposID, 1, glm::value_ptr(lightpos));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &model[0][0]);

	//trees.lsystem_transform.push(trees.lsystem_transform.top());
	terrain.drawObject(0, textureID);
	//trees.lsystem_transform.pop();
	trees.lsystem_transform.push(glm::mat4(1.0f));
	trees.lsystem_transform.top() = glm::translate(trees.lsystem_transform.top(), glm::vec3(10, -8, 0));
	trees.lsystem_transform.top() = glm::scale(trees.lsystem_transform.top(), glm::vec3(1, 1.5, 1));
	trees.lsystem_transform.top() = glm::rotate(trees.lsystem_transform.top(), -90.0f, glm::vec3(0, 1, 0));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &trees.lsystem_transform.top()[0][0]);
	trees.trees(3, texID, modelID, colourmodeID);

	
	
	

	glUseProgram(0);


}

void renderSkybox()
{
	glDepthMask(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(24 * sizeof(float)));
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo2);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(24 * sizeof(float)));
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID4);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	/* Draw our textured quad*/
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo3);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(24 * sizeof(float)));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID5);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo4);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(24 * sizeof(float)));
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID6);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo5);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(24 * sizeof(float)));
	glEnable(GL_TEXTURE_2D);
	/* Draw our textured quad*/
	glBindTexture(GL_TEXTURE_2D, textureID7);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo6);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)(12 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)(24 * sizeof(float)));
	glEnable(GL_TEXTURE_2D);
	/* Draw our textured quad*/
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDepthMask(1);



	
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
	if (key == 'A') zoom += 5.0f;
	if (key == 'S') zoom -= 5.0f;
	terrain.keyPresses(key);
	//terrain.createTerrain(10, 10, 20.f, 20.f);
	//terrain.createObject();
	

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

