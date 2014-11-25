#include "Skybox.h"
#include "Soil.h"
Skybox::Skybox()
{
	
}

Skybox::~Skybox()
{
	
	
}

void Skybox::createSkybox()
{
	static const GLfloat quad_data[] =
	{
		// Vertex positions
		50.0f, 50.0f, 50.0f,
		50.0f, -10.0f, 50.0f,
		-50.0f, 50.0f, 50.0f,
		-50.0f, -10.0f, 50.0f,

		-50.0f, 50.0f, -50.0f,
		-50.0f, -10.0f, -50.0f,
		50.0f, 50.0f, -50.0f,
		50.0f, -10.0f, -50.0f,

		-50.0f, 50.0f, 50.0f,
		-50.0f, -10.0f, 50.0f,
		-50.0f, 50.0f, -50.0f,
		-50.0f, -10.0f, -50.0f,

		50.0f, 50.0, -50.0f,
		50.0f, -10.0, -50.0f,
		50.0f, 50.0, 50.0f,
		50.0f, -10.0, 50.0f,

		-50.0f, 50.0, -50.0f,
		50.0f, 50.0, -50.0f,
		-50.0f, 50.0, 50.0f,
		50.0f, 50.0, 50.0f,

		50.0f, -10.0, -50.0f,
		-50.0f, -10.0, -50.0f,
		50.0f, -10.0, 50.0f,
		-50.0f, -10.0, 50.0f,

	};
	static const GLfloat normals[]
	{
		// Normals
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
		0, 1.f, 0,
	};
	static const GLfloat textureCoords[]
	{
		0.0f, 1.0f, 0,
		0.0f, 0.0f, 0,
		1.0f, 1.0f, 0,
		1.0f, 0.0f, 0,
	};

	glGenBuffers(1, &quad_vbo_front);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_front);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(glm::vec3), &quad_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quad_normals);
	glBindBuffer(GL_ARRAY_BUFFER, quad_normals);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals) * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quad_textures);
	glBindBuffer(GL_ARRAY_BUFFER, quad_textures);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords) * sizeof(glm::vec3), &textureCoords[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quad_vbo_back);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_back);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(glm::vec3), &quad_data[12], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quad_vbo_left);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_left);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(glm::vec3), &quad_data[24], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quad_vbo_right);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_right);
	glBufferData(GL_ARRAY_BUFFER, 36 * sizeof(glm::vec3), &quad_data[36], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quad_vbo_top);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_top);
	glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(glm::vec3), &quad_data[48], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &quad_vbo_bottom);
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_bottom);
	glBufferData(GL_ARRAY_BUFFER, 60 * sizeof(glm::vec3), &quad_data[60], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
}

void Skybox::renderSkybox(GLuint textureFront, GLuint textureBack, GLuint textureLeft, GLuint textureRight, GLuint textureTop, GLuint textureBottom)
{
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_front);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, quad_normals);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, quad_textures);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Front
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureFront);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	
	//Back
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_back);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureBack);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);

	//Left
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_left);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureLeft);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);

	//Right
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_right);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureRight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);

	//Top
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_top);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureTop);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);

	//Bottom under terrain
	glBindBuffer(GL_ARRAY_BUFFER, quad_vbo_bottom);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureBottom);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}
