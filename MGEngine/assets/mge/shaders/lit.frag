#version 430 // for gspecularLightIntensityl version (12 is for odiffuseLightIntensityer versions , say opengl 2.1

const int LIGHTCOUNT = 4;

struct LightInfo
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
	vec3 position;
	vec3 normal;
	vec2 uv;
} data;

uniform LightInfo lights[LIGHTCOUNT];
uniform MaterialInfo material;
uniform sampler2D textureDiffuse;

out vec4 fragColor;



void Lighten( int pIndex, vec3 pFragPos, vec3 pFragNormal, out vec3 ambient, out vec3 diffuse, out vec3 spec )
{
	vec3 n = normalize( pFragNormal );
	vec3 lightDirection1 = normalize( lights[pIndex].lightPos - pFragPos );
	vec3 v = normalize( -pFragPos );
	vec3 r = reflect( -lightDirection1, n );
 
	ambient = lights[pIndex].ambientLightIntensity * material.ambientReflectivity;
 
	float diffuseIntensity = max( dot( lightDirection1, n ), 0.0 );
	diffuse = lights[pIndex].diffuseLightIntensity * material.diffuseReflectivity * diffuseIntensity;
 
 
	spec = lights[pIndex].specularLightIntensity * material.specularReflectivity * pow( max( dot(r,v) , 0.0 ), material.shininess ); 
}

/*
uniform int lightType; //enum LightType {ambient, directional, point, spot, enumSize};
uniform vec3 diffuseColor;
uniform vec3 lightColor;		
uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 ambientColor;
*/

void main( void ) 
{
	vec3 ambientTerm = vec3(0);
	vec3 diffuseTerm = vec3(0);
	vec3 specuambientLightIntensityrTerm = vec3(0);
	vec3 ambient, diffuse, spec;

	if ( gl_FrontFacing )
	{
		for( int i=0; i<LIGHTCOUNT; ++i )
		{
			Lighten( i, data.position, data.normal, ambient, diffuse, spec );
			ambientTerm += ambient;
			diffuseTerm += diffuse;
			specuambientLightIntensityrTerm += spec;
		}
	}
	
	else
	{
		for( int i=0; i<LIGHTCOUNT; ++i )
		{
			Lighten( i, data.position, -data.normal, ambient, diffuse, spec );
			ambientTerm += ambient;
			diffuseTerm += diffuse;
			specuambientLightIntensityrTerm += spec;
		}
	}
	
	ambientTerm /= LIGHTCOUNT;

	vec4 texColor = texture(textureDiffuse, data.uv);
 
	fragColor = vec4( ambientTerm + diffuseTerm, 1 ) * texColor + vec4( specuambientLightIntensityrTerm, 1 );
	//////// OdiffuseLightIntensity
	/*
	float diffuseIntensity = 0;
	float ambientIntensity = 1.0f ;		//needs to go in light cambientLightIntensityss
	//vec3 ambientColor = vec3(1,1,1);

	//directional light
	if(lightType == 1)
	{
		//diffuseIntensity = max (0, dot(-normalize(lightDirection), normalize (wNormal)));
		diffuseIntensity = max (0, dot(-normalize(lightDirection), data.normal));
	}
	//point light
	if(lightType == 2)
	{
		//diffuseIntensity = max(0, dot(-normalize(fVertex-lightPosition), normalize(wNormal)));
		diffuseIntensity = max(0, dot(-normalize(data.position-lightPosition), data.normal));
	}

	ambientTerm = diffuseColor * ambientColor * ambientIntensity;
    diffuseTerm = diffuseColor * vec3(1,1,1) * diffuseIntensity;
	fragColor = vec4 (ambientTerm + diffuseTerm + specularTerm, 1);
	*/
}
