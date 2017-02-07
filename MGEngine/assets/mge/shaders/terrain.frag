//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform vec3 eyePosition;

struct DirLight
{
	vec3 direction;
	vec3 ambientC;
	vec3 diffuseC;
	vec3 specularC;
	float intensity;
};
uniform DirLight dirLight;

uniform sampler2D textureDiffuseR; 	//water
uniform sampler2D textureDiffuseG;	//grass
uniform sampler2D textureDiffuseB;	//stone
uniform sampler2D textureDiffuseA;	//snow

float shininess = 64;

uniform sampler2D textureSplatMap;

uniform float time;

in FragData
{
	vec3 position;		//in world space
	vec3 normal;		//in world space 
	vec2 uv;			
} fragData;

out vec4 fragment_color;



//==============================================================		totalDiffuse 		===========================================================================
vec3 GetTotalDiffuse()
{

	vec4 splatColor = texture(textureSplatMap, fragData.uv);
	
	float fragHeight = fragData.position.y;		//for light adjustment
	
	vec4 diff2 = splatColor.x * texture(textureDiffuseR, fragData.uv) * (1 + fragHeight*2);
	vec4 diff1 = splatColor.y * texture(textureDiffuseG, fragData.uv);
	vec4 diff3 = splatColor.z * texture(textureDiffuseB, fragData.uv);
	vec4 diff4 = splatColor.w * texture(textureDiffuseA, fragData.uv);
	vec3 totalDiff = vec3(diff1 + (diff2)/2 + diff3 + diff4);
	return totalDiff;
}

//==============================================================		directional 		===========================================================================
vec3 CalculateDirLight(DirLight pLight, vec3 pNormal, vec3 pEyeDir, vec3 pTotalDiffuse)
{
	vec3 lightDir = normalize(-pLight.direction);
	//diffuse
	float diffuseIntensity = max(dot(pNormal, lightDir),0.0f);
	//specular
	vec3 reflectDir = reflect(-lightDir, pNormal);
	float specularIntensity = pow(max(dot(pEyeDir, reflectDir), 0.0f), shininess);
	//result
	vec3 ambientTerm = pLight.ambientC * pTotalDiffuse;
	vec3 diffuseTerm = pLight.diffuseC * pTotalDiffuse * diffuseIntensity;
	vec3 specularTerm = pLight.specularC * pTotalDiffuse * specularIntensity;
	return (ambientTerm + diffuseTerm + specularTerm) *	pLight.intensity;
}



void main( void ) 
{

	vec3 norm = normalize(fragData.normal);//fragment based normals
	//vec3 norm = normalize(vec3(0, 1, 0));
	vec3 eyeDir = normalize(eyePosition - fragData.position);
	
	fragment_color = vec4(CalculateDirLight(dirLight, norm, eyeDir, GetTotalDiffuse()), 1.0f);
}
