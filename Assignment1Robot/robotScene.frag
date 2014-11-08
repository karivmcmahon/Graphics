// Fragment shader by Kari McMahon in October 2014
// Based on Iain Martin's phong lighting lab

#version 400

in vec4 fcolour;
in vec4 diffuse_albedo;
in vec4 P;
in vec3 normals;
in mat3 normalmatrix;
in vec3 emissive;
uniform vec4 lightpos;

//globals
vec3 specular_albedo = vec3(1.0, 1.0, 1.0);
vec3 global_ambient = vec3(0.05, 0.05, 0.05);
float  shininess = 8.0;

out vec4 outputColor;

void main()
{

	vec3 N = normalize(normalmatrix * normals);		
    vec3 L = lightpos.xyz - P.xyz;	
	float distancetolight = length(L);	
	L = normalize(L);
	vec3 D = normalize(vec3(0,0,1)); //spotlight direction
	vec3 ambient = diffuse_albedo.xyz * 0.2;
				
	float val = max( dot(N,L), 0.0);
	if(val > 0)
	{
			vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo.xyz;
			vec3 V = normalize(-P.xyz);	
			vec3 R = reflect(-L, N);
			vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;
			float attenuation_k = 2.0;
			float attenuation = 1.0 / (1.0 + attenuation_k * pow(distancetolight, 2));
			attenuation += 0.8;
			outputColor = vec4(attenuation*(ambient + diffuse + specular ) + emissive + global_ambient, 1.0); 
	}
	else
	{
		outputColor = vec4(ambient, 1.0);
	}
	
}