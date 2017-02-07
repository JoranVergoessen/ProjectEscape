#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 	projectionMatrix;
uniform	mat4 	viewMatrix;
uniform	mat4 	modelMatrix;
uniform float	time;

in vec3 vertex;
in vec3 normal;
in vec2 uv;

float scaleSize = 5;
float scaleSpeed = 5;
out vec2 texCoord; //make sure the texture coord is interpolated

void main( void ){
	mat4 mvp = projectionMatrix*viewMatrix*modelMatrix;
	//center = vec3(modelMatrix[0][3], modelMatrix[1][3], modelMatrix[2][3]);
	vec3 center = vec3(0,0,0);
	vec3 direction = normalize(center - vertex);

	vec3 newVertex = vertex;
	newVertex.x += sin(newVertex.y*time);
	newVertex.y += sin(time*scaleSpeed)*scaleSize*direction.x;
	newVertex.z += cos(time*scaleSpeed)*scaleSize*direction.z;

	gl_Position = mvp * vec4(newVertex, 1.f);
	texCoord = uv;
}
