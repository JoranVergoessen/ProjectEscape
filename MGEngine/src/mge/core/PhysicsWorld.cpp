#include "PhysicsWorld.hpp"
#include <iostream>

PhysicsWorld::PhysicsWorld(float pX, float pY, float pZ):
	_broadphase(NULL),_collisionConfig(NULL), _dispatcher(NULL), _solver(NULL), _dynamicsWorld(NULL)
{
	_gravity = btVector3(pX,pY,pZ);
	initialize();
}

PhysicsWorld::~PhysicsWorld()
{
	//DELETE
}


void PhysicsWorld::initialize() 
{
	std::cout << "Initializing dynamics world..." << std::endl;
	_broadphase = new btDbvtBroadphase();
	_collisionConfig = new btDefaultCollisionConfiguration();
	_dispatcher = new btCollisionDispatcher(_collisionConfig);
	_solver = new btSequentialImpulseConstraintSolver();
	_dynamicsWorld = new btDiscreteDynamicsWorld(_dispatcher, _broadphase, _solver, _collisionConfig);
	_dynamicsWorld->setGravity(_gravity);
	std::cout << " Dynamics world initiated!" << std::endl;
}

void PhysicsWorld::AddRigidBody(btRigidBody* pRigidBody)
{
	_dynamicsWorld->addRigidBody(pRigidBody);
}

btDiscreteDynamicsWorld* PhysicsWorld::GetDynamicsWorld()
{
	return _dynamicsWorld;
}
void PhysicsWorld::Update(float pStep, int pMaxTimeSteps)
{
	_dynamicsWorld->stepSimulation(pStep, pMaxTimeSteps);
	//std::cout<< "number of collision objects = "<<_dynamicsWorld->getNumCollisionObjects() << std::endl;
}
