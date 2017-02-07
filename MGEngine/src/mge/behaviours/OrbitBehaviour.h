#pragma once
#include "mge/behaviours/AbstractBehaviour.hpp"
#include <glm\glm.hpp>
#include <SFML\Graphics.hpp>


class OrbitBehaviour :
	public AbstractBehaviour
{
public:
	OrbitBehaviour(GameObject* pTarget);
	OrbitBehaviour(glm::vec3 pTargetLocation);
	virtual ~OrbitBehaviour();

	virtual void update(float pStep);

private:
	GameObject* _target;
	glm::vec3 _targetLocation;

	glm::vec3 _orbitDistance = glm::vec3(0, 2, 10);
	float _angleX = 0.0f;
	float _angleY = 0.0f;
	float _orbitSpeed = 1.0f;
	bool _isRmbPressed = false;
	int _onPressPosX = 0;
	int _onPressPosY = 0;
	int _triggerDistance = 100;


	void UpdateTransform();
	void MouseInput(float pStep);
	void Zoom();
};

