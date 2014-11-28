#include "points.h"
#include "glm/gtc/random.hpp"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/* Constructor, set initial parameters*/
points::points(GLuint number, GLfloat dist, GLfloat sp)
{
	numpoints = number;
	maxdist = dist;
	speed = sp;
	pi = 3.1415926535898;
}


points::~points()
{
	delete[] colours;
	delete[] vertices;
}

void points::updateParams(GLfloat dist, GLfloat sp)
{
	maxdist = dist;
	speed = sp;
}


void  points::create()
{
	vertices = new glm::vec3[numpoints];
	colours = new glm::vec3[numpoints];
	velocity = new glm::vec3[numpoints];
	active = new bool[numpoints];
	bounceCount = new GLuint[numpoints];

//	 Define random position and velocity 
	for (int i = 0; i < numpoints; i++)
	{
		//vertices[i] = glm::sphericalRand(0.05f);
		//vertices[i] = glm::vec3(glm::linearRand(0.0, 0.03), glm::linearRand(0.0, 0.1), glm::linearRand(0.0, 0.05));
		bounceCount[i] = 0;
		active[i] = true;
		vertices[i] = glm::vec3(glm::ballRand(glm::linearRand(0.0, 0.03)));
		colours[i] = glm::vec3(1.0, glm::linearRand(0.3, 0.9), 0.0);

		//velocity[i] = glm::vec3( );
	} 

	

	/* Create the vertex buffer object */
	/* and the vertex buffer positions */
	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &colour_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), colours, GL_STATIC_DRAW);

	
}


void points::draw()
{
	int prevPoint = 0;
	for (int i = 0; i < numpoints; i++)
	{
		//if (active[i] == true)
		//{
			
			/* Bind  vertices. Note that this is in attribute index 0 */
			glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			/* Bind cube colours. Note that this is in attribute index 1 */
			glBindBuffer(GL_ARRAY_BUFFER, colour_buffer);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

			/*glBindBuffer(GL_ARRAY_BUFFER, tex_buffer);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);*/

			/* Draw our points*/
		//	glBindTexture(GL_TEXTURE_2D, tex);
			glDrawArrays(GL_POINTS,0,numpoints);
			prevPoint = i;
		//}
	}
}


void points::animate()
{

	for (int i = 0; i < numpoints; i++)
	{
		
	
		
			
		vertices[i].y += 0.015f * i / 300;

		if (vertices[i].y > 0.04)
		{
			//vertices[i].y = 0;
			vertices[i] = glm::vec3(glm::ballRand(glm::linearRand(0.0, 0.03)));
	}
		
			

	}

	// Update the vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, numpoints * sizeof(glm::vec3), vertices, GL_DYNAMIC_DRAW);
}

