#pragma once

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"

class PhysicsWorld
{
public:
	PhysicsWorld(float pX, float pY, float pZ);
	virtual ~PhysicsWorld();

	virtual void AddRigidBody(btRigidBody* pRigidBody);

	virtual btDiscreteDynamicsWorld* GetDynamicsWorld();

protected:
	btVector3 _gravity;
	//broadphase	
	btBroadphaseInterface* _broadphase;
	//collisionconfigurator
	btDefaultCollisionConfiguration* _collisionConfig;
	//dispatcher
	btCollisionDispatcher* _dispatcher;
	//solver
	btSequentialImpulseConstraintSolver* _solver;
	//dynamicsworld
	btDiscreteDynamicsWorld* _dynamicsWorld;

	virtual void initialize();
	virtual void update(float pStep);
};