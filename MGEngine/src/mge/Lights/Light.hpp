#pragma once
#include <string>
#include "mge/core/GameObject.hpp"

class Light : public GameObject
{
public:
	enum LightType {directional, point, spot};
	//directional ctor
	Light(std::string pName, glm::vec3 pColor, glm::vec3 pPosition,float pIntensity, glm::vec3 pDirection);
	//point ctor
	Light(std::string pName, glm::vec3 pColor, glm::vec3 pPosition,float pIntensity, float pAttenuationFactor);
	//spot ctor
	Light(std::string pName, glm::vec3 pColor, glm::vec3 pPosition,float pIntensicy, glm::vec3 pSpotDirection, float pCutoff, float farCutoff, float pAttenuationFactor);

	virtual ~Light();

	LightType type;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;

	float intensity;

	float constAtten;
	float linearAtten;
	float quadAtten;

	float cutoff;
	float farCutoff;

	virtual void setParent(GameObject* pParent) override;

private:
};