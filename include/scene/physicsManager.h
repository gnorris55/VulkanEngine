#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include <btBulletDynamicsCommon.h>
#include <iostream>
#include <vector>

class PhysicsManager {

public:
	PhysicsManager() {
		init();
	}

    void addObject(Entity* object) {
        objects.push_back(object);
        object->createRigidBody();
        dynamicsWorld->addRigidBody(object->getRigidBody());
    }

    void addConstraint(btTypedConstraint* constraint) {
        dynamicsWorld->addConstraint(constraint);
    }


    void simulationLoop(double time_step, bool debugging = false) {
        // 5. Simulation loop
        dynamicsWorld->stepSimulation(time_step, 1);


        //if (debugging)
            //dynamicsWorld->debugDrawWorld();


        for (int i = 0; i < objects.size(); i++) {
            btTransform trans;
            btRigidBody* curr_body = objects[i]->getRigidBody();
            curr_body->getMotionState()->getWorldTransform(trans);
            btVector3 linear = trans.getOrigin();
            btQuaternion angular = trans.getRotation();

            float openGLMatrix[16];
            trans.getOpenGLMatrix(openGLMatrix);

            objects[i]->updateTransform(openGLMatrix);
        }
    }


private:
    std::vector<Entity*> objects;
    btDiscreteDynamicsWorld* dynamicsWorld;
    //PhysicsDebugger* physics_debugger;


	void init() {
        // 1. Initialize Bullet

        btBroadphaseInterface* broadphase = new btDbvtBroadphase();
        btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
        btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
        btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();


        // 2. Create the physics world
        this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
        dynamicsWorld->setGravity(btVector3(0, -9.81, 0));  // Set gravity

	}


};


#endif