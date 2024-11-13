#ifndef POOL_CUE_H
#define POOL_CUE_H

#include <objects/entity.h>

class PoolCue: public Entity {

public: 
	glm::vec3 normal;
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	PoolCue(glm::vec3 position) : Entity(position) {


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
	const std::string MODEL_PATH = RESOURCES_PATH"models/poolCue/poolCue.obj";

};


#endif