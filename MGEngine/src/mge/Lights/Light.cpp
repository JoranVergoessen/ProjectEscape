#include <iostream>
using namespace std;

#include <glm.hpp>
#include "mge/Lights/Light.hpp"
#include "mge/core/World.hpp"

//directional light ctor
Light::Light(std::string pName, glm::vec3 pColor, glm::vec3 pPosition,float pIntensity, glm::vec3 pDirection) : GameObject(pName, pPosition)
{
	type = Light::directional;
	this->SetDirection(pDirection);
	ambientColor = pColor * 0.2f;
	diffuseColor = pColor * 0.5f;
	specularColor = pColor;
	intensity = pIntensity;
}

//point ctor
Light::Light(std::string pName, glm::vec3 pColor, glm::vec3 pPosition,float pIntensity, float pAttenuationFactor = 1.0f) : GameObject(pName, pPosition)
{
	type = Light::point;
	ambientColor = pColor * 0.1f;
	diffuseColor = pColor * 0.5f;
	specularColor = pColor;
	constAtten = 1.0f;
	linearAtten = 0.007f * pAttenuationFactor;
	quadAtten = 0.0002f * pAttenuationFactor;
	intensity = pIntensity;
}

//spot ctor
Light::Light(std::string pName, glm::vec3 pColor, glm::vec3 pPosition,float pIntensity, glm::vec3 pSpotDirection, float pCutoff, float pFarCutoff, float pAttenuationFactor = 1.0f) : GameObject(pName, pPosition)
{
	type = Light::spot;
	this->SetDirection(pSpotDirection);
	ambientColor = pColor * 0.1f;
	diffuseColor = pColor * 0.5f;
	specularColor = pColor;
	constAtten = 1.0f;
	linearAtten = 0.007f * pAttenuationFactor;
	quadAtten = 0.0002f * pAttenuationFactor;
	cutoff = glm::cos(glm::radians(pCutoff));
	farCutoff = glm::cos(glm::radians(pFarCutoff));
	intensity = pIntensity;
}

Light::~Light()
{
}

void Light::setParent(GameObject* pParent)
{
	GameObject::setParent(pParent);

	if (pParent == NULL) World::RemoveLight(this);
	else World::AddLight(this);
}


