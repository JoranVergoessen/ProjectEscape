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
//physics
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
//material
#include <mge/materials/AbstractMaterial.hpp>
#include <mge/materials/LitTextureMaterial.hpp>
#include <mge/materials/TextureMaterial.hpp>

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
	SetTargetFps(120.0f);
	//_renderer->setClearColor(1, 0, 1);

	PhysicsWorld* physicsWorld = new PhysicsWorld(0,-10,0);
	AddPhysicsWorld(physicsWorld);

	Texture* terrainSnow = Texture::load(config::MGE_TEXTURE_PATH + "terrain/diffuse4.jpg");
	Texture* waterTexture = Texture::load(config::MGE_TEXTURE_PATH + "terrain/water.jpg");

	AbstractMaterial* snowMat = new TextureMaterial(terrainSnow);
	AbstractMaterial* waterMat = new TextureMaterial(waterTexture);

	Mesh* sphereMesh = Mesh::load(config::MGE_MODEL_PATH + "sphere_smooth.obj");
	Mesh* planeMesh = Mesh::load(config::MGE_MODEL_PATH + "plane_8192.obj");

	Camera* mainCamera = new Camera("mainCamera", glm::vec3(0, 2, 15));
	world->add(mainCamera);
	world->setMainCamera(mainCamera);
	//TODO camera behaviour here

	GameObject* plane = new GameObject("plane", glm::vec3(0, 0, 0));
	plane->scale(glm::vec3(10, 10, 10));
	plane->setMesh(planeMesh);
	plane->setMaterial(snowMat);
	world->add(plane);

	GameObject* sphere = new GameObject("sphere", glm::vec3(0, 5, 0));
	sphere->setMesh(sphereMesh);
	sphere->setMaterial(waterMat);
	world->add(sphere);

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
	_hud->setDebugInfo(debugInfo);
	_hud->draw();
}

Project3::~Project3()
{
}
