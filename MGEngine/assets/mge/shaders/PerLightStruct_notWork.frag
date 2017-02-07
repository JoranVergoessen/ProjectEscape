#version 430 // for gspecularLightIntensityl version (12 is for odiffuseLightIntensityer versions , say opengl 2.1

uniform vec3 eyePosition;

struct DirLight
{
	vec3 direction;
	vec3 ambientC;
	vec3 diffuseC;
	vec3 specularC;
};
uniform DirLight dirLight;
vec3 CalculateDirLight(DirLight pLight, vec3 pNormal, vec3 pEyeDir);

struct PointLight
{
	vec3 position;
	float constAtten;
	float linearAtten;
	float quadAtten;
	vec3 ambientC;
	vec3 diffuseC;
	vec3 specularC;
};
#define MAX_POINT 4
uniform PointLight pointLights[MAX_POINT];
vec3 CalculatePointLight(PointLight pLight, vec3 pNormal, vec3 pFragPos, vec3 pEyeDir);

struct SpotLight
{
	vec3 position;
	vec3 spotDirection;
	float cutoff;
	float farCutoff;
	float constAtten;
	float linearAtten;
	float quadAtten;
	vec3 ambientC;
	vec3 diffuseC;
	vec3 specularC;
};
uniform SpotLight spotLight;
vec3 CalculateSpotLight(SpotLight pLight, vec3 pNormal, vec3 pFragPos, vec3 pEyeDir);

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

vec3 matColor = vec3(1,1,1);

vec3 DirectionalLight()
{
	//vec3 ambTerm = dirLight.ambientC * vec3(texture(material.diffuseMap, fragData.uv));
	vec3 ambTerm = dirLight.ambientC;
	
	vec3 norm = normalize(fragData.normal);
	float diffIntensity = max(dot(norm,dirLight.direction), 0.0);
	vec3 diffTerm = diffIntensity * dirLight.diffuseC;
	
	vec3 eyeDir = normalize(eyePosition - fragData.position);
	vec3 reflectDir = reflect(-dirLight.direction, norm);
	float specIntensity = pow(max(dot(eyeDir, reflectDir), 0.0), material.shininess);
	vec3 specTerm = specIntensity * dirLight.specularC;
	
	vec3 result = (ambTerm + diffTerm + specTerm) * matColor;
	return result;
}


void main()
{
	vec3 norm = normalize(fragData.normal);//fragment based normals
	vec3 eyeDirection = normalize(eyePosition - fragData.position);
	//directional light computation
	//vec3 finalColor = CalculateDirLight(dirLight, norm, eyeDirection);
	vec3 finalColor = DirectionalLight();
	//point lights computation
	/**
	for(int i = 0; i < MAX_POINT; i ++)
	{
		finalColor += CalculatePointLight(pointLights[i], norm, fragData.position, eyeDirection);
	}
	//spot liht computation
	finalColor += CalculateSpotLight(spotLight, norm, fragData.position, eyeDirection);
	/**/
	fragColor = vec4(finalColor, 1.0f);
}

vec3 CalculateDirLight(DirLight pLight, vec3 pNormal, vec3 pEyeDir)
{
	vec3 lightDir = normalize(-pLight.direction);
	//diffuse
	float diffuseIntensity = max(dot(pNormal, lightDir),0.0f);
	//specular
	vec3 reflectDir = reflect(-lightDir, pNormal);
	float specularIntensity = pow(max(dot(pEyeDir, reflectDir),0.0f), material.shininess);
	//result
	vec3 ambientTerm = pLight.ambientC * vec3(texture(material.diffuseMap, fragData.uv));
	vec3 diffuseTerm = pLight.diffuseC * vec3(texture(material.diffuseMap, fragData.uv)) * diffuseIntensity;
	vec3 specularTerm = pLight.specularC * vec3(texture(material.specularMap, fragData.uv)) * specularIntensity;
	return (ambientTerm + diffuseTerm + specularTerm);
}

vec3 CalculatePointLight(PointLight pLight, vec3 pNormal, vec3 pFragPos, vec3 pEyeDir)
{
	vec3 lightDir = normalize(pLight.position - fragData.position);
	//diffuse
	float diffuseIntensity = max(dot(pNormal, lightDir), 0.0f);
	//specular
	vec3 reflectDir = reflect(-lightDir, pNormal);
	float specularIntensity = pow(max(dot(pEyeDir, reflectDir), 0.0f), material.shininess);
	//attenuation
	float dist = length(pLight.position - fragData.position);
	float attenuation = 1.0f / (pLight.constAtten + pLight.linearAtten * dist + pLight.quadAtten * pow(dist, 2));
	//result 
	vec3 ambientTerm = pLight.ambientC * vec3(texture(material.diffuseMap, fragData.uv)) * attenuation;
	vec3 diffuseTerm = pLight.diffuseC * vec3(texture(material.diffuseMap, fragData.uv)) * attenuation * diffuseIntensity;
	vec3 specularTerm = pLight.specularC * vec3(texture(material.specularMap, fragData.uv)) * attenuation * specularIntensity;
	return (ambientTerm + diffuseTerm + specularTerm);
}

vec3 CalculateSpotLight(SpotLight pLight, vec3 pNormal, vec3 pFragPos, vec3 pEyeDir)
{
	vec3 lightDir = normalize(pLight.position - fragData.position);
	//diffuse
	float diffuseIntensity = max(dot(pNormal, lightDir), 0.0f);
	//specular
	vec3 reflectDir = reflect(-lightDir, pNormal);
	float specularIntensity = pow(max(dot(pEyeDir, reflectDir), 0.0), material.shininess);
	//attenuation
	float dist = length(pLight.position-fragData.position);
	float attenuation = 1.0f / (pLight.constAtten + pLight.linearAtten * dist + pLight.quadAtten * pow(dist, 2));
	//spotlight
	float theta = dot(lightDir, normalize(-pLight.spotDirection)); 	//the angle between the spot direction and the light to pixel direction
	float epsilon = pLight.cutoff - pLight.farCutoff;				//the gap between inner and outer rings
	float spotIntensity = clamp((theta - pLight.farCutoff) / epsilon, 0.0, 1.0); //intensity clamped from 1 to 0 as light reachers the farcutoff ring
	//result 
	vec3 ambientTerm = pLight.ambientC * vec3(texture(material.diffuseMap, fragData.uv)) * attenuation * spotIntensity;
	vec3 diffuseTerm = pLight.diffuseC * vec3(texture(material.diffuseMap, fragData.uv)) * attenuation * diffuseIntensity * spotIntensity;
	vec3 specularTerm = pLight.specularC * vec3(texture(material.specularMap, fragData.uv)) * attenuation * specularIntensity * spotIntensity;
	return (ambientTerm + diffuseTerm + specularTerm);
}

