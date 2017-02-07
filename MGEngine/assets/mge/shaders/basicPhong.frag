#version 430 // for gspecularLightIntensityl version (12 is for odiffuseLightIntensityer versions , say opengl 2.1

vec3 lightColor = vec3(1,1,1);
vec3 objectColor = vec3(0.1f,0.5f,0.9f);

uniform vec3 lightPosition;
uniform vec3 eyePosition;

in FragData
{
	vec3 position;		//in world space
	vec3 normal;		//in world space (possible interpolation issues?)
	vec2 uv;			
} fragData;

out vec4 fragColor;

void main()
{
		//ambient calculations
	float ambientIntensity = 0.2f;	//basically limit the darkest part of an object
	vec3 ambientTerm = ambientIntensity * lightColor;
	
		//diffuse calculations
	vec3 norm = normalize(fragData.normal);
	vec3 lightDirection = normalize(lightPosition - fragData.position); //direction from the pixel to the light source
	float diffuseIntensity = max(dot(norm,lightDirection) , 0.0f);
	vec3 diffuseTerm = diffuseIntensity * lightColor;
	
		//specular calculations
	float specularIntensity = 1.0f;
	int shininess = 32;
	vec3 eyeDirection = normalize(eyePosition - fragData.position);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	specularIntensity *= pow(max(dot(eyeDirection, reflectDirection), 0.0f), shininess);
	vec3 specularTerm = specularIntensity * lightColor;
		
		//final color
	vec3 result = (ambientTerm + diffuseTerm + specularTerm) * objectColor;
	fragColor = vec4(result, 1.0f);
}
