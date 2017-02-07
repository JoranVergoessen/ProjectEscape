#include <glm.hpp>

#include "mge/materials/TerrainMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/World.hpp"
#include "mge/config.hpp"

ShaderProgram* TerrainMaterial::_shader = NULL;

//light
GLint _uEyePosition = 0;
GLint _uDLDirection;
GLint _uDLAmbientC;
GLint _uDLDiffuseC;
GLint _uDLSpecularC;
GLint _uDLIntensity;

//textures
GLint _uTextureDiffuseR = 0;
GLint _uTextureDiffuseG = 0;
GLint _uTextureDiffuseB = 0;
GLint _uTextureDiffuseA = 0;
GLint _uTextureHeightMap = 0;
GLint _uTextureSplatMap = 0;

//matrices
GLint _uModel = 0;
GLint _uProj = 0;
GLint _uView = 0;
GLint _uNormal = 0;

//vertex
GLint _aVertex = 0;
GLint _aNormal = 0;
GLint _aUV = 0;

GLint _uTime = 0;

GameObject* TerrainMaterial::_camera;


TerrainMaterial::TerrainMaterial(Texture * pDiffuseTextureR, Texture * pDiffuseTextureG, Texture * pDiffuseTextureB, Texture * pDiffuseTextureA, Texture* pHeightMap, Texture* pSplatMap):
	_diffuseTextureR(pDiffuseTextureR), _diffuseTextureG(pDiffuseTextureG), _diffuseTextureB(pDiffuseTextureB), _diffuseTextureA(pDiffuseTextureA), _textureHeightMap(pHeightMap), _textureSplatMap(pSplatMap) 
{
    _lazyInitializeShader();
}

TerrainMaterial::~TerrainMaterial() {}

void TerrainMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"terrain.vert");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"terrain.frag");
        _shader->finalize();

		_uEyePosition = _shader->getUniformLocation("eyePosition");
		_camera = World::findGameObjectByName("mainCamera");

		_uDLDirection = _shader->getUniformLocation("dirLight.direction");
		_uDLAmbientC = _shader->getUniformLocation("dirLight.ambientC");
		_uDLDiffuseC = _shader->getUniformLocation("dirLight.diffuseC");
		_uDLSpecularC = _shader->getUniformLocation("dirLight.specularC");
		_uDLIntensity = _shader->getUniformLocation("dirLight.intensity");

		_uTextureDiffuseR = _shader->getUniformLocation("textureDiffuseR");
		_uTextureDiffuseG = _shader->getUniformLocation("textureDiffuseG");
		_uTextureDiffuseB = _shader->getUniformLocation("textureDiffuseB");
		_uTextureDiffuseA = _shader->getUniformLocation("textureDiffuseA");

		_uTextureHeightMap = _shader->getUniformLocation("textureHeightMap");
		_uTextureSplatMap = _shader->getUniformLocation("textureSplatMap");

		_uModel = _shader->getUniformLocation("modelMatrix");
		_uView = _shader->getUniformLocation("viewMatrix");
		_uProj = _shader->getUniformLocation("projectionMatrix");
		_uNormal = _shader->getUniformLocation("normalMatrix");

		_aVertex = _shader->getAttribLocation("vertexPos");
		_aNormal = _shader->getAttribLocation("vertexNormal");
		_aUV = _shader->getAttribLocation("vertexUv");

		_uTime = _shader->getUniformLocation("time");
    }
}

void TerrainMaterial::setDiffuseTextures (Texture* pDiffuseTextureR, Texture* pDiffuseTextureG, Texture* pDiffuseTextureB, Texture* pDiffuseTextureA) 
{
	_diffuseTextureR = pDiffuseTextureR;
	_diffuseTextureG = pDiffuseTextureG;
	_diffuseTextureB = pDiffuseTextureB;
	_diffuseTextureA = pDiffuseTextureA;
}

void TerrainMaterial::SetTextureHeightMap(Texture* pHeightMap) {
	_textureHeightMap = pHeightMap;
}

void TerrainMaterial::SetTextureSplatMap(Texture* pSplatMap) {
	_textureSplatMap = pSplatMap;
}


void TerrainMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTextureR) return;

	_shader->use();

	glUniform1f(_uTime, clock() / (float)CLOCKS_PER_SEC);
	glUniform3fv(_uEyePosition, 1, glm::value_ptr(_camera->getWorldPosition()));

	//light
	Light* cl = World::GetLights()[0];
	glUniform3fv(_uDLDirection, 1, glm::value_ptr(cl->GetDirection()));
	glUniform3fv(_uDLAmbientC, 1, glm::value_ptr(cl->ambientColor));
	glUniform3fv(_uDLDiffuseC, 1, glm::value_ptr(cl->diffuseColor));
	glUniform3fv(_uDLSpecularC, 1, glm::value_ptr(cl->specularColor));
	glUniform1f(_uDLIntensity, cl->intensity);
	//cout << cl->GetDirection() << "::" << cl->ambientColor << "::" << cl->intensity << endl;

	//setup textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTextureR->getId());
	glUniform1i(_uTextureDiffuseR, 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _diffuseTextureG->getId());
	glUniform1i(_uTextureDiffuseG, 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, _diffuseTextureB->getId());
	glUniform1i(_uTextureDiffuseB, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, _diffuseTextureA->getId());
	glUniform1i(_uTextureDiffuseA, 3);

	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, _textureHeightMap->getId());
	glUniform1i(_uTextureHeightMap, 4);

	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, _textureSplatMap->getId());
	glUniform1i(_uTextureSplatMap, 5);

	//pass in all MVP matrices separately
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(pModelMatrix));

	glUniformMatrix4fv(_uProj, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_uView, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_uModel, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_uNormal, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
