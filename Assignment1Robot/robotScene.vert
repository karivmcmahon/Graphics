// Vertex shader by Kari McMahon in October 2014
// Based on Iain Martin's positional lighting lab

#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;


// These are the uniforms that are defined in the application
uniform mat4 model, view, projection;
uniform float colourmode;
uniform uint emitmode;

out vec4 fcolour;
out vec4 diffuse_albedo;
out vec4 P;
out vec3 normals;
out mat3 normalmatrix;
out vec3 emissive;


void main()
{
	normals = normal; //Normals to send to fragment shader
	vec4 position_h = vec4(position, 1.0);	
	mat4 mv_matrix = view * model;		
	P = mv_matrix * position_h;	
    normalmatrix = mat3(transpose(inverse(mv_matrix))); //Need to do this as scaling objects

	//Setting colours for application based on uniform
	if (colourmode == 2)
	{
		diffuse_albedo = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else if (colourmode == 1)
	{
		diffuse_albedo = vec4(0.0, 0.0, 0.0, 1.0);
	}
	else if (colourmode == 3)
	{
		diffuse_albedo = vec4(0.0, 0.0, 1.0, 1.0);
	}
	else
	{
		diffuse_albedo = vec4(0.5, 0.5, 0.5, 1.0);
	}
	
	//Emissive colour
	if (emitmode == 1) emissive = vec3(1.0, 1.0, 0.8); 

	fcolour = colour;
	gl_Position =  (projection * view * model)  * position_h;
	
	
}