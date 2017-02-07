#include "Project3.h"
#include <glm.hpp>
#include <iostream>
#include <string>
using namespace std;

//		core
#include "mge/core/Renderer.hpp"
#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"
//util
#include "mge/util/DebugHud.hpp"
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

Project3::Project3() :AbstractGame(), _hud(0)
{
}

void Project3::initialize() 
{
	AbstractGame::initialize();

	cout << "Initializing HUD" << endl;
	_hud = new DebugHud(_window);
	cout << "HUD initialized." << endl << endl;
}

void Project3::_initializeScene()
{
	//_renderer->setClearColor(1, 0, 1);

	Camera* mainCamera = new Camera("mainCamera", glm::vec3(0, 2, 5));
	_world->add(mainCamera);
	_world->setMainCamera(mainCamera);
	//TODO camera behaviour here

}

void Project3::_render()
{
	AbstractGame::_render();
	_updateHud();
}

void Project3::_updateHud()
{
	string debugInfo = "";
	debugInfo += string("FPS: ") + std::to_string((int)_fps) + "\n";
}

Project3::~Project3()
{
}
