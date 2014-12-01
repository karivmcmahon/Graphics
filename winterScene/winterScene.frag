/// Fragment shader for winter scene. Originally by Iain Martin. Modified by Kari McMahon Nov 2014

#version 400

in vec4 positionMV;
in vec3 normals;
in vec2 ftexcoord;
in float fdistance;

uniform vec4 lightpos;
uniform mat3 normalmatrix;
uniform sampler2D tex1;
uniform uint colourmode, emitmode;

vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
int  shininess = 8;

out vec4 outputColor;

void main()
{
	vec3 N, L, V, R;
	vec4 diffuse_albedo, fcolour;

	//Colours based on mode
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
	
	
	vec4 texcolour = texture(tex1, ftexcoord) * fcolour; //Get texture and merge with colour mode
	N = normalize(normalmatrix * normals);		// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
    L = lightpos.xyz - positionMV.xyz;		// Calculate the vector from the light position to the vertex in eye space
	
	float distanceToLight = length(L);	//Distance to light
	L = normalize(L);
	
	vec3 ambient = diffuse_albedo.xyz * 1.0; // Ambient calc
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo.xyz; //Diffuse calc
	V = normalize(-positionMV.xyz);	
	R = reflect(-L, N);
	
	vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;
	
	//Attenuation calc
	float attenuation_k = 2.0;
	float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2)); 
  
	outputColor = vec4((ambient + specular + diffuse), 1.0) * texcolour;
}