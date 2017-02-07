#version 430 // for gspecularLightIntensityl version (12 is for odiffuseLightIntensityer versions , say opengl 2.1

uniform vec3 eyePosition;

struct LightInfo
{
	vec3 position;
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};
uniform LightInfo light;


struct MaterialInfo
{
	sampler2D diffuseMap;
	sampler2D specularMap;
	float shininess;
};
uniform MaterialInfo material;

in FragData
{
	vec3 position;		//in world space
	vec3 normal;		//in world space 
	vec2 uv;			
} fragData;

out vec4 fragColor;

void main()
{
		//ambient calculations
	vec3 ambientTerm = light.ambientColor * vec3(texture(material.diffuseMap, fragData.uv)); //ambient color is the same as the material diffuse
	
		//diffuse calculations
	vec3 norm = normalize(fragData.normal);
	vec3 lightDirection = normalize(light.position - fragData.position); //direction from the pixel to the light source
	float diffuseIntensity = max(dot(norm,lightDirection) , 0.0f);
	vec3 diffuseTerm = diffuseIntensity * light.diffuseColor * vec3(texture(material.diffuseMap, fragData.uv));
	
		//specular calculations
	vec3 eyeDirection = normalize(eyePosition - fragData.position);
	vec3 reflectDirection = reflect(-lightDirection, norm);
	float specularIntensity = pow(max(dot(eyeDirection, reflectDirection), 0.0f), material.shininess);
	vec3 specularTerm = specularIntensity * light.specularColor * vec3(texture(material.specularMap, fragData.uv));
		
		//final color
	vec3 result = ambientTerm + diffuseTerm + specularTerm;
	fragColor = vec4(result, 1.0f);
}
