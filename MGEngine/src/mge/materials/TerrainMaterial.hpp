#pragma once

#include "mge/core/ShaderProgram.hpp"
#include "mge/core/Texture.hpp"
#include "mge/materials/AbstractMaterial.hpp"

/**
 * Simple single texture material, this is a sample which doesn't cache anything upfront and
 * passes in separate matrices for the MVP calculation
 */
class TerrainMaterial : public AbstractMaterial
{
    public:
		TerrainMaterial(Texture* pDiffuseTextureR, Texture* pDiffuseTextureG, Texture* pDiffuseTextureB, Texture* pDiffuseTextureA, Texture* pHeightMap, Texture* pSplatMap);
        virtual ~TerrainMaterial();

        virtual void render(Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;

		void setDiffuseTextures(Texture* pDiffuseTextureR, Texture* pDiffuseTextureG, Texture* pDiffuseTextureB, Texture* pDiffuseTextureA);
		void SetTextureHeightMap(Texture* pHeightMap);
		void SetTextureSplatMap(Texture* pSplatMap);

    protected:
    private:
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();

		Texture* _diffuseTextureR;
		Texture* _diffuseTextureG;
		Texture* _diffuseTextureB;
		Texture* _diffuseTextureA;

		Texture* _textureHeightMap;
		Texture* _textureSplatMap;

		static GameObject* _camera;

		TerrainMaterial(const TerrainMaterial&);
		TerrainMaterial& operator=(const TerrainMaterial&);

};