void points::createTree()
{


	for (int i = 0; i <= 32; i++) {
		double angle = (2 * pi / 32) * i;
		double x = cos(angle);
		double y = sin(angle);
		float tc = (float)(2 * pi / 32) * i;
		stripTexCoords.push_back(tc);
		stripTexCoords.push_back(0);
		stripTexCoords.push_back(0);
		cylinderStripPositions.push_back(glm::vec3(float(x) * 0.5, float(y) * 0.5, 2.5f));
		cylinderStripNormals.push_back(glm::vec3(float(x), float(y), 0));
		stripTexCoords.push_back(tc);
		stripTexCoords.push_back(1);
		stripTexCoords.push_back(0);
		cylinderStripPositions.push_back(glm::vec3(float(x) * 0.5, float(y) * 0.5, 0.0f));
		cylinderStripNormals.push_back(glm::vec3(float(x), float(y), 0));

		tc = tc + (float)(2 * pi / 32) * i;


	}
	stripTexCoords.push_back(0);
	stripTexCoords.push_back(0);
	stripTexCoords.push_back(0);
	stripTexCoords.push_back(0);
	stripTexCoords.push_back(1);
	stripTexCoords.push_back(0);
	cylinderTopPositions.push_back(glm::vec3(0, 0, 2.5));    // Center vertex for top of cylinder.
	texCoords.push_back(0.5);
	texCoords.push_back(0.5);
	texCoords.push_back(0.0f);
	for (int i = 0; i <= 32; i++) {  // Vertices around the top.
		double angle = (2 * pi / 32) * i;
		double x = cos(angle);
		double y = sin(angle);
		cylinderTopPositions.push_back(glm::vec3(float(x) * 0.5, float(y) * 0.5, 2.5f));
		cylinderTopNormals.push_back(glm::vec3(0, 0, 1));

		texCoords.push_back((float(x) + 1.0) * 0.5);
		texCoords.push_back((float(y) + 1.0) * 0.5);
		texCoords.push_back(0.0f);



	}
	texCoords.push_back((1.0 + 1.0) * 0.5);
	texCoords.push_back((0.0 + 1.0) * 0.5);
	texCoords.push_back(0.0f);
	cylinderBottomPositions.push_back(glm::vec3(0, 0, 0));    // Center vertex for top of cylinder.
	texCoords.push_back(0.5);
	texCoords.push_back(0.5);
	texCoords.push_back(0.0f);
	for (int i = 0; i <= 32; i++) {  // Vertices around the top.
		double angle = (2 * pi / 32) * i;
		double x = cos(angle);
		double y = sin(angle);
		cylinderBottomPositions.push_back(glm::vec3(float(x) * 0.5, float(y) * 0.5, 0.0f));
		cylinderBottomNormals.push_back(glm::vec3(0, 0, -1));
		texCoords.push_back((float(x) + 1.0) * 0.5);
		texCoords.push_back((float(y) + 1.0) * 0.5);
		texCoords.push_back(0.0f);
	}
	texCoords.push_back((1.0 + 1.0) * 0.5);
	texCoords.push_back((0.0 + 1.0) * 0.5);
	texCoords.push_back(0.0f);




	glGenBuffers(1, &cylinderStripBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderStripBufferObject);
	glBufferData(GL_ARRAY_BUFFER, cylinderStripPositions.size() * sizeof(glm::vec3), &cylinderStripPositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &cylinderStripNormalObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderStripNormalObject);
	glBufferData(GL_ARRAY_BUFFER, cylinderStripNormals.size() * sizeof(glm::vec3), &cylinderStripNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cylinderStripTextureObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderStripTextureObject);
	glBufferData(GL_ARRAY_BUFFER, stripTexCoords.size() * sizeof(glm::vec3), &stripTexCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cylinderTopBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopBufferObject);
	glBufferData(GL_ARRAY_BUFFER, cylinderTopPositions.size() * sizeof(glm::vec3), &cylinderTopPositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glGenBuffers(1, &cylinderTopNormalObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopNormalObject);
	glBufferData(GL_ARRAY_BUFFER, cylinderTopNormals.size() * sizeof(glm::vec3), &cylinderTopNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cylinderTopTextureObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopTextureObject);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec3), &texCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cylinderBottomBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomBufferObject);
	glBufferData(GL_ARRAY_BUFFER, cylinderBottomPositions.size() * sizeof(glm::vec3), &cylinderBottomPositions[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glGenBuffers(1, &cylinderBottomNormalObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomNormalObject);
	glBufferData(GL_ARRAY_BUFFER, cylinderBottomNormals.size() * sizeof(glm::vec3), &cylinderBottomNormals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &cylinderBottomTextureObject);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomTextureObject);
	glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(glm::vec3), &texCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void points::drawBranch(int rotate, int color, GLuint texID, GLuint transID, GLuint colorID)
{
	lsystem_transform.push(lsystem_transform.top());
	lsystem_transform.top() = glm::rotate(lsystem_transform.top(), 90.f, glm::vec3(1, 0, 0));
	if (rotate == 1)
	{
		lsystem_transform.top() = glm::rotate(lsystem_transform.top(), -45.f, glm::vec3(0, 1, 0));
	}
	else if (rotate == 2)
	{
		lsystem_transform.top() = glm::rotate(lsystem_transform.top(), 45.f, glm::vec3(0, 1, 0));
	}
	else if (rotate == 3)
	{
		lsystem_transform.top() = glm::rotate(lsystem_transform.top(), 45.f, glm::vec3(1, 0, 0));
	}
	else if (rotate == 4)
	{
		lsystem_transform.top() = glm::rotate(lsystem_transform.top(), -45.f, glm::vec3(1, 0, 0));
	}
	glUniformMatrix4fv(transID, 1, GL_FALSE, &(lsystem_transform.top()[0][0]));
	glUniform1ui(colorID, color);
	//	transform = transform * model;
	//colourmode = color;
	glBindBuffer(GL_ARRAY_BUFFER, cylinderStripBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cylinderStripTextureObject);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindTexture(GL_TEXTURE_2D, texID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, cylinderStripPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cylinderTopTextureObject);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindTexture(GL_TEXTURE_2D, texID);
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinderTopPositions.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, cylinderBottomTextureObject);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindTexture(GL_TEXTURE_2D, texID);
	glDrawArrays(GL_TRIANGLE_FAN, 0, cylinderBottomPositions.size());


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(2);
	lsystem_transform.pop();


}



