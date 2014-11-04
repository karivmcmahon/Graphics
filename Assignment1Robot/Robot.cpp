/**
Robot class stores the creation of the robot
Created by Kari McMahon October 2014
**/
#pragma once
#include <string>
#include <iostream>
#include <vector>

/* Static Libs */
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "glloadD.lib")
#pragma comment(lib, "opengl32.lib")

#include "wrapper_glfw.h"
#include "robotScene.h"

/* Inlcude GL_Load and GLFW */
#include <glload/gl_4_0.h>
#include <glload/gl_load.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <stack>

class Robot
{
public:
	//Uniforms
    GLuint modelID,  colourModeID, emitModeID;	
	//Sets up various movement variables for robot
	GLfloat neckmovement, robotRotation, elbowBasedMovement, armMoving, armUpDownMovement, legmovement, fingerMovement, fingerPosition, kneeMovement;
	GLfloat coneRotation, boltRotation;
	Robot()
	{
		coneRotation = 90;
		boltRotation = 90;
		neckmovement = 0;
		robotRotation = 0;
		fingerMovement = 0;
		elbowBasedMovement = 0;
		legmovement = 0;
		armUpDownMovement = 0;
		armMoving = 0;
		fingerPosition = 45;
	}

	/**
	* Draws the robot
	**/
	void drawRobot()
	{
		model.push(model.top());
			model.top() = glm::rotate(model.top(), -neckmovement, glm::vec3(0, 1, 0));
			model.top() = glm::translate(model.top(), glm::vec3(0, 0, 0));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			drawHat();
			drawHead();
			drawNeck();
		model.pop();
		drawBody();
		drawArm(0.20, 1);
		drawArm(-0.20, 0);
		drawLeg(0.1, 1);
		drawLeg(-0.1, 0);
	}

	/**
	Draws robots cone hat
	**/
	void drawHat()
	{
		model.push(glm::mat4(1.0f));
			model.top() = glm::translate(model.top(), glm::vec3(0, 0.7, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.2, 0.1));
			model.top() = glm::rotate(model.top(), -neckmovement, glm::vec3(0, 1, 0));
			model.top() = glm::rotate(model.top(), -robotRotation, glm::vec3(0, 1, 0));
			model.top() = glm::rotate(model.top(), -(coneRotation), glm::vec3(1, 0, 0));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 2);
			glUniform1ui(emitModeID, 0);
			shape.drawCone();
		model.pop();

	}
	
	/**
	Draws the robots head
	**/
	void drawHead()
	{
		model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(0, 0.50, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.8, 0.8, 0.3));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 0);
			shape.drawCube();
		model.pop();
		drawEye(0.1);
		drawEye(-0.1);
		drawLips();
		drawBoltEars(0.22);
		drawBoltEars(-0.22);
	}

	/**
	Draws robots neck
	**/
	void drawNeck()
	{
		model.push(glm::mat4(model.top()));
			model.top() = glm::translate(model.top(), glm::vec3(0, 0.24, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.25, 0.25, 0.3));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 0);
			shape.drawCube();
		model.pop();
	}

	/**
	Draws thr robot's eye
	**/
	void drawEye(GLfloat x)
	{
		model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(x, 0.55, -0.05));
			model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.3));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 1);
			shape.drawCube();
		model.pop();
	}

	/**
	Draws the robots lips
	**/
	void drawLips()
	{
		model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(0, 0.40, -0.05));
			model.top() = glm::scale(model.top(), glm::vec3(0.25, 0.05, 0.2));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 2);
			shape.drawCube();
		model.pop();
	}

	/**
	Draws the robots bolt ears
	**/
	void drawBoltEars(GLfloat x)
	{
		model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(x, 0.5, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.05, 0.1));
			model.top() = glm::rotate(model.top(), boltRotation, glm::vec3(0, 1, 0));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 1);
			shape.drawBolt();
		model.pop();
	}

	/**
	Draws  robots body
	**/
	void drawBody()
	{
		model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(0.0, -0.07, 0));
			model.top() = glm::scale(model.top(), glm::vec3(0.6, 1, 0.3));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 0);
			shape.drawCube();
		model.pop();
		drawBoltButton(-0.07);
		drawBoltButton(0.05);
		drawBoltButton(-0.20);
	}

	/**
	Draws robots buttons
	**/
	void drawBoltButton(GLfloat y)
	{
		model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(0, y, -0.1));
			model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.05, 0.05));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
			glUniform1f(colourModeID, 1);
			shape.drawBolt();
		model.pop();
	}

	/**
	Creates and draws robots arm
	**/
	void drawArm(GLfloat x, GLfloat side)
	{

		//Transform for arm
		model.push(model.top());
		model.top() = glm::translate(model.top(), glm::vec3(x, 0.10, 0));
		glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		glUniform1f(colourModeID, 0);

			//SHOULDER SOCKET
			model.push(model.top());
				model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
				shape.drawSphere();
			model.pop();

			//SHOULDER
			model.push(model.top());
				if (side == 0)
				{
					model.top() = glm::rotate(model.top(), -armMoving, glm::vec3(1, 0, 0));
					model.top() = glm::rotate(model.top(), -armUpDownMovement, glm::vec3(0, 0, 1));
				}
				else
				{
					model.top() = glm::rotate(model.top(), armMoving, glm::vec3(1, 0, 0));
					model.top() = glm::rotate(model.top(), armUpDownMovement, glm::vec3(0, 0, 1));
				}
				model.top() = glm::translate(model.top(), glm::vec3(0, -0.12, 0));
				model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
				shape.drawCube();
			model.pop();


			//TRANSFORM FOR ARM
			model.push(model.top());
				if (side == 0)
				{
					model.top() = glm::rotate(model.top(), -armMoving, glm::vec3(1, 0, 0));
					model.top() = glm::rotate(model.top(), -armUpDownMovement, glm::vec3(0, 0, 1));
				}
				else
				{
					model.top() = glm::rotate(model.top(), armMoving, glm::vec3(1, 0, 0));
					model.top() = glm::rotate(model.top(), armUpDownMovement, glm::vec3(0, 0, 1));
				}
				model.top() = glm::translate(model.top(), glm::vec3(0, -0.27, 0)); //rotating in clockwise direction around x-axis
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);

				//ELBOW SPHERE
				model.push(model.top());
					model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					shape.drawSphere();
				model.pop();

				//ARM
				model.push(model.top());
					model.top() = glm::rotate(model.top(), elbowBasedMovement, glm::vec3(1, 0, 0));
					model.top() = glm::translate(model.top(), glm::vec3(0, -0.10, 0));
					model.top() = glm::scale(model.top(), glm::vec3(0.15, 0.4, 0.3));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					shape.drawCube();
				model.pop();


				//TRANSFORM FOR HAND
				model.push(model.top());
					model.top() = glm::rotate(model.top(), elbowBasedMovement, glm::vec3(1, 0, 0));
					model.top() = glm::translate(model.top(), glm::vec3(0, -0.20, 0));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);


					//// HAND SPHERE
					model.push(model.top());
						model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
						glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
						shape.drawSphere();
					model.pop();

					//FINGER
					model.push(model.top());
						if (side == 0)
						{
							model.top() = glm::rotate(model.top(), -fingerMovement, glm::vec3(0, 0, 1));
						}
						else
						{
							model.top() = glm::rotate(model.top(), fingerMovement, glm::vec3(0, 0, 1));
						}
						model.top() = glm::translate(model.top(), glm::vec3(0, -0.09, 0));
						model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.15, 0.1));
						glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
						shape.drawCube();
					model.pop();

					//FINGER 2
					model.push(model.top());
						if (side == 0)
						{
							model.top() = glm::rotate(model.top(), fingerMovement, glm::vec3(0, 0, 1));
							model.top() = glm::rotate(model.top(), -fingerPosition, glm::vec3(0, 0, 1));
						}
						else
						{
							model.top() = glm::rotate(model.top(), -fingerMovement, glm::vec3(0, 0, 1));
							model.top() = glm::rotate(model.top(), fingerPosition, glm::vec3(0, 0, 1));
						}
						model.top() = glm::translate(model.top(), glm::vec3(0.01, -0.09, 0));
						model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.15, 0.1));
						glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
						shape.drawCube();
					model.pop();
				model.pop();
			model.pop();
		model.pop();
	}

	/**
	Creates and draws leg for robot
	**/
	void drawLeg(GLfloat x, GLfloat side)
	{
		//Transform for leg
		model.push(model.top());
			model.top() = glm::translate(model.top(), glm::vec3(x, -0.37, 0));
			glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
		
			//LEG CONNECTOR
			model.push(model.top());
				model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
				shape.drawSphere();
			model.pop();

			//Transformation for leg
			model.push(model.top());
				if (side == 0)
				{
					model.top() = glm::rotate(model.top(), legmovement, glm::vec3(1, 0, 0));
				}
				else
				{
					model.top() = glm::rotate(model.top(), -legmovement, glm::vec3(1, 0, 0));
				}
				model.top() = glm::translate(model.top(), glm::vec3(0, -0.15, 0));
				glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);

				// THIGH
				model.push(model.top());
					model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.5, 0.3));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					shape.drawCube();
				model.pop();

				//Transform for knee downwards 
				model.push(model.top());
					model.top() = glm::translate(model.top(), glm::vec3(0, -0.17, 0));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);

				// KNEE CAP
				model.push(model.top());
					model.top() = glm::scale(model.top(), glm::vec3(0.05, 0.06, 0.06));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					shape.drawSphere();
				model.pop();

				//LEG
				model.push(model.top());
					model.top() = glm::rotate(model.top(), -kneeMovement, glm::vec3(1, 0, 0));
					model.top() = glm::translate(model.top(), glm::vec3(0, -0.17, 0));
					model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.5, 0.3));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					shape.drawCube();
				model.pop();

				////SHOE
				model.push(model.top());
					model.top() = glm::rotate(model.top(), -kneeMovement, glm::vec3(1, 0, 0));
					model.top() = glm::translate(model.top(), glm::vec3(0, -0.27, -0.05));
					model.top() = glm::scale(model.top(), glm::vec3(0.2, 0.2, 0.5));
					glUniformMatrix4fv(modelID, 1, GL_FALSE, &(model.top())[0][0]);
					shape.drawCube();
				model.pop();
			model.pop();
		model.pop();
	model.pop();
	}

	/**
	Key pressed to move the robot in various ways
	**/
	void robotKeyMoves(int k)
	{
		if (k == 'Q')	robotRotation -= 10.0f;
		if (k == 'W')	robotRotation += 10.0f;

		if (k == 'R')	neckmovement -= 5;
		if (k == 'E')	neckmovement += 5;

		if (k == 'A')	armUpDownMovement += 5 % 360;
		if (k == 'S')	armUpDownMovement -= 5 % 360;
		if (k == 'Z')	elbowBasedMovement += 5;
		if (k == 'X')	elbowBasedMovement -= 5;
		if (k == 'B')	legmovement += 5;
		if (k == 'N')	legmovement -= 5;
		if (k == 'I')	kneeMovement += 5;
		if (k == 'O')	kneeMovement -= 5;
		if (k == 'F')	fingerMovement += 5;
		if (k == 'G')	fingerMovement -= 5;
		if (k == 'Y')	armMoving += 5;
		if (k == 'U')	armMoving -= 5;
	}

	/**
	Movement constraints to make movement seem realistic to a robot
	**/
	void robotMovementConstraints()
	{
		if (legmovement >= 45)	legmovement = 45;
		if (legmovement <= -45)	legmovement = -45;
		if (kneeMovement >= 45)	kneeMovement = 45;
		if (kneeMovement <= -45) kneeMovement = -45;
		if (neckmovement >= 90)	 neckmovement = 90;
		if (neckmovement <= -90) neckmovement = -90;
		if (fingerMovement >= 20) fingerMovement = 20;
		if (fingerMovement <= 0) fingerMovement = 0;
		if (armUpDownMovement >= 170) armUpDownMovement = 170;
		if (armUpDownMovement <= 0)	armUpDownMovement = 0;
		if (elbowBasedMovement <= 0) elbowBasedMovement = 0;
		if (elbowBasedMovement >= 120) elbowBasedMovement = 120;
		if (armMoving <= -60) armMoving = -60;
		if (armMoving >= 60) armMoving = 60;
	}

	


};



