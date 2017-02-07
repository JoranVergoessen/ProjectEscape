#version 430 // for gspecularLightIntensityl version (12 is for odiffuseLightIntensityer versions , say opengl 2.1
//==================================================================================================================//
const int LIGHTCOUNT = 4;
struct LightSource
{
	vec3 lightPos;					//Light Position in eye-coords
	vec3 ambientLightIntensity;		//Ambient light intensity
	vec3 diffuseLightIntensity;		//Diffuse light intensity
	vec3 specularLightIntensity;	//Specular light intensity
};

struct MaterialInfo
{
	vec3 ambientReflectivity;			//Ambient reflectivity
	vec3 diffuseReflectivity;			//Diffuse reflectivity
	vec3 specularReflectivity;			//specular reflectivity
	float shininess;					//specular shininess factor
};

in fragData
{
	vec3 position; 	//interpolated pixel position
	vec3 normal;	//interpolated pixel normal
	vec2 uv;		//interpolated pixel UV
} data;

uniform LightSource lights[LIGHTCOUNT];
uniform MaterialInfo material;
uniform sampler2D textureDiffuse;

out vec4 fragColor; //final pixel color
//==================================================================================================================//


void Lighten( int pIndex, vec3 pFragPos, vec3 pFragNormal, out vec3 pAmbient, out vec3 pDiffuse, out vec3 pSpecular )
{
	vec3 n = normalize( pFragNormal );
	vec3 d = normalize( lights[pIndex].lightPos - pFragPos );
	vec3 v = normalize( -pFragPos );
	vec3 r = reflect( -d, n );
 
	pAmbient = lights[pIndex].ambientLightIntensity * material.ambientReflectivity;
 
	float diffuseIntensity = max( dot( d, n ), 0.0 );
	pDiffuse = lights[pIndex].diffuseLightIntensity * material.diffuseReflectivity * diffuseIntensity;
 
 
	pSpecular = lights[pIndex].specularLightIntensity * material.specularReflectivity * pow( max( dot(r,v) , 0.0 ), material.shininess ); 
}

void main( void ) 
{
	vec3 ambientTerm = vec3(0);
	vec3 diffuseTerm = vec3(0);
	vec3 specularTerm = vec3(0);
	vec3 ambient, diffuse, spec;

	if ( gl_FrontFacing )
	{
		for( int i=0; i<LIGHTCOUNT; ++i )
		{
			Lighten( i, data.position, data.normal, ambient, diffuse, spec );
			ambientTerm += ambient;
			diffuseTerm += diffuse;
			specularTerm += spec;
		}
	}
	
	else
	{
		for( int i=0; i<LIGHTCOUNT; ++i )
		{
			Lighten( i, data.position, -data.normal, ambient, diffuse, spec );
			ambientTerm += ambient;
			diffuseTerm += diffuse;
			specularTerm += spec;
		}
	}
	
	ambientTerm /= LIGHTCOUNT;

	vec4 texColor = texture(textureDiffuse, data.uv);
 
	//fragColor = vec4( ambientTerm + diffuseTerm, 1 ) * texColor + vec4( specularTerm, 1 );
	fragColor = texColor;
}
