/**
Shape class stores the creation and drawing of all shapes in the application
Created by Kari McMahon October 2014
**/
#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <stack>

/* Static Libs */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

#include "wrapper_glfw.h"

/* Inlcude GL_Load and GLFW */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>


class Shape
{
	public:
		  //Pi variables for some shape calculations
		  GLfloat pi, twicePi;
		  GLfloat x, y;
		  //Vectors to store positions , colours and normals for various shapes
		  std::vector<glm::vec3> conePositions, boltPositions, starPositions, cylinderStripPositions, cylinderTopPositions, cylinderBottomPositions;
		  std::vector<GLfloat> coneColours,boltsColours,starColours, cylinderStripColours, cylinderTopColours, cylinderBottomColours;
		  std::vector<glm::vec3> coneNormals, boltsNormals, starNormals, cylinderStripNormals, cylinderTopNormals, cylinderBottomNormals;
		  std::stack<glm::mat4> model2;
		  //Objects for the buffer
		  GLuint positionBufferObject, normalsBufferObject, colourObject;
		  GLuint sphereBufferObject, sphereNormals, sphereColours, elementbuffer;
		  GLuint coneBufferObj, coneColourObj, coneNormalObj;
		  GLuint boltBufferObject, boltNormalObject, boltColourObject;
		  GLuint starBufferObject, starNormalObject, starColourObject;
		  GLuint cylinderStripBufferObject, cylinderStripNormalObject, cylinderStripColourObject;
		  GLuint cylinderTopBufferObject, cylinderTopNormalObject, cylinderTopColourObject;
		  GLuint cylinderBottomBufferObject, cylinderBottomNormalObject, cylinderBottomColourObject;
		  //Latitude and longitude for sphere
		  GLuint numlats, numlongs;
		 
		  Shape()
		  {
				pi = 3.1415926535898;
				twicePi = 2.0f * pi;
				x = 0; 
				y = 0;
				numlats = 40;
				numlongs = 40;
		  }

