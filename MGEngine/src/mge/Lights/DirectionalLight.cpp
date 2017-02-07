#include <iostream>
using namespace std;

#include <glm.hpp>
#include "mge/Lights/DirectionalLight.hpp"

DirectionalLight::DirectionalLight(std::string pName, glm::vec3 pPosition, glm::vec3 pLightColor, float pLightIntensity)
	: Light(pName, pPosition, pLightColor, pLightIntensity)
{
}

DirectionalLight::~DirectionalLight()
{
	//dtor
}

