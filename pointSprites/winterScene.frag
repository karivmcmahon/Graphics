// Minimal fragment shader

#version 400

in vec4 fcolour;
in vec4 diffuse_albedo;
in vec3 emissive;
in vec4 P;
in vec3 normals;
in vec2 ftexcoord;
in float fdistance;



uniform vec4 lightpos;
uniform mat3 normalmatrix;
uniform sampler2D tex1;
vec3 specular_albedo = vec3(1.0, 0.8, 0.6);
vec3 global_ambient = vec3(0.05, 0.05, 0.05);
int  shininess = 8;

out vec4 outputColor;

void main()
{
vec3 N;
vec3 L;
vec3 V;
vec3 R;
	vec4 texcolour = texture(tex1, ftexcoord) * fcolour;
	N = normalize(normalmatrix * normals);		// Modify the normals by the normal-matrix (i.e. to model-view (or eye) coordinates )
    L = lightpos.xyz - P.xyz;		// Calculate the vector from the light position to the vertex in eye space
	float distanceToLight = length(L);
	L = normalize(L);
	vec3 ambient = diffuse_albedo.xyz * 1.0;
	vec3 diffuse = max(dot(N, L), 0.0) * diffuse_albedo.xyz;
	V = normalize(-P.xyz);	
	R = reflect(-L, N);
	vec3 specular = pow(max(dot(R, V), 0.0), shininess) * specular_albedo;
	float attenuation_k = 2.0;
   float attenuation = 1.0 / (1.0 + attenuation_k * pow(distanceToLight, 2)); 
  
	outputColor = vec4((ambient + specular + diffuse) + global_ambient + emissive, 1.0) * texcolour;
}