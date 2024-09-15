#ifndef POOL_BALLS
#define POOL_BALLS

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <dataStructures.h>
#include <dataProcessing.h>


class PoolBall {

private:

	glm::mat4 transformMatrix = glm::mat4(1.0f);

public:
	PoolBall(glm::vec3 initialPosition) {
		this->transformMatrix = glm::translate(transformMatrix, initialPosition);
		std::cout << "STARTING POSITION!: " << glm::to_string(initialPosition) << std::endl;
	}

};


class PoolSet {

private:

	const std::string MODEL_PATH = RESOURCES_PATH"models/sphere/sphere.obj";
	//const std::string TEXTURE_PATH = RESOURCES_PATH"models/sphere/poolBallTextures/Ball9.jpg";
	int numBalls;
	glm::vec3 center;
	std::vector<PoolBall> poolBalls;

	


	void setInitialPositions() {
		for (int i = 0; i < numBalls; i++) {
			poolBalls.push_back(PoolBall(center));
		}
	}



	//void loadModel() {

	//}

public:
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	PoolSet(int numBalls, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f)) : numBalls(numBalls), center(center) {
		setInitialPositions();
	}

	void cleanUp(VkDevice &device) {
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


};


#endif