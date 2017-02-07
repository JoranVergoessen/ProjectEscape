#include <glm.hpp>

#include "mge/materials/LitMaterial.hpp"
#include "mge/config.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/Lights/Light.hpp"

ShaderProgram* LitMaterial::_shader = NULL;

GLint LitMaterial::_uModelMatrix = 0;
GLint LitMaterial::_uViewMatrix = 0;
GLint LitMaterial::_uProjectionMatrix = 0;
GLint LitMaterial::_uDiffuseColor = 0;

GLint LitMaterial::_aVertex = 0;
GLint LitMaterial::_aNormal = 0;
GLint LitMaterial::_aUV = 0;

GLint LitMaterial::_uLightType = 0;
GLint LitMaterial::_uLightColor = 0;
GLint LitMaterial::_uLightPosition = 0;
GLint LitMaterial::_uLightDirection = 0;
GLint LitMaterial::_uAmbientColor = 0;

LitMaterial::LitMaterial(glm::vec3 pDiffuseColor):_diffuseColor (pDiffuseColor)
{
    //every time we create an instance of colormaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
}

void LitMaterial::_lazyInitializeShader() 
{
    //this shader contains everything the material can do (it can render something in 3d using a single color)
    if (!_shader) 
	{
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"lit.vert");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"lit.frag");
        _shader->finalize();

        //cache all the uniform and attribute indices
        _uModelMatrix = _shader->getUniformLocation("modelMatrix");
		_uViewMatrix = _shader->getUniformLocation("viewMatrix");
		_uProjectionMatrix = _shader->getUniformLocation("projectionMatrix");
        _uDiffuseColor = _shader->getUniformLocation("diffuseColor");

        _aVertex = _shader->getAttribLocation("vertexPos");
        _aNormal = _shader->getAttribLocation("vertexNormal");
        _aUV =     _shader->getAttribLocation("vertexUV");

		_uLightType = _shader->getUniformLocation("lightType");
		_uLightColor = _shader->getUniformLocation("lightColor");
		_uLightPosition = _shader->getUniformLocation("lightPosition");
		_uLightDirection = _shader->getUniformLocation("lightDirection");
		_uAmbientColor = _shader->getUniformLocation("ambientColor");
    }
}

LitMaterial::~LitMaterial()
{
    //dtor
}

void LitMaterial::setDiffuseColor(glm::vec3 pDiffuseColor) 
{
    _diffuseColor = pDiffuseColor;
}

void LitMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
	_shader->use();

	//pass in a precalculate mvp matrix (see texture material for the opposite)
	glUniformMatrix4fv(_uModelMatrix, 1, GL_FALSE, glm::value_ptr(pModelMatrix));
	glUniformMatrix4fv(_uViewMatrix, 1, GL_FALSE, glm::value_ptr(pViewMatrix));
	glUniformMatrix4fv(_uProjectionMatrix, 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));

	//set the material color
	glUniform3fv(_uDiffuseColor, 1, glm::value_ptr(_diffuseColor));

	//light information
	for (int i = 0; i < World::GetLights().size(); i++)
	{
		Light* cl = World::GetLights()[i];

		glUniform1i(_uLightType, cl->type);
		//glUniform3fv(_uLightColor, 1, glm::value_ptr(cl->color));
		glUniform3fv(_uLightPosition, 1, glm::value_ptr(cl->getWorldPosition()));
		glUniform3fv(_uLightDirection, 1, glm::value_ptr(cl->GetDirection()));
		glUniform3fv(_uAmbientColor, 1, glm::value_ptr(cl->ambientColor));
	}
	//now inform mesh of where to stream its data
	pMesh->streamToOpenGL(_aVertex, _aNormal, _aUV);

}
