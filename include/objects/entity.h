#ifndef ENTITY_H
#define ENTITY_H

#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <btBulletDynamicsCommon.h>

class Entity {


public:
	//TODO: add initial rotation quaternion

	Entity(glm::vec3 startingPosition, glm::vec3 scale = glm::vec3(1.0, 1.0, 1.0), glm::vec3 color = glm::vec3(1.0, 1.0, 1.0)) {
		setTransform(startingPosition, scale);

	}

	void setTransform(glm::vec3 translate, glm::vec3 scale) {

		glm::mat4 model = glm::mat4(1.0f);
		
		model = glm::translate(model, translate);
		model = glm::scale(model, scale);

		transformMatrix = model;
		this->scale = scale;
		this->position = translate;
		//std::cout << glm::to_string(transformMatrix) << std::endl;
	}

	void updateTransform(float matrix[16]) {
		glm::mat4 newTransform = glm::mat4(matrix[0], matrix[1], matrix[2], matrix[3],
			matrix[4], matrix[5], matrix[6], matrix[7],
			matrix[8], matrix[9], matrix[10], matrix[11],
			matrix[12], matrix[13], matrix[14], matrix[15]);
		newTransform = glm::scale(newTransform, scale);
		transformMatrix = newTransform;
	}

	glm::mat4 getTransform() {
		return transformMatrix;
	}

	btRigidBody* getRigidBody() {
		return this->rigidBody;
	}

	virtual void createRigidBody() {
		std::runtime_error("the current entity does not have a rigid body");
	}

protected:
	glm::mat4 transformMatrix;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 color;
	btRigidBody* rigidBody = nullptr;

private:

};

#endif