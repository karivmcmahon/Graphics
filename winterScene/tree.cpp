#include "tree.h"
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>


tree::tree()
{
	pi = 3.1415926535898;
	twicePi = 2.0f * pi;
	strcpy(lstring, "F[C][A][D][G]F\0");
}

tree::~tree()
{

}

void tree::createTree()
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
		cylinderStripColours.push_back(0.0f);
		cylinderStripColours.push_back(0.0f);
		cylinderStripColours.push_back(1.0f);
		cylinderStripNormals.push_back(glm::vec3(float(x), float(y), 0));
		stripTexCoords.push_back(tc);
		stripTexCoords.push_back(1);
		stripTexCoords.push_back(0);
		cylinderStripPositions.push_back(glm::vec3(float(x) * 0.5, float(y) * 0.5, 0.0f));
		cylinderStripColours.push_back(0.0f);
		cylinderStripColours.push_back(0.0f);
		cylinderStripColours.push_back(1.0f);
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
		cylinderTopColours.push_back(0.0f);
		cylinderTopColours.push_back(0.0f);
		cylinderTopColours.push_back(1.0f);
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
		cylinderBottomColours.push_back(0.0f);
		cylinderBottomColours.push_back(0.0f);
		cylinderBottomColours.push_back(1.0f);
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

void tree::drawBranch(int rotate, int color, GLuint texID, GLuint transID, GLuint colorID)
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
void tree::setUp()
{
	//lsystem_transform.push(glm::mat4(1.0f));
	//lsystem_transform.top() = glm::translate(lsystem_transform.top(), glm::vec3(-3, -4, 0));
}
void tree::trees(int level, GLuint texID, GLuint transID, GLuint colorID) {
	int current;
	current = 0;
	while (lstring[current])
	{

		switch (lstring[current])
		{
		case 'F':
		case 'A':
		case 'G':
		case 'D':
		case 'C':
			if (level == 0)
			{
				if (lstring[current] == 'F')
				{
					drawBranch(0, 1, texID, transID,colorID);
					lsystem_transform.top() = glm::translate(lsystem_transform.top(), glm::vec3(0, 0.1f, 0));
				}
				else if (lstring[current] == 'A')
				{
					drawBranch(1, 0, texID,transID,colorID);
				}
				else if (lstring[current] == 'G')
				{
					drawBranch(2, 0, texID, transID,colorID);
				}
				else if (lstring[current] == 'C')
				{
					drawBranch(3, 0, texID,transID,colorID);
				}
				else if (lstring[current] == 'D')
				{
					drawBranch(4, 0, texID,transID,colorID);
				}
			}
			else
			{

				trees(level - 1, texID, transID, colorID);

			}

			break;
		case '[':
			lsystem_transform.push(glm::scale(lsystem_transform.top(), glm::vec3(0.8, 1, 1)));
			break;
		case ']':
			lsystem_transform.pop();
			break;
		default:
			break;
		}
		current++;

	}
}