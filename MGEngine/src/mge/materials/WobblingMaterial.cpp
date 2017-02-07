#include <glm.hpp>

#include "mge/materials/WobblingMaterial.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/GameObject.hpp"
#include "mge/config.hpp"

ShaderProgram* WobblingMaterial::_shader = NULL;
GLint vertexLoc = 0;
GLint normalLoc = 0;
GLint uvLoc = 0;

WobblingMaterial::WobblingMaterial(Texture * pDiffuseTexture):_diffuseTexture(pDiffuseTexture) {
    _lazyInitializeShader();
}

WobblingMaterial::~WobblingMaterial() {}

void WobblingMaterial::_lazyInitializeShader() {
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_VERTEX_SHADER, config::MGE_SHADER_PATH+"wob.vert");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADER_PATH+"wob.frag");
        _shader->finalize();
    }
	vertexLoc = _shader->getAttribLocation("vertex");
	normalLoc = _shader->getAttribLocation("normal");
	uvLoc = _shader->getAttribLocation("uv");
}

void WobblingMaterial::setDiffuseTexture (Texture* pDiffuseTexture) {
    _diffuseTexture = pDiffuseTexture;
}

void WobblingMaterial::render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    if (!_diffuseTexture) return;

    _shader->use();

    //setup texture slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _diffuseTexture->getId());
    glUniform1i (_shader->getUniformLocation("textureDiffuse"), 0);
	glUniform1f(_shader->getUniformLocation("time"), clock()/(float)CLOCKS_PER_SEC);

	//wobbly stuff

    //pass in all MVP matrices separately
    glUniformMatrix4fv ( _shader->getUniformLocation("projectionMatrix"),   1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("viewMatrix"),         1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv ( _shader->getUniformLocation("modelMatrix"),        1, GL_FALSE, glm::value_ptr(pModelMatrix));

    //now inform mesh of where to stream its data
	pMesh->streamToOpenGL(vertexLoc, normalLoc, uvLoc);
}