		  void createCylinder()
		  {
			  for (int i = 0; i <= 32; i++) {
				  double angle = (2 * pi / 32) * i;
				  double x = cos(angle);
				  double y = sin(angle);
				  cylinderStripPositions.push_back(glm::vec3(float(x) * 0.7,float(y) * 0.7,2.5f));
				  cylinderStripColours.push_back(0.0f);
				  cylinderStripColours.push_back(0.0f);
				  cylinderStripColours.push_back(1.0f);
				  cylinderStripNormals.push_back(glm::vec3(float(x), float(y), 0));
				 
				  cylinderStripPositions.push_back(glm::vec3(float(x) * 0.7, float(y) * 0.7, 0.0f));
				  cylinderStripColours.push_back(0.0f);
				  cylinderStripColours.push_back(0.0f);
				  cylinderStripColours.push_back(1.0f);
				  cylinderStripNormals.push_back(glm::vec3(float(x), float(y), 0));
				  
			  }
			  cylinderTopPositions.push_back(glm::vec3(0,0,2.5));    // Center vertex for top of cylinder.
			  
			  for (int i = 0; i <= 32; i++) {  // Vertices around the top.
				  double angle = (2 * pi / 32) * i;
				  double x = cos(angle);
				  double y = sin(angle);
				  cylinderTopPositions.push_back(glm::vec3(float(x) * 0.7, float(y) * 0.7, 2.5f));
				  cylinderTopColours.push_back(0.0f);
				  cylinderTopColours.push_back(0.0f);
				  cylinderTopColours.push_back(1.0f);
				  cylinderTopNormals.push_back(glm::vec3(0, 0, 1));
			  }

			  cylinderBottomPositions.push_back(glm::vec3(0, 0, 0));    // Center vertex for top of cylinder.

			  for (int i = 0; i <= 32; i++) {  // Vertices around the top.
				  double angle = (2 * pi / 32) * i;
				  double x = cos(angle);
				  double y = sin(angle);
				  cylinderBottomPositions.push_back(glm::vec3(float(x) * 0.7, float(y) * 0.7, 0.0f));
				  cylinderBottomColours.push_back(0.0f);
				  cylinderBottomColours.push_back(0.0f);
				  cylinderBottomColours.push_back(1.0f);
				  cylinderBottomNormals.push_back(glm::vec3(0, 0, -1));
			  }

			 
			  glGenBuffers(1, &cylinderStripBufferObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderStripBufferObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderStripPositions.size() * sizeof(glm::vec3), &cylinderStripPositions[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderStripColourObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderStripColourObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderStripColours.size(), &cylinderStripColours[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderStripNormalObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderStripNormalObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderStripNormals.size() * sizeof(glm::vec3), &cylinderStripNormals[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderTopBufferObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderTopBufferObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderTopPositions.size() * sizeof(glm::vec3), &cylinderTopPositions[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderTopColourObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderTopColourObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderTopColours.size(), &cylinderTopColours[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderTopNormalObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderTopNormalObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderTopNormals.size() * sizeof(glm::vec3), &cylinderTopNormals[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderBottomBufferObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomBufferObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderBottomPositions.size() * sizeof(glm::vec3), &cylinderBottomPositions[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderBottomColourObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomColourObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderBottomColours.size(), &cylinderBottomColours[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);

			  glGenBuffers(1, &cylinderBottomNormalObject);
			  glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomNormalObject);
			  glBufferData(GL_ARRAY_BUFFER, cylinderBottomNormals.size() * sizeof(glm::vec3), &cylinderBottomNormals[0], GL_STATIC_DRAW);
			  glBindBuffer(GL_ARRAY_BUFFER, 0);
		  }
		
	  
	     /** Creates cone positions, normals and colours and places in buffer **/
		void createCone()
		{
			conePositions.push_back(glm::vec3(0.0f, 0.0f, 0.8f));
			coneColours.push_back(0.0f);
			coneColours.push_back(0.0f);
			coneColours.push_back(1.0f);
			for (GLfloat angle = 0.0; angle <= 90; angle++)
			{
				conePositions.push_back(glm::vec3((x + (cos(angle * twicePi / 20) * 0.5f)), (y + (sin(angle * twicePi / 20) * 0.5f)), 0.0f));
				coneColours.push_back(0.0f);
				coneColours.push_back(0.0f);
				coneColours.push_back(1.0f);
			}
			
			for (int v = 0; v < conePositions.size() - sizeof(glm::vec3); v += 3)
			{
				glm::vec3 normal = glm::cross(conePositions.at(v + 1) - conePositions.at(v),
					conePositions.at(v + 2) - conePositions.at(v));
				coneNormals.push_back(normal);
				coneNormals.push_back(normal);
				coneNormals.push_back(normal);
			}
			glGenBuffers(1, &coneBufferObj);
			glBindBuffer(GL_ARRAY_BUFFER, coneBufferObj);
			glBufferData(GL_ARRAY_BUFFER, conePositions.size() * sizeof(glm::vec3), &conePositions[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &coneColourObj);
			glBindBuffer(GL_ARRAY_BUFFER, coneColourObj);
			glBufferData(GL_ARRAY_BUFFER, coneColours.size(), &coneColours[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &coneNormalObj);
			glBindBuffer(GL_ARRAY_BUFFER, coneNormalObj);
			glBufferData(GL_ARRAY_BUFFER, coneNormals.size() * sizeof(glm::vec3), &coneNormals[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		/*
		Creates a bolt like shape for the robot by setting positions, normals and colours then places in buffer
		*/
		void createBolt()
		{
			for (GLfloat length = 0; length <= 2.0f; length += 0.01)
			{
				boltPositions.push_back(glm::vec3(0.0f, 0.0f, length));
				boltsColours.push_back(0.0f);
				boltsColours.push_back(0.0f);
				boltsColours.push_back(1.0f);
				for (GLfloat angle = 0.0; angle <= 360; angle += 4)
				{
					boltPositions.push_back(glm::vec3((x + (cos(angle * twicePi / 40)) * 0.7f), (y + (sin(angle * twicePi / 40)) * 0.7f), length));
					boltsColours.push_back(0.0f);
					boltsColours.push_back(1.0f);
					boltsColours.push_back(1.0f);
				}
			}
			for (int v = 0; v < boltPositions.size(); v += 3)
			{
				glm::vec3 normal = glm::cross(boltPositions.at(v + 1) - boltPositions.at(v),
					boltPositions.at(v + 2) - boltPositions.at(v));
				boltsNormals.push_back(normal);
				boltsNormals.push_back(normal);
				boltsNormals.push_back(normal);
			}

			glGenBuffers(1, &boltBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, boltBufferObject);
			glBufferData(GL_ARRAY_BUFFER, boltPositions.size() * sizeof(glm::vec3), &boltPositions[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &boltColourObject);
			glBindBuffer(GL_ARRAY_BUFFER, boltColourObject);
			glBufferData(GL_ARRAY_BUFFER, boltsColours.size(), &boltsColours[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &boltNormalObject);
			glBindBuffer(GL_ARRAY_BUFFER, boltNormalObject);
			glBufferData(GL_ARRAY_BUFFER, boltsNormals.size() * sizeof(glm::vec3), &boltsNormals[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}


		/*
		Creates a star like shape by setting positions, colours and normals and places in buffer
		*/
		void createStar()
		{
			for (GLfloat length = 0; length < 0.5f; length += 0.01)
			{
				starPositions.push_back(glm::vec3(0.0f, 0.0f, length));
				starColours.push_back(1.0f);
				starColours.push_back(1.0f);
				starColours.push_back(1.0f);
				for (GLfloat angle = 0.0; angle <= 360; angle += 4)
				{
					starPositions.push_back(glm::vec3((x + (cos(angle * twicePi / 10)) * 1.0f), (y + (sin(angle * twicePi / 10)) * 1.0f), length));
					starColours.push_back(1.0f);
					starColours.push_back(1.0f);
					starColours.push_back(1.0f);
				}
			}
			std::cout << sizeof(glm::vec3) << "\n";
			for (int v = 0; v < starPositions.size() - sizeof(glm::vec3); v += 3)
			{
				glm::vec3 normal = glm::cross(starPositions.at(v + 1) - starPositions.at(v),
					starPositions.at(v + 2) - starPositions.at(v));
				starNormals.push_back(normal);
				starNormals.push_back(normal);
				starNormals.push_back(normal);
			}
			glGenBuffers(1, &starBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, starBufferObject);
			glBufferData(GL_ARRAY_BUFFER, starPositions.size() * sizeof(glm::vec3), &starPositions[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &starColourObject);
			glBindBuffer(GL_ARRAY_BUFFER, starColourObject);
			glBufferData(GL_ARRAY_BUFFER, starColours.size(), &starColours[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &starNormalObject);
			glBindBuffer(GL_ARRAY_BUFFER, starNormalObject);
			glBufferData(GL_ARRAY_BUFFER, starNormals.size() * sizeof(glm::vec3), &starNormals[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		/** Makes sphere which is an example from Iain Martin **/
		GLuint makeSphereVBO(GLuint numlats, GLuint numlongs)
		{
			GLuint i, j;
			/* Calculate the number of vertices required in sphere */
			GLuint numvertices = 2 + ((numlats - 1) * numlongs);
			GLfloat* pVertices = new GLfloat[numvertices * 3];
			GLfloat* pColours = new GLfloat[numvertices * 4];
			makeUnitSphere(pVertices, numlats, numlongs);

			/* Define colours as the x,y,z components of the sphere vertices */
			for (i = 0; i < numvertices; i++)
			{
				pColours[i * 4] = pVertices[i * 3];
				pColours[i * 4 + 1] = pVertices[i * 3 + 1];
				pColours[i * 4 + 2] = pVertices[i * 3 + 2];
				pColours[i * 4 + 3] = 1.f;
			}

			/* Generate the vertex buffer object */
			glGenBuffers(1, &sphereBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, sphereBufferObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numvertices * 3, pVertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			/* Store the normals in a buffer object */
			glGenBuffers(1, &sphereNormals);
			glBindBuffer(GL_ARRAY_BUFFER, sphereNormals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 3, pVertices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			/* Store the colours in a buffer object */
			glGenBuffers(1, &sphereColours);
			glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)* numvertices * 4, pColours, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			/* Calculate the number of indices in our index array and allocate memory for it */
			GLuint numindices = ((numlongs * 2) + 2) * (numlats - 1) + ((numlongs + 2) * 2);
			GLuint* pindices = new GLuint[numindices];

			// fill "indices" to define triangle strips
			GLuint index = 0;		// Current index

			// Define indices for the first triangle fan for one pole
			for (i = 0; i < numlongs + 1; i++)
			{
				pindices[index++] = i;
			}
			pindices[index++] = 1;	// Join last triangle in the triangle fan

			GLuint start = 1;		// Start index for each latitude row
			for (j = 0; j < numlats - 2; j++)
			{
				for (i = 0; i < numlongs; i++)
				{
					pindices[index++] = start + i;
					pindices[index++] = start + i + numlongs;
				}
				pindices[index++] = start; // close the triangle strip loop by going back to the first vertex in the loop
				pindices[index++] = start + numlongs; // close the triangle strip loop by going back to the first vertex in the loop

				start += numlongs;
			}

			// Define indices for the last triangle fan for the south pole region
			for (i = numvertices - 1; i > (numvertices - numlongs - 2); i--)
			{
				pindices[index++] = i;
			}
			pindices[index] = numvertices - 2;	// Tie up last triangle in fan

			// Generate a buffer for the indices
			glGenBuffers(1, &elementbuffer);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, numindices * sizeof(GLuint), pindices, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			delete pindices;
			delete pColours;
			delete pVertices;
			return numvertices;
		}


		/*
		Define the vertex positions for a sphere - Example from iain martin
		*/
		void makeUnitSphere(GLfloat *pVertices, GLuint numlats, GLuint numlongs)
		{
			GLfloat DEG_TO_RADIANS = 3.141592f / 180.f;
			GLuint vnum = 0;
			GLfloat x, y, z, lat_radians, lon_radians;

			/* Define north pole */
			pVertices[0] = 0; pVertices[1] = 0; pVertices[2] = 1.f;
			vnum++;

			GLfloat latstep = 180.f / numlats;
			GLfloat longstep = 360.f / numlongs;

			/* Define vertices along latitude lines */
			for (GLfloat lat = 90.f - latstep; lat > -90.f; lat -= latstep)
			{
				lat_radians = lat * DEG_TO_RADIANS;
				for (GLfloat lon = -180.f; lon < 180.f; lon += longstep)
				{
					lon_radians = lon * DEG_TO_RADIANS;

					x = cos(lat_radians) * cos(lon_radians);
					y = cos(lat_radians) * sin(lon_radians);
					z = sin(lat_radians);

					/* Define the vertex */
					pVertices[vnum * 3] = x; pVertices[vnum * 3 + 1] = y; pVertices[vnum * 3 + 2] = z;
					vnum++;
				}
			}
			/* Define south pole */
			pVertices[vnum * 3] = 0; pVertices[vnum * 3 + 1] = 0; pVertices[vnum * 3 + 2] = -1.f;
		}

		/**
		Draws a star shape
		**/
		void drawStar()
		{
			glBindBuffer(GL_ARRAY_BUFFER, starBufferObject);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, starColourObject);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, starNormalObject);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 18492);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}

		/**
		Draws a bolt shape
		**/
		void drawBolt()
		{
			glBindBuffer(GL_ARRAY_BUFFER, boltBufferObject);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, boltColourObject);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, boltNormalObject);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			glDrawArrays(GL_TRIANGLE_FAN, 0, 73968);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

		}

		/**
		Draws a cone shape
		**/
		void drawCone()
		{
			glBindBuffer(GL_ARRAY_BUFFER, coneBufferObj);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, coneColourObj);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glFrontFace(GL_CW);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, coneNormalObj);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 276);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

		}

		/* Draws the sphere form the previously defined vertex and index buffers - Example from iain martin */
		void drawSphere()
		{
			GLuint i;

			/* Draw the vertices as GL_POINTS */
			glBindBuffer(GL_ARRAY_BUFFER, sphereBufferObject);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);

			/* Bind the sphere normals */
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, sphereNormals);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			/* Bind the sphere colours */
			glBindBuffer(GL_ARRAY_BUFFER, sphereColours);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);

			glPointSize(3.f);

			/* Bind the indexed vertex buffer */
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

			/* Draw the north pole regions as a triangle  */
			glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(0));

			/* Calculate offsets into the indexed array. Note that we multiply offsets by 4
			because it is a memory offset the indices are type GLuint which is 4-bytes */
			GLuint lat_offset_jump = (numlongs * 2) + 2;
			GLuint lat_offset_start = numlongs + 2;
			GLuint lat_offset_current = lat_offset_start * 4;

			/* Draw the triangle strips of latitudes */
			for (i = 0; i < numlats - 2; i++)
			{
				glDrawElements(GL_TRIANGLE_STRIP, numlongs * 2 + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));
				lat_offset_current += (lat_offset_jump * 4);
			}
			/* Draw the south pole as a triangle fan */
			glDrawElements(GL_TRIANGLE_FAN, numlongs + 2, GL_UNSIGNED_INT, (GLvoid*)(lat_offset_current));

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			
		}

		/**
		Draws cube shape - Example from Iain Martin
		**/
		void createCube()
		{
			//Set up  cube positions, colours and normals then send into vertex buffers
			GLfloat vertexPositions[] =
			{
				-0.25f, 0.25f, -0.25f,
				-0.25f, -0.25f, -0.25f,
				0.25f, -0.25f, -0.25f,

				0.25f, -0.25f, -0.25f,
				0.25f, 0.25f, -0.25f,
				-0.25f, 0.25f, -0.25f,

				0.25f, -0.25f, -0.25f,
				0.25f, -0.25f, 0.25f,
				0.25f, 0.25f, -0.25f,

				0.25f, -0.25f, 0.25f,
				0.25f, 0.25f, 0.25f,
				0.25f, 0.25f, -0.25f,

				0.25f, -0.25f, 0.25f,
				-0.25f, -0.25f, 0.25f,
				0.25f, 0.25f, 0.25f,

				-0.25f, -0.25f, 0.25f,
				-0.25f, 0.25f, 0.25f,
				0.25f, 0.25f, 0.25f,

				-0.25f, -0.25f, 0.25f,
				-0.25f, -0.25f, -0.25f,
				-0.25f, 0.25f, 0.25f,

				-0.25f, -0.25f, -0.25f,
				-0.25f, 0.25f, -0.25f,
				-0.25f, 0.25f, 0.25f,

				-0.25f, -0.25f, 0.25f,
				0.25f, -0.25f, 0.25f,
				0.25f, -0.25f, -0.25f,

				0.25f, -0.25f, -0.25f,
				-0.25f, -0.25f, -0.25f,
				-0.25f, -0.25f, 0.25f,

				-0.25f, 0.25f, -0.25f,
				0.25f, 0.25f, -0.25f,
				0.25f, 0.25f, 0.25f,

				0.25f, 0.25f, 0.25f,
				-0.25f, 0.25f, 0.25f,
				-0.25f, 0.25f, -0.25f,
			};

			/* Define an array of colours */
			float vertexColours[] = {
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,

				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 0.5f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,

				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,

				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 0.5f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,

				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,
				0.5f, 0.5f, 0.5f, 1.0f,

				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 0.5f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,
				0.3f, 0.3f, 0.3f, 1.0f,


			};

			GLfloat normals[] =
			{
				0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
				0, 0, -1.f, 0, 0, -1.f, 0, 0, -1.f,
				1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
				1.f, 0, 0, 1.f, 0, 0, 1.f, 0, 0,
				0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
				0, 0, 1.f, 0, 0, 1.f, 0, 0, 1.f,
				-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
				-1.f, 0, 0, -1.f, 0, 0, -1.f, 0, 0,
				0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
				0, -1.f, 0, 0, -1.f, 0, 0, -1.f, 0,
				0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
				0, 1.f, 0, 0, 1.f, 0, 0, 1.f, 0,
			};

			glGenBuffers(1, &positionBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &colourObject);
			glBindBuffer(GL_ARRAY_BUFFER, colourObject);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColours), vertexColours, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glGenBuffers(1, &normalsBufferObject);
			glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
			glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), normals, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		/**
		Draws cube shape - Example from Iain Martin
		**/
		void drawCube()
		{
			glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
			glEnableVertexAttribArray(0);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, colourObject);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, normalsBufferObject);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, 36);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);

		}

		void drawCylinder()
		{
			glBindBuffer(GL_ARRAY_BUFFER, cylinderTopBufferObject);
			glEnableVertexAttribArray(0);


			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, cylinderTopColourObject);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, cylinderTopBufferObject);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);




			glDrawArrays(GL_TRIANGLE_FAN, 0, 1440);

			glBindBuffer(GL_ARRAY_BUFFER, cylinderStripBufferObject);
			glEnableVertexAttribArray(0);

			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, cylinderStripColourObject);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, cylinderStripBufferObject);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 2880);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomBufferObject);
			glEnableVertexAttribArray(0);


			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomColourObject);
			glEnableVertexAttribArray(1);

			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomBufferObject);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 1440);
			

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);

			


			
		}
	

	

};


