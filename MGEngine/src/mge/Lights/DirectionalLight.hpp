#pragma once
#include <string>
#include "mge/Lights/Light.hpp"

/**
* Camera is just a GameObject with an additional projection matrix.
* The camera's own transform is used to generate a world to view matrix by taking the inverse of the camera transform
* (see renderer).
*/
class DirectionalLight : public Light
{
public:
	DirectionalLight(
		std::string pName = "directionalLight",
		glm::vec3 pPosition = glm::vec3(1.0f, 1.0f, 0.0f),
		glm::vec3 pLightColor = glm::vec3(1, 1, 1),
		float pLightIntensity = 1
		);

	virtual ~DirectionalLight();

private:
};