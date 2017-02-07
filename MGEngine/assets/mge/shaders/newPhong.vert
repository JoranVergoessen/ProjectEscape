#version 430 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexUV;

out FragData
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} fragData;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

void main( void )
{
	mat4 mvpMatrix = projectionMatrix*viewMatrix*modelMatrix;
	
	fragData.position = vec3(modelMatrix * vec4(vertexPos, 1));
	//fragData.normal = normalize(normalMatrix*vertexNormal);
	fragData.normal = normalMatrix * vertexNormal;
	fragData.uv = vertexUV;

	gl_Position = mvpMatrix * vec4(vertexPos, 1.f);
}
