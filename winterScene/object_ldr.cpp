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


/* I don't like using namespaces in header files but have less issues with them in
seperate cpp files */
using namespace std;

/* Define the vertex attributes for vertex positions and normals.
Make these match your application and vertex shader
You might also want to add colours and texture coordinates */
object_ldr::object_ldr()
{
	attribute_v_coord = 0;
	attribute_v_normal = 2;
}


object_ldr::~object_ldr()
{
}

/* Load the object, parsing the file.
For every line begining with 'v', it adds an extra glm::vec3 to the std::vector containor
For every line beginning with 'f', it adds the "face" indices to the array of indices
Then it calculates flat shaded normals, i.e calculates the face normal and applies that to
all vertices in the face.

This function could be improved by extending the parsing to cope with face definitions with
normals defined.
*/
void object_ldr::load_obj(const char* filename) {
	FILE * file = fopen(filename, "r");
	//if (!in) { cerr << "Cannot open " << filename << endl; exit(1); }

	string line;
	while (1)
	{
		char lineHeader[128];
		// read the first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0){
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			texture.push_back(glm::vec3(uv,0.0));
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 vertexnormal;
			fscanf(file, "%f %f %f\n", &vertexnormal.x, &vertexnormal.y, &vertexnormal.z);
			normals.push_back(vertexnormal);
		} 
	   else if (strcmp(lineHeader, "f") == 0)
		{
			GLuint vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1],  &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9){
				printf("File can't be read by our simple parser : ( Try exporting with other options\n");
			}
			velements.push_back(vertexIndex[0]); velements.push_back(vertexIndex[1]); velements.push_back(vertexIndex[2]); 
			telements.push_back(uvIndex[0]); telements.push_back(uvIndex[1]); telements.push_back(uvIndex[2]); 
			nelements.push_back(normalIndex[0]); nelements.push_back(normalIndex[1]); nelements.push_back(normalIndex[2]);
	   }
		else if (line[0] == '#') { /* ignoring this line */ }
		else { /* ignoring this line */ }
	}

	
}


/* Copy the vertices, normals and element indices into vertex buffers */
void object_ldr::createObject()
{
	for (unsigned int i = 0; i < velements.size(); i++){
		unsigned int vertexIndex = velements[i];
		glm::vec3 vertex = vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < telements.size(); i++){
		unsigned int uvIndex = telements[i];
		glm::vec3 uv = texture[uvIndex - 1];
		out_uv.push_back(uv);
	}

	for (unsigned int i = 0; i < nelements.size(); i++){
		unsigned int nIndex = nelements[i];
		glm::vec3 n = normals[nIndex - 1];
		out_normals.push_back(n);
	} 

	
	/* Generate the vertex buffer object */
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
Could improve efficiency by moving the vertex attribute pointer functions to the
create object but this method is more general
This code is almost untouched fomr the tutorial code except that I changed the
number of elements per vertex from 4 to 3*/
void object_ldr::drawObject(GLuint textureID8)
{
	int size;	// Used to get the byte size of the element (vertex index) array

	// Describe our vertices array to OpenGL (it can't guess its format automatically)
	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_vertices);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		attribute_v_coord,  // attribute index
		3,                  // number of elements per vertex, here (x,y,z)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_normals);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,  // attribute index
		3,                  // number of elements per vertex, here (x,y,z)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		); 

	glBindBuffer(GL_ARRAY_BUFFER, vbo_mesh_textures);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,  // attribute index
		3,                  // number of elements per vertex, here (x,y,z)
		GL_FLOAT,           // the type of each element
		GL_FALSE,           // take our values as-is
		0,                  // no extra data between each position
		0                   // offset of first element
		);
	
	
	glBindTexture(GL_TEXTURE_2D, textureID8);
	
	glDrawArrays(GL_TRIANGLES, 0, out_vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}