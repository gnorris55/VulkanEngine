#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <dataStructures.h>
#include <dataProcessing.h>
#include <objects/entity.h>

class Plane : public Entity {

public:
	glm::vec3 normal;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	Plane(glm::vec3 position, glm::vec3 normal) : Entity(position, glm::vec3(3, 3, 3)) {
		this->normal = normal;

	}

	void createRigidBody() override {
		btCollisionShape* planeShape = new btStaticPlaneShape(LMath::glmToBt(normal), 0);

		// Step 3: Set up the motion state (using the identity matrix for position and orientation)
		btDefaultMotionState* planeMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));

		// Step 4: Define mass and inertia (mass is 0 for static objects)
		btScalar mass = 0;  // Static objects have 0 mass
		btVector3 inertia(0, 0, 0);

		// Step 5: Create the rigid body
		btRigidBody::btRigidBodyConstructionInfo planeRigidBodyCI(mass, planeMotionState, planeShape, inertia);
		this->rigidBody = new btRigidBody(planeRigidBodyCI);
		//this->rigidBody = new btRigidBody(fallRigidBodyCI);
	}

	void cleanUp(VkDevice& device) {
		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);

		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
	}

	void loadModel(VkDevice& device, VkCommandPool& commandPool, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue) {

		Loader::loadModel(vertices, indices, MODEL_PATH);
		Loader::createVertexBuffer(vertexBuffer, vertexBufferMemory, vertices, device, commandPool, physicalDevice, graphicsQueue);
		Loader::createIndexBuffer(indexBuffer, indexBufferMemory, indices, device, commandPool, physicalDevice, graphicsQueue);
	}

private:
	const std::string MODEL_PATH = RESOURCES_PATH"models/plane/plane.obj";


	




};


#endif