#include "mge/behaviours/OrbitBehaviour.h"
#include <glm.hpp>
#include "mge/core/GameObject.hpp"
#include <SFML\Window\Keyboard.hpp>
#include <SFML\Graphics.hpp>


sf::Vector2i _oldMousePos = sf::Mouse::getPosition();

OrbitBehaviour::OrbitBehaviour(GameObject* pTarget): AbstractBehaviour(), _target(pTarget)
{
	_targetLocation = _target->getWorldPosition();
}

OrbitBehaviour::OrbitBehaviour(glm::vec3 pTargetLocation) : AbstractBehaviour()
{
	_targetLocation = pTargetLocation;
}


OrbitBehaviour::~OrbitBehaviour()
{
}

void OrbitBehaviour::update(float pStep)
{
	Zoom();
	MouseInput(pStep);
	UpdateTransform();
}



void OrbitBehaviour::Zoom()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))_orbitDistance += glm::vec3(0, 0, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))_orbitDistance -= glm::vec3(0, 0, 1);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))_orbitDistance += glm::vec3(0, 1, 0);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))_orbitDistance -= glm::vec3(0, 1, 0);
}


void OrbitBehaviour::MouseInput(float pStep) 
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		/**
		//onKeyDown
		if (!_isRmbPressed)
		{
			_onPressPosX = sf::Mouse::getPosition().x;
			_onPressPosY = sf::Mouse::getPosition().y;
			std::cout << "rmb pressed at: " << _onPressPosX << "    " << _onPressPosY << std::endl;
			_isRmbPressed = true;
		}
		//onKeyStay
		int deltaX = sf::Mouse::getPosition().x - _onPressPosX;
		int deltaY = sf::Mouse::getPosition().y - _onPressPosY;
		/**
		int xSpeedMultiplier = deltaY / _triggerDistance;
		int ySpeedMultiplier = deltaX / _triggerDistance;

		_angleY += _orbitSpeed*pStep*ySpeedMultiplier;
		_angleX += _orbitSpeed*pStep*xSpeedMultiplier;
		/**/
		if (!_isRmbPressed)
		{
			_oldMousePos = sf::Mouse::getPosition();
			_isRmbPressed = true;
			//std::cout << "rmb pressed at: " << _onPressPosX << "    " << _onPressPosY << std::endl;
		}

		sf::Vector2i delta = sf::Mouse::getPosition() - _oldMousePos;
		_angleY += delta.x / 360.0f;
		_angleX += delta.y / 360.0f;
		if (_oldMousePos != sf::Mouse::getPosition())_oldMousePos = sf::Mouse::getPosition();
	}
	else if (_isRmbPressed) _isRmbPressed = false;
}

void OrbitBehaviour::UpdateTransform()
{
	if (_target != NULL)_targetLocation = _target->getWorldPosition();
	glm::mat4 myTransform = _owner->getTransform();

	myTransform =
		glm::translate(_targetLocation)*				//orbit relative to the target position
		glm::rotate(_angleY, glm::vec3(0, 1, 0))*		//rotation around Y
		glm::rotate(_angleX, glm::vec3(1, 0, 0))*		//rotation around X
		glm::translate(_orbitDistance);					//orbit distance

	_owner->setTransform(myTransform);
}

