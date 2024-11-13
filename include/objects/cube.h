#ifndef CUBE_H
#define CUBE_H

#include <objects/entity.h>

class Cube : public Entity {

public:

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	Cube(glm::vec3 initialPosition, glm::vec3 initialScale = glm::vec3(1, 1, 1)) : Entity(initialPosition, initialScale) {
		//std::cout << "STARTING POSITION!: " << glm::to_string(initialPosition) << std::endl;
	}

	void createRigidBody() override {
		btCollisionShape* fallShape = new btBoxShape(LMath::glmToBt(scale));  // Box of size 1x1x1
		btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), LMath::glmToBt(position)));
		
		btScalar mass = 1;

		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		this->rigidBody = new btRigidBody(fallRigidBodyCI);
		//this->rigidBody->setFriction(0.02f);
		//this->rigidBody->setRollingFriction(0.03f);
	}

	void loadModel(VkDevice& device, VkCommandPool& commandPool, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue) {

		Loader::loadModel(vertices, indices, MODEL_PATH);
		Loader::createVertexBuffer(vertexBuffer, vertexBufferMemory, vertices, device, commandPool, physicalDevice, graphicsQueue);
		Loader::createIndexBuffer(indexBuffer, indexBufferMemory, indices, device, commandPool, physicalDevice, graphicsQueue);
	}

	void cleanUp(VkDevice& device) {
		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);

		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
	}
private:
	const std::string MODEL_PATH = RESOURCES_PATH"models/cube/cube.obj";

};


#endif