// Vertex shader for winter scene. Originally by Iain Martin
// Modified by Kari McMahon - Nov 2014

// Specify minimum OpenGL version
#version 400

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texture;

// This is the outputs sent to the rasterizer
out vec4 positionMV;
out vec3 normals;
out vec2 ftexcoord;

// These are the uniforms that are defined in the application
uniform mat4 model, view, projection, texMatrix;

void main()
{
	normals = normal;
	vec4 position_h = vec4(position, 1.0);	// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)
	mat4 mv_matrix = view * model;		// Calculate the model-view transformation
	positionMV = mv_matrix * position_h;	// Modify the vertex position (x, y, z, w) by the model-view transformation	
	gl_Position = (projection * view * model) * position_h; //Set position
	vec4 tc4 = texMatrix * vec4(texture.xy, 0, 1.0); //Modify texture by texture matrix
	ftexcoord = tc4.xy; //Get texture matrix x and y coords
}