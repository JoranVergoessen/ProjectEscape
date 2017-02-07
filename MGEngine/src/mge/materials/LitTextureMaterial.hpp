#pragma once

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"

/**
* Simple single texture material, this is a sample which doesn't cache anything upfront and
* passes in separate matrices for the MVP calculation
*/
class LitTextureMaterial : public AbstractMaterial
{
public:
	LitTextureMaterial(Texture* pDiffuseTexture);
	LitTextureMaterial(Texture* pDiffuseTexture, Texture* pSpecularTexture, float pShininess = 64.0f);
	virtual ~LitTextureMaterial();

	virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

	void setDiffuseTexture(Texture* pDiffuseTexture);

protected:
private:
	static ShaderProgram* _shader;
	static void _lazyInitializeShader();

	//in this example we cache all identifiers for uniforms & attributes
	static GLint _uModelMatrix;
	static GLint _uViewMatrix;
	static GLint _uProjectionMatrix;
	static GLint _uNormalMatrix;

	//light information
	static GLint _uEyePosition;

	static GLint _uSLIntensity;

	static GLint _uDLDirection;
	static GLint _uDLAmbientC;
	static GLint _uDLDiffuseC;
	static GLint _uDLSpecularC;
	static GLint _uDLIntensity;

	static GLint _uPLPosition0;
	static GLint _uPLConstAtten0;
	static GLint _uPLLinearAtten0;
	static GLint _uPLQuadAtten0;
	static GLint _uPLAmbientC0;
	static GLint _uPLDiffuseC0;
	static GLint _uPLSpecularC0;
	static GLint _uPLIntensity0;

	static GLint _uPLPosition1;
	static GLint _uPLConstAtten1;
	static GLint _uPLLinearAtten1;
	static GLint _uPLQuadAtten1;
	static GLint _uPLAmbientC1;
	static GLint _uPLDiffuseC1;
	static GLint _uPLSpecularC1;
	static GLint _uPLIntensity1;

	static GLint _uPLPosition2;
	static GLint _uPLConstAtten2;
	static GLint _uPLLinearAtten2;
	static GLint _uPLQuadAtten2;
	static GLint _uPLAmbientC2;
	static GLint _uPLDiffuseC2;
	static GLint _uPLSpecularC2;
	static GLint _uPLIntensity2;

	static GLint _uPLPosition3;
	static GLint _uPLConstAtten3;
	static GLint _uPLLinearAtten3;
	static GLint _uPLQuadAtten3;
	static GLint _uPLAmbientC3;
	static GLint _uPLDiffuseC3;
	static GLint _uPLSpecularC3;
	static GLint _uPLIntensity3;

	static GLint _uSLPosition;
	static GLint _uSLSpotDir;
	static GLint _uSLCutoff;
	static GLint _uSLFarCutoff;
	static GLint _uSLConstAtten;
	static GLint _uSLLinearAtten;
	static GLint _uSLQuadAtten;
	static GLint _uSLAmbientC;
	static GLint _uSLDiffuseC;
	static GLint _uSLSpecularC;

	//material information
	static GLint _uMatAmbient;
	static GLint _uMatDiffuse;
	static GLint _uMatDiffuseMap;
	static GLint _uMatSpecularMap;
	static GLint _uMatSpecular;
	static GLint _uMatShininess;

	//vertex information
	static GLint _aVertex;
	static GLint _aNormal;
	static GLint _aUV;

	Texture* _diffuseTexture;
	Texture* _specularTexture;
	float _shininess;	

	static GameObject* _cam;

	LitTextureMaterial(const LitTextureMaterial&);
	LitTextureMaterial& operator=(const LitTextureMaterial&);

};

