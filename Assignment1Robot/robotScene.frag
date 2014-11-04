// Fragment shader by Kari McMahon in October 2014
// Based on Iain Martin's positional lighting lab

#version 400

in vec4 fcolour;
in vec4 diffuse_albedo;
in vec4 P;
in vec3 normals;
in mat3 normalmatrix;
in vec3 emissive;
uniform vec4 lightpos;


vec3 specular_albedo = vec3(1.0, 1.0, 1.0);

vec3 global_ambient = vec3(0.05, 0.05, 0.05);
float  shininess = 8.0;

out vec4 outputColor;

void main()
{

	vec3 N;
	vec3 L;
	vec3 V;
	vec3 R;
	N = normalize(normalmatrix * normals);		// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
    L = lightpos.xyz - P.xyz;		// Calculate the vector from the light position to the vertex in eye space
	L = normalize(L);
	vec3 ambient = diffuse_albedo.xyz * 0.2;
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo.xyz;
	V = normalize(-P.xyz);	
	R = reflect(-L, N);
	vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;
	outputColor = vec4((ambient + diffuse + specular ) + emissive + global_ambient, 1.0);
}