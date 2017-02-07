#include <glm.hpp>

#include "mge/materials/LitTextureMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/World.hpp"
#include "mge/config.hpp"

ShaderProgram* LitTextureMaterial::_shader = NULL;
//matrices
GLint LitTextureMaterial::_uModelMatrix = 0;
GLint LitTextureMaterial::_uViewMatrix = 0;
GLint LitTextureMaterial::_uProjectionMatrix = 0;
GLint LitTextureMaterial::_uNormalMatrix = 0;

//material info
GLint LitTextureMaterial::_uMatAmbient = 0;
GLint LitTextureMaterial::_uMatDiffuse = 0;
GLint LitTextureMaterial::_uMatDiffuseMap = 0;
GLint LitTextureMaterial::_uMatSpecularMap = 0;
GLint LitTextureMaterial::_uMatSpecular = 0;
GLint LitTextureMaterial::_uMatShininess = 0;

//light info
GLint LitTextureMaterial::_uEyePosition = 0;


GLint LitTextureMaterial::_uDLDirection;
GLint LitTextureMaterial::_uDLAmbientC;
GLint LitTextureMaterial::_uDLDiffuseC;
GLint LitTextureMaterial::_uDLSpecularC;
GLint LitTextureMaterial::_uDLIntensity;

GLint LitTextureMaterial::_uPLPosition0;
GLint LitTextureMaterial::_uPLConstAtten0;
GLint LitTextureMaterial::_uPLLinearAtten0;
GLint LitTextureMaterial::_uPLQuadAtten0;
GLint LitTextureMaterial::_uPLAmbientC0;
GLint LitTextureMaterial::_uPLDiffuseC0;
GLint LitTextureMaterial::_uPLSpecularC0;
GLint LitTextureMaterial::_uPLIntensity0;

GLint LitTextureMaterial::_uPLPosition1;
GLint LitTextureMaterial::_uPLConstAtten1;
GLint LitTextureMaterial::_uPLLinearAtten1;
GLint LitTextureMaterial::_uPLQuadAtten1;
GLint LitTextureMaterial::_uPLAmbientC1;
GLint LitTextureMaterial::_uPLDiffuseC1;
GLint LitTextureMaterial::_uPLSpecularC1;
GLint LitTextureMaterial::_uPLIntensity1;

GLint LitTextureMaterial::_uPLPosition2;
GLint LitTextureMaterial::_uPLConstAtten2;
GLint LitTextureMaterial::_uPLLinearAtten2;
GLint LitTextureMaterial::_uPLQuadAtten2;
GLint LitTextureMaterial::_uPLAmbientC2;
GLint LitTextureMaterial::_uPLDiffuseC2;
GLint LitTextureMaterial::_uPLSpecularC2;
GLint LitTextureMaterial::_uPLIntensity2;

GLint LitTextureMaterial::_uPLPosition3;
GLint LitTextureMaterial::_uPLConstAtten3;
GLint LitTextureMaterial::_uPLLinearAtten3;
GLint LitTextureMaterial::_uPLQuadAtten3;
GLint LitTextureMaterial::_uPLAmbientC3;
GLint LitTextureMaterial::_uPLDiffuseC3;
GLint LitTextureMaterial::_uPLSpecularC3;
GLint LitTextureMaterial::_uPLIntensity3;

GLint LitTextureMaterial::_uSLPosition;
GLint LitTextureMaterial::_uSLSpotDir;
GLint LitTextureMaterial::_uSLCutoff;
GLint LitTextureMaterial::_uSLFarCutoff;
GLint LitTextureMaterial::_uSLConstAtten;
GLint LitTextureMaterial::_uSLLinearAtten;
GLint LitTextureMaterial::_uSLQuadAtten;
GLint LitTextureMaterial::_uSLAmbientC;
GLint LitTextureMaterial::_uSLDiffuseC;
GLint LitTextureMaterial::_uSLSpecularC;
GLint LitTextureMaterial::_uSLIntensity;

//vertex info
GLint LitTextureMaterial::_aVertex = 0;
GLint LitTextureMaterial::_aNormal = 0;
GLint LitTextureMaterial::_aUV = 0;

GameObject* LitTextureMaterial::_cam;

LitTextureMaterial::LitTextureMaterial(Texture * pDiffuseTexture) : _diffuseTexture(pDiffuseTexture), _specularTexture(pDiffuseTexture)
{
	_lazyInitializeShader();
}

LitTextureMaterial::LitTextureMaterial(Texture* pDiffuseTexture, Texture* pSpecularTexture, float pShininess) : _diffuseTexture(pDiffuseTexture), _specularTexture(pSpecularTexture), _shininess(pShininess)
{
	_lazyInitializeShader();
}

LitTextureMaterial::~LitTextureMaterial() {}

void LitTextureMaterial::_lazyInitializeShader() {
	if (!_shader) {
		_shader = new ShaderProgram();
		_shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH + "newPhong.vert");
		_shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH + "newPhong.frag");
		_shader->finalize();

		//cache all the uniform and attribute indices
		//matrices
		_uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uViewMatrix = _shader->getUniformLocation("viewMatrix");
		_uProjectionMatrix = _shader->getUniformLocation("projectionMatrix");
		_uNormalMatrix = _shader->getUniformLocation("normalMatrix");

		//material
		//_uMatAmbient =		_shader->getUniformLocation("material.ambientColor"); // NOT USED
		//_uMatDiffuse =		_shader->getUniformLocation("material.diffuseColor"); // NOT USED
		//_uMatSpecular =		_shader->getUniformLocation("material.specularColor"); // NOT USED
		_uMatDiffuseMap =	_shader->getUniformLocation("material.diffuseMap");
		_uMatSpecularMap =	_shader->getUniformLocation("material.specularMap");
		_uMatShininess =	_shader->getUniformLocation("material.shininess");
		
		//light
		_uEyePosition = _shader->getUniformLocation("eyePosition");


		_uDLDirection = _shader->getUniformLocation("dirLight.direction");
		_uDLAmbientC =	_shader->getUniformLocation("dirLight.ambientC");
		_uDLDiffuseC =	_shader->getUniformLocation("dirLight.diffuseC");
		_uDLSpecularC =	_shader->getUniformLocation("dirLight.specularC");
		_uDLIntensity = _shader->getUniformLocation("dirLight.intensity");

		_uPLPosition0 =		_shader->getUniformLocation("pointLights[0].position");
		_uPLConstAtten0 =	_shader->getUniformLocation("pointLights[0].constAtten");
		_uPLLinearAtten0 =	_shader->getUniformLocation("pointLights[0].linearAtten");
		_uPLQuadAtten0 =	_shader->getUniformLocation("pointLights[0].quadAtten");
		_uPLAmbientC0 =		_shader->getUniformLocation("pointLights[0].ambientC");
		_uPLDiffuseC0 =		_shader->getUniformLocation("pointLights[0].diffuseC");
		_uPLSpecularC0 =	_shader->getUniformLocation("pointLights[0].specularC");
		_uPLIntensity0 = _shader->getUniformLocation("pointLights[0].intensity");

		_uPLPosition1 =		_shader->getUniformLocation("pointLights[1].position");
		_uPLConstAtten1 =	_shader->getUniformLocation("pointLights[1].constAtten");
		_uPLLinearAtten1 =	_shader->getUniformLocation("pointLights[1].linearAtten");
		_uPLQuadAtten1 =	_shader->getUniformLocation("pointLights[1].quadAtten");
		_uPLAmbientC1 =		_shader->getUniformLocation("pointLights[1].ambientC");
		_uPLDiffuseC1 =		_shader->getUniformLocation("pointLights[1].diffuseC");
		_uPLSpecularC1 =	_shader->getUniformLocation("pointLights[1].specularC");
		_uPLIntensity1 = _shader->getUniformLocation("pointLights[1].intensity");

		_uPLPosition2 =		_shader->getUniformLocation("pointLights[2].position");
		_uPLConstAtten2 =	_shader->getUniformLocation("pointLights[2].constAtten");
		_uPLLinearAtten2 =	_shader->getUniformLocation("pointLights[2].linearAtten");
		_uPLQuadAtten2 =	_shader->getUniformLocation("pointLights[2].quadAtten");
		_uPLAmbientC2 =		_shader->getUniformLocation("pointLights[2].ambientC");
		_uPLDiffuseC2 =		_shader->getUniformLocation("pointLights[2].diffuseC");
		_uPLSpecularC2 =	_shader->getUniformLocation("pointLights[2].specularC");
		_uPLIntensity2 = _shader->getUniformLocation("pointLights[2].intensity");

		_uPLPosition3 =		_shader->getUniformLocation("pointLights[3].position");
		_uPLConstAtten3 =	_shader->getUniformLocation("pointLights[3].constAtten");
		_uPLLinearAtten3 =	_shader->getUniformLocation("pointLights[3].linearAtten");
		_uPLQuadAtten3 =	_shader->getUniformLocation("pointLights[3].quadAtten");
		_uPLAmbientC3 =		_shader->getUniformLocation("pointLights[3].ambientC");
		_uPLDiffuseC3 =		_shader->getUniformLocation("pointLights[3].diffuseC");
		_uPLSpecularC3 =	_shader->getUniformLocation("pointLights[3].specularC");
		_uPLIntensity3 = _shader->getUniformLocation("pointLights[3].intensity");

		_uSLPosition =		_shader->getUniformLocation("spotLight.position");
		_uSLSpotDir =		_shader->getUniformLocation("spotLight.spotDirection");
		_uSLCutoff =		_shader->getUniformLocation("spotLight.cutoff");
		_uSLFarCutoff =		_shader->getUniformLocation("spotLight.farCutoff");
		_uSLConstAtten =	_shader->getUniformLocation("spotLight.constAtten");
		_uSLLinearAtten =	_shader->getUniformLocation("spotLight.linearAtten");
		_uSLQuadAtten =		_shader->getUniformLocation("spotLight.quadAtten");
		_uSLAmbientC =		_shader->getUniformLocation("spotLight.ambientC");
		_uSLDiffuseC =		_shader->getUniformLocation("spotLight.diffuseC");
		_uSLSpecularC =		_shader->getUniformLocation("spotLight.specularC");
		_uSLIntensity =		_shader->getUniformLocation("spotLight.intensity");
		
		//vertex
		_aVertex = _shader->getAttribLocation("vertexPos");
		_aNormal = _shader->getAttribLocation("vertexNormal");
		_aUV = _shader->getAttribLocation("vertexUV");

		_cam = World::findGameObjectByName("mainCamera");
	}
}

void LitTextureMaterial::setDiffuseTexture(Texture* pDiffuseTexture) {
	_diffuseTexture = pDiffuseTexture;
}

void LitTextureMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	if (!_diffuseTexture) return;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(pModelMatrix));//special matrix to transform the normals in world space without causing issues after non uniform scaling
	_shader->use();

	//pass in all MVP matrices separately
	glUniformMatrix4fv(_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
	glUniformMatrix4fv(_uViewMatrix, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix3fv(_uNormalMatrix, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	//======= materail info
	//setup texture slot 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
	glUniform1i(_uMatDiffuseMap, 0);
	//setup texture slot 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _specularTexture->getId()); //usually black/white picture to limit the parts that shine
	glUniform1i(_uMatSpecularMap, 1);

	//glUniform3fv(_uMatAmbient, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
	//glUniform3fv(_uMatDiffuse, 1, glm::value_ptr(glm::vec3(1.0f, 0.5f, 0.31f)));
	//glUniform3fv(_uMatSpecular, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
	glUniform1f(_uMatShininess, _shininess);

	//====== light information
	int pointLightCount = 0;
	for (int i = 0; i < World::GetLights().size(); i++)
	{
		glUniform3fv(_uEyePosition, 1, glm::value_ptr(_cam->getWorldPosition()));

		Light* cl = World::GetLights()[i];
		//cout << "current light is " << cl->type << endl;

		switch (cl->type) 
		{
			default:
				break;

			case Light::directional:
				//cout << "sending directional light info" << endl;
				glUniform3fv(_uDLDirection, 1, glm::value_ptr(cl->GetDirection()));
				glUniform3fv(_uDLAmbientC, 1, glm::value_ptr(cl->ambientColor));
				glUniform3fv(_uDLDiffuseC, 1, glm::value_ptr(cl->diffuseColor));
				glUniform3fv(_uDLSpecularC, 1, glm::value_ptr(cl->specularColor));
				glUniform1f(_uDLIntensity, cl->intensity);
				//cout <<" direction = "<<cl->GetDirection()<< " ambient = " << cl->ambientColor <<" diffuse = "<<cl->diffuseColor<<" spec = "<<cl->specularColor<< endl;
				break;

			case Light::point:
				//cout << "sending point light info" << endl;
				switch (pointLightCount)
				{
					default:
						break;

					case 0:
						/**/
						glUniform3fv(_uPLPosition0, 1, glm::value_ptr(cl->getWorldPosition()));
						glUniform3fv(_uPLAmbientC0, 1, glm::value_ptr(cl->ambientColor));
						glUniform3fv(_uPLDiffuseC0, 1, glm::value_ptr(cl->diffuseColor));
						glUniform3fv(_uPLSpecularC0, 1, glm::value_ptr(cl->specularColor));
						glUniform1f(_uPLConstAtten0, cl->constAtten);
						glUniform1f(_uPLLinearAtten0, cl->linearAtten);
						glUniform1f(_uPLQuadAtten0, cl->quadAtten);
						glUniform1f(_uPLIntensity0, cl->intensity);
						/**/
						break;

					case 1:
						glUniform3fv(_uPLPosition1, 1, glm::value_ptr(cl->getWorldPosition()));
						glUniform3fv(_uPLAmbientC1, 1, glm::value_ptr(cl->ambientColor));
						glUniform3fv(_uPLDiffuseC1, 1, glm::value_ptr(cl->diffuseColor));
						glUniform3fv(_uPLSpecularC1, 1, glm::value_ptr(cl->specularColor));
						glUniform1f(_uPLConstAtten1, cl->constAtten);
						glUniform1f(_uPLLinearAtten1, cl->linearAtten);
						glUniform1f(_uPLQuadAtten1, cl->quadAtten);
						glUniform1f(_uPLIntensity1, cl->intensity);
						break;

					case 2:
						glUniform3fv(_uPLPosition2, 1, glm::value_ptr(cl->getWorldPosition()));
						glUniform3fv(_uPLAmbientC2, 1, glm::value_ptr(cl->ambientColor));
						glUniform3fv(_uPLDiffuseC2, 1, glm::value_ptr(cl->diffuseColor));
						glUniform3fv(_uPLSpecularC2, 1, glm::value_ptr(cl->specularColor));
						glUniform1f(_uPLConstAtten2, cl->constAtten);
						glUniform1f(_uPLLinearAtten2, cl->linearAtten);
						glUniform1f(_uPLQuadAtten2, cl->quadAtten);
						glUniform1f(_uPLIntensity2, cl->intensity);
						break;

					case 3:
						glUniform3fv(_uPLPosition3, 1, glm::value_ptr(cl->getWorldPosition()));
						glUniform3fv(_uPLAmbientC3, 1, glm::value_ptr(cl->ambientColor));
						glUniform3fv(_uPLDiffuseC3, 1, glm::value_ptr(cl->diffuseColor));
						glUniform3fv(_uPLSpecularC3, 1, glm::value_ptr(cl->specularColor));
						glUniform1f(_uPLConstAtten3, cl->constAtten);
						glUniform1f(_uPLLinearAtten3, cl->linearAtten);
						glUniform1f(_uPLQuadAtten3, cl->quadAtten);
						glUniform1f(_uPLIntensity3, cl->intensity);
						break;
				}
				pointLightCount++;
				break;

			case Light::spot:
				//cout << "sending spot light info" << endl;
				glUniform3fv(_uSLPosition, 1, glm::value_ptr(cl->getWorldPosition()));
				glUniform3fv(_uSLSpotDir, 1, glm::value_ptr(cl->GetDirection()));
				glUniform3fv(_uSLAmbientC, 1, glm::value_ptr(cl->ambientColor));
				glUniform3fv(_uSLDiffuseC, 1, glm::value_ptr(cl->diffuseColor));
				glUniform3fv(_uSLSpecularC, 1, glm::value_ptr(cl->specularColor));
				glUniform1f(_uSLConstAtten, cl->constAtten);
				glUniform1f(_uSLLinearAtten, cl->linearAtten);
				glUniform1f(_uSLQuadAtten, cl->quadAtten);
				glUniform1f(_uSLCutoff, cl->cutoff);
				glUniform1f(_uSLFarCutoff, cl->farCutoff);
				glUniform1f(_uSLIntensity, cl->intensity);
				//cout << cl->getWorldPosition() << " || " << cl->GetDirection() << " || " << cl->ambientColor << " || " << cl->diffuseColor << " || " << cl->specularColor << " || " << cl->constAtten << " || " << cl->linearAtten << " || " << cl->quadAtten << " || " << cl->cutoff << " || " << cl->farCutoff
				//	<< endl;
				break;
		}
	}

	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);
}
