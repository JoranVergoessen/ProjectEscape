#version 430 // for glsl version (12 is for older versions , say opengl 2.1

in vec3 vertexPos;
in vec3 vertexNormal;
in vec2 vertexUV;

out fragData
{
	vec3 position;
	vec3 normal;
	vec2 uv;
} data;


uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main( void )
{
	mat4 mvpMatrix = projectionMatrix*viewMatrix*modelMatrix;
	
	data.position = vec3(modelMatrix*vec4(vertexPos,1.0f));
	data.normal = normalize(vec3(modelMatrix*vec4(vertexNormal,0)));
	data.uv = vertexUV;

	gl_Position = mvpMatrix * vec4(vertexPos, 1.f);
}
