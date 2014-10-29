// Vertex shader with a model transformation uniform

#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
out vec4 fcolour;
uniform mat4 model, view;
uniform uint colourmode;

void main()
{

	vec3 lightDirection = vec3(2,1.0,2.0);
	mat4 modelViewMatrix = view * model;
	vec4 position4 = vec4(position, 1.0);
    mat3 normalMatrix = mat3(modelViewMatrix);
	vec3 transNormal = vec3(normalMatrix * normal);
	lightDirection = normalize(lightDirection);
	transNormal = normalize(transNormal);
	float diffuseComponent = max(dot(transNormal,lightDirection),0.0);
	vec4 diffuse_colour;
	if(colourmode == 2)
	{
		diffuse_colour = vec4(1.0, 0.0, 0.0, 1.0);
	}
	if (colourmode == 1)
	{
		diffuse_colour = vec4(1.0, 0.0, 0.0, 1.0);
	}
	else
	{
		diffuse_colour = vec4(0.6, 0.6, 0.6, 1.0);
	}
	
	vec4 diffuseLighting = vec4( diffuse_colour * diffuseComponent);
	vec4 ambient = diffuse_colour * 0.3;
	
	vec4 transPosition = position4 * modelViewMatrix;
	float shininess = 8.0;
	vec3 v = normalize(-transPosition.xyz);
	vec3 r = reflect(-lightDirection,transNormal);
	vec3 specular = pow(max(dot(r,v),0.0), shininess) * vec3(1.0,1.0,1.0);
	
	fcolour = ambient + diffuseLighting + specular;
	
	gl_Position =   view * model  * position4;
	
	
}