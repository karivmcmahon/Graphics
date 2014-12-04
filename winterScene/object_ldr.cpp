/* object_ldr.cpp
Example class to show the start of an .obj mesh obkect file
loader
Iain Martin November 2014

This is a wrapper class based around the code taken from :
http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ
I made a few changes to fit in with my vertex shaders and added the code to
bind the vertex buffers.
Uses std::vector class as the containor for the array of glm::vec3 types

Modified by Kari McMahon using tutorial http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
*/

#include "object_ldr.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace std;


object_ldr::object_ldr()
{
	attribute_v_coord = 0;
	attribute_v_normal = 2;
}


object_ldr::~object_ldr()
{
}

/* Load the object, parsing the file.
Loads normals, vertex positions, texture coordinates and faces.
Parameters :
	filename - Name of file being loaded
*/
void object_ldr::load_obj(const char* filename) {
	//Attempt to open file
	FILE * file = fopen(filename, "r");
	if (!file) { cerr << "Cannot open " << filename << endl; exit(1); }

	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF) //If end of file then break
			break; 

		if (strcmp(lineHeader, "v") == 0) //If v it is a vertex - so push into vertices each point
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){ // If vt - vertex texture coordinates - so push into texture
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			texture.push_back(glm::vec3(uv,0.0)); //Add 0 to make vec3 - easier to handle
		}
		else if (strcmp(lineHeader, "vn") == 0) //If vn - vertex normal - so push onto normals
		{
			glm::vec3 vertexnormal;
			fscanf(file, "%f %f %f\n", &vertexnormal.x, &vertexnormal.y, &vertexnormal.z);
			normals.push_back(vertexnormal);
		} 
	   else if (strcmp(lineHeader, "f") == 0) //If f then these are the faces - vertice/texture/normal - the obj should be triangulated faces before reading
		{
			GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1],  &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser. Check the faces are triangulated ? \n");
			}
			velements.push_back(vertexIndex[0]); velements.push_back(vertexIndex[1]); velements.push_back(vertexIndex[2]); 
			telements.push_back(uvIndex[0]); telements.push_back(uvIndex[1]); telements.push_back(uvIndex[2]); 
			nelements.push_back(normalIndex[0]); nelements.push_back(normalIndex[1]); nelements.push_back(normalIndex[2]);
	   }
		else { /* ignoring this line */ }
	}

	
}


/* Copy the vertices, normals and textures into vertex buffers */
void object_ldr::createObject()
{
	//Gets the vertex positions based on indices
	for (unsigned int i = 0; i < velements.size(); i++){
		unsigned int vertexIndex = velements[i];
		glm::vec3 vertex = vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}
	//Gets texture coords based on indices
	for (unsigned int i = 0; i < telements.size(); i++){
		unsigned int uvIndex = telements[i];
		glm::vec3 uv = texture[uvIndex - 1];
		out_uv.push_back(uv);
	}
	//Gets normals based on indices
	for (unsigned int i = 0; i < nelements.size(); i++){
		unsigned int nIndex = nelements[i];
		glm::vec3 n = normals[nIndex - 1];
		out_normals.push_back(n);
	} 

	
	/* Generate the vertex buffer objects */
	glGenBuffers(1, &vbo_mesh_vertices);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &(out_vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_mesh_textures);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_textures);
	glBufferData(GL_ARRAY_BUFFER, out_uv.size() * sizeof(glm::vec3), &(out_uv[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &vbo_mesh_normals);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &(out_normals[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 

}

/* Enable vertex attributes and draw object
Parameters :
		textureID8 - texture to binded for the object	
*/
void object_ldr::drawObject(GLuint textureID8)
{
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		attribute_v_coord,  
		3,                 
		GL_FLOAT,           
		GL_FALSE,           
		0,                  
		0                  
		);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,  
		3,                  
		GL_FLOAT,           
		GL_FALSE,          
		0,                 
		0                   
		); 

	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_textures);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,  
		3,                 
		GL_FLOAT,           
		GL_FALSE,           
		0,                 
		0                   
		);
	
	
	glBindTexture(GL_TEXTURE_2D, textureID8);
	
	glDrawArrays(GL_TRIANGLES, 0, out_vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}