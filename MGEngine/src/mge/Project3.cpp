#include "Project3.h"
#include "mge/config.hpp"
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
#include "mge/core/PhysicsWorld.hpp"
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
	Mesh* sphere = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");

	Camera* mainCamera = new Camera("mainCamera", glm::vec3(0, 2, 5));
	_world->add(mainCamera);
	_world->setMainCamera(mainCamera);
	//TODO camera behaviour here

	PhysicsWorld* physicsWorld = new PhysicsWorld(0,-10,0);

	/*======================================================================================================================================================*/
	//shapes
	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	btCollisionShape* fallShape = new btSphereShape(1);

	//ground rigid body

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

	physicsWorld->GetDynamicsWorld()->addRigidBody(groundRigidBody);

	//falling object rigid body

	btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 50, 0)));
	btScalar mass = 1;
	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);

	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	physicsWorld->GetDynamicsWorld()->addRigidBody(fallRigidBody);

	//stepping
	for (int i = 0; i < 300; i++) 
	{

		physicsWorld->GetDynamicsWorld()->stepSimulation(1 / 60.f, 10);

		btTransform trans;
		fallRigidBody->getMotionState()->getWorldTransform(trans);

		std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
	}
	/*======================================================================================================================================================*/
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
