#ifndef CONSTRAINT_ENTITY
#define CONSTRAINT_ENTITY

#include <objects/cube.h>
#include <scene/physicsManager.h>

class ConstraintEntity {

public:

	glm::vec3 position = position;
	Cube cube1 = Cube(glm::vec3(0, 5, 15), glm::vec3(1, 1, 1));
	Cube cube2 = Cube(glm::vec3(5, 7, 15), glm::vec3(1, 1, 1));

	ConstraintEntity(glm::vec3 position, PhysicsManager  *physicsManager) {


		physicsManager->addObject(&cube1);
		physicsManager->addObject(&cube2);
		setConstraints(physicsManager);
	}



private:

	void setConstraints(PhysicsManager* physicsManager) {
		const btVector3 pivotInA(2, 0, 0);       // Center of the object
		const btVector3 axisInA(0, 1, 0);
		const btVector3 pivotInB(-2, 0, 0);      // Hinge pivot on the second body
		const btVector3 axisInB(0, 1, 0);
		btHingeConstraint* hinge = new btHingeConstraint(*cube1.getRigidBody(), *cube2.getRigidBody(), pivotInA, pivotInB, axisInA, axisInB, true);
		hinge->setLimit(-SIMD_HALF_PI , SIMD_HALF_PI);
		physicsManager->addConstraint(hinge);
	}

};


#endif