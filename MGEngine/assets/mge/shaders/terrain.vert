//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform mat3 	normalMatrix;

uniform float time;

uniform sampler2D textureHeightMap;
uniform sampler2D textureSplatMap;

float rippleScale = 0.03;
float rippleSpeed = 3;
float rippleFreq = 20;

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexUv;

out FragData
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fragData;


//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-	DEFORMED NORMALS	-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
vec3 CalculateDeformedNormal()
{
	float offset = 1.0f / textureSize(textureHeightMap, 0).x; //uv size
	vec2 leftUV = vec2(vertexUv.x - offset, vertexUv.y);
	vec2 rightUV = vec2(vertexUv.x + offset, vertexUv.y);
	vec2 downUV = vec2(vertexUv.x, vertexUv.y + offset);
	vec2 upUV = vec2(vertexUv.x, vertexUv.y - offset);
	
	float leftSample = length(texture(textureHeightMap, leftUV));
	float rightSample = length(texture(textureHeightMap, rightUV));
	float upSample = length(texture(textureHeightMap, upUV));
	float downSample = length(texture(textureHeightMap, downUV));
	
	vec3 left = vec3(leftUV.x, leftSample, leftUV.y);
	vec3 right = vec3(rightUV.x, rightSample, rightUV.y);
	vec3 down = vec3(downUV.x, downSample, downUV.y);
	vec3 up = vec3(upUV.x, upSample, upUV.y);
	
	vec3 a = down - up;
	vec3 b = right - left;
	vec3 newNormal = cross(a, b);
	return newNormal;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-	MAIN 	-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=--=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
void main( void ){
	vec3 newVertex = vertexPos;
	
	//heightmap values
	float height = length(vec3(texture(textureHeightMap, vertexUv)));
	//float height = texture(textureHeightMap, vertexUv).r;
	newVertex.y = height;
	
	//wave calculations
	float waveFactor = vec3(texture(textureSplatMap, vertexUv)).x; //control the waves based on the water texture
	if(waveFactor != 0)
	{
		float dist = sqrt(pow(vertexPos.x, 2) + pow(vertexPos.z, 2));
		float ripple = waveFactor * rippleScale * sin(time * rippleSpeed + dist * rippleFreq);
		newVertex.y = ripple;
	}
	
	//frag data
	fragData.position = vec3(modelMatrix * vec4(newVertex, 1.0f));
	//fragData.normal = normalize(normalMatrix*vertexNormal);
	//fragData.normal = normalMatrix * vertexNormal;
	fragData.normal = CalculateDeformedNormal();
	fragData.uv = vertexUv;
	
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(newVertex, 1.f);
}
