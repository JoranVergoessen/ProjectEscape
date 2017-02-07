#pragma once

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"
#include <SFML/Graphics.hpp>


class LitMaterial : public AbstractMaterial
{
    public:
		LitMaterial(glm::vec3 pColor = glm::vec3(1,0,0));
        virtual ~LitMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

        //in rgb values
        void setDiffuseColor (glm::vec3 pDiffuseColor);

    private:
        //all the static properties are shared between instances of ColorMaterial
        //note that they are all PRIVATE, we do not expose this static info to the outside world
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

        //in this example we cache all identifiers for uniforms & attributes
        static GLint _uModelMatrix;
		static GLint _uViewMatrix;
		static GLint _uProjectionMatrix;

        static GLint _uDiffuseColor;

		static GLint _aVertex;
        static GLint _aNormal;
		static GLint _aUV;

		//light information
		static GLint _uLightType;
		static GLint _uLightColor;
		static GLint _uLightPosition;
		static GLint _uLightDirection;
		static GLint _uAmbientColor;

        //this one is unique per instance of color material
        glm::vec3 _diffuseColor;
};
