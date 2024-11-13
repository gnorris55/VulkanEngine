#ifndef POOL_BALLS
#define POOL_BALLS

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <dataStructures.h>
#include <dataProcessing.h>
#include <objects/entity.h>


class PoolBall : public Entity {

private:

	//glm::mat4 transformMatrix = glm::mat4(1.0f);

public:
	PoolBall(glm::vec3 initialPosition) : Entity(initialPosition)  {
		//std::cout << "STARTING POSITION!: " << glm::to_string(initialPosition) << std::endl;
	}

	void createRigidBody() override {
		btCollisionShape* fallShape = new btSphereShape(1.0);  // Box of size 1x1x1
		btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), LMath::glmToBt(position)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		this->rigidBody = new btRigidBody(fallRigidBodyCI);
		//this->rigidBody->setFriction(0.02f);
		//this->rigidBody->setRollingFriction(0.03f);
	}

};

class CueBall : public PoolBall {
public:
	CueBall(glm::vec3 initialPosition) : PoolBall(initialPosition) {
		//std::cout << "STARTING POSITION!: " << glm::to_string(initialPosition) << std::endl;
	}

	void addForce(glm::vec3 force) {
		this->rigidBody->applyCentralForce(LMath::glmToBt(force));
	}

	void createRigidBody() override {
		btCollisionShape* fallShape = new btSphereShape(1.0);  // Box of size 1x1x1
		btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), LMath::glmToBt(position)));
		btScalar mass = 1;
		btVector3 fallInertia(0, 0, 0);
		fallShape->calculateLocalInertia(mass, fallInertia);
		btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
		this->rigidBody = new btRigidBody(fallRigidBodyCI);
		this->rigidBody->setActivationState(DISABLE_DEACTIVATION);
	}

private:

};


class PoolSet {

private:

	const std::string MODEL_PATH = RESOURCES_PATH"models/sphere/sphere.obj";
	

	//const std::string TEXTURE_PATH = RESOURCES_PATH"models/sphere/poolBallTextures/Ball9.jpg";
	glm::vec3 center;

	void setInitialPositions(std::vector<glm::vec3> positions) {
		for (int i = 0; i < numBalls; i++) {
			poolBalls.push_back(PoolBall(center+positions[i]));
		}
	}

	std::vector<glm::vec3> generateTrianglePositions(int rows) {
		std::vector<glm::vec3> positions;
		float radius = 1.0f;
		float spacing = 2.0f; // Distance between centers of balls
		float verticalOffset = sqrt(3.0f); // Vertical offset between rows

		for (int row = 0; row < rows; ++row) {
			float y = -row * verticalOffset; // Vertical position of the row
			int numBallsInRow = row + 1;

			// Horizontal starting position, centered
			float startX = -((numBallsInRow - 1) * spacing) / 2.0f;

			for (int i = 0; i < numBallsInRow; ++i) {
				float x = startX + i * spacing;
				positions.push_back(glm::vec3(x, 0.0f, -y)); // Position in 2D (x, y), Z = 0
			}
		}

		return positions;
	}

public:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<PoolBall> poolBalls;
	CueBall *cueBall;
	
	int numBalls;

	PoolSet(int numBalls, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f)) : numBalls(numBalls), center(center) {
		std::vector<glm::vec3> initialPositions = generateTrianglePositions(5);
		setInitialPositions(initialPositions);
		cueBall = new CueBall(center + glm::vec3(0, 0, -5));
	}

	void cleanUp(VkDevice &device) {
		vkDestroyBuffer(device, indexBuffer, nullptr);
		vkFreeMemory(device, indexBufferMemory, nullptr);

		vkDestroyBuffer(device, vertexBuffer, nullptr);
		vkFreeMemory(device, vertexBufferMemory, nullptr);
		delete cueBall;
	}

	void loadModel(VkDevice& device, VkCommandPool& commandPool, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue) {
		
		Loader::loadModel(vertices, indices, MODEL_PATH);
		Loader::createVertexBuffer(vertexBuffer, vertexBufferMemory, vertices, device, commandPool, physicalDevice, graphicsQueue);
		Loader::createIndexBuffer(indexBuffer, indexBufferMemory, indices, device, commandPool, physicalDevice, graphicsQueue);
	}


};


#endif