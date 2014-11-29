// Veretx shader: When completed should implement
// per vertex lighting

// Specify minimum OpenGL version
#version 400

// Define the vertex attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texture;

// This is the output vertex colour sent to the rasterizer
out vec4 fcolour;
out vec4 diffuse_albedo;

out vec4 P;
out vec3 normals;
out vec2 ftexcoord;
out float distanceToLight;
out vec3 emissive;

// These are the uniforms that are defined in the application
uniform mat4 model, view, projection, texMatrix;
uniform uint colourmode, emitmode;


// Global constants (for this vertex shader)
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
vec3 global_ambient = vec3(0.05, 0.05, 0.05);
int  shininess = 8;

void main()
{
	normals = normal;
	vec4 position_h = vec4(position, 1.0);	// Convert the (x,y,z) position to homogeneous coords (x,y,z,w)
	mat4 mv_matrix = view * model;		// Calculate the model-view transformation
	P = mv_matrix * position_h;	// Modify the vertex position (x, y, z, w) by the model-view transformation
	
	if (colourmode == 0)
	{
		diffuse_albedo = vec4(0.7, 0.7, 0.7, 1.0);
		fcolour = vec4(1.0,1.0,1.0,1.0);
	}
	else
	{
		diffuse_albedo = vec4(0.7, 0.7, 0.7, 1.0);
		fcolour = vec4(1.0,1.0,1.0,1.0);
	}
	if (emitmode == 1) emissive = vec3(1.0, 1.0, 0.8); 
	
	gl_Position = (projection * view * model) * position_h;
	vec4 tc4 = texMatrix * vec4(texture.xy, 0, 1.0);
	ftexcoord = tc4.xy;
}