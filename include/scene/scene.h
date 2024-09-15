#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

#include <scene/camera.h>
#include <scene/light.h>
#include <objects/poolBalls.h>

class Scene {



public:

    //TODO: probably going to be more lights
    Scene(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Camera *camera, Light *light) : camera(camera), light(light) {
        poolBalls = new PoolSet(1, glm::vec3(0, 1, 0));

    }

    ~Scene() {
        delete poolBalls;
    }

    void loadModels(VkDevice& device, VkCommandPool& commandPool, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue) {
        poolBalls->loadModel(device, commandPool, physicalDevice, graphicsQueue);
    }

    // where we add our buffers for our objects
    void addDrawingBuffers(VkCommandBuffer &commandBuffer, VkPipelineLayout &pipelineLayout, std::vector<VkDescriptorSet> &descriptorSets, uint32_t &currentFrame, VkExtent2D swapChainExtent, std::vector<void*>& uniformBuffersMapped) {

        VkBuffer vertexBuffers[] = { poolBalls->vertexBuffer, poolBalls->vertexBuffer};
        //VkBuffer indexBuffers[] = { indexBuffer };
        VkDeviceSize offsets[] = { 0 };

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, poolBalls->indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
                
        updateUniformBuffer(swapChainExtent, uniformBuffersMapped, currentFrame, glm::vec3(0, 0, 0));
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(poolBalls->indices.size()), 15, 0, 0, 0);

        //updateUniformBuffer(swapChainExtent, uniformBuffersMapped, currentFrame, glm::vec3(0, 3, 0));
        //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(poolBalls->indices.size()), 1, 0, 0, 0);
        //std::cout << poolBalls->indices.size() << std::endl;
    }
    
    void updateUniformBuffer(VkExtent2D swapChainExtent, std::vector<void*> &uniformBuffersMapped, uint32_t currentImage, glm::vec3 position) {

        static auto startTime = std::chrono::high_resolution_clock::now();

        auto currentTime = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

        UniformBufferObject ubo{};
        ubo.model = glm::translate(glm::mat4(1.0f), position);
        ubo.model = glm::rotate(ubo.model, time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        //ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        ubo.view = camera->GetViewMatrix();
        ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
        ubo.lightPos = glm::vec4(light->getPosition(), 1.0);
        ubo.cameraPos = glm::vec4(camera->Position, 1.0);
        glm::mat4 model = glm::mat4(1.0f);
        for(int i = 0; i < 15; i++)
            ubo.instanceTransform[i] = glm::translate(model, glm::vec3(i*2, 0, 0));


        ubo.proj[1][1] *= -1;

        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    void setScene(GLFWwindow* window) {
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;

        getFPS(frameCount, currentTime);
        processInput(window);

        lastTime = currentTime;

        //std::cout << glm::to_string(camera.Position) << std::endl;
    }

    void getFPS(int& frameCount, float currentTime) {
        float frameDeltaTime = currentTime - lastFrameTime;
        frameCount++;
        if (frameDeltaTime >= 1.0) {
            // Calculate FPS
            double fps = (double)frameCount / frameDeltaTime;

            // Display FPS
            std::cout << "fps: " << fps << "\n";
            // Reset for next calculation

            lastFrameTime = currentTime;
            frameCount = 0;
        }
    }

    void cleanUp(VkDevice& device) {

        poolBalls->cleanUp(device);
    }


    Camera * getCamera() {
        return camera;
    }
    
    Light * getLight() {
        return light;
    }

    VkBuffer getVertexBuffer() {
        return poolBalls->vertexBuffer;
    }

private:

	Camera *camera;
    Light* light;
    PoolSet* poolBalls;

    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    int frameCount = 0;
    float lastFrameTime = 0;
    glm::vec3 processInput(GLFWwindow* window)
    {

       
        float movement_speed = deltaTime * 5.0;
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->forward_mv = true;
        else if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE)
            camera->forward_mv = false;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->back_mv = true;
        else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE)
            camera->back_mv = false;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->left_mv = true;
        else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE)
            camera->left_mv = false;

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->right_mv = true;
        else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE)
            camera->right_mv = false;

        if (camera->forward_mv)
            camera->ProcessKeyboard(FORWARD, movement_speed);
        if (camera->back_mv)
            camera->ProcessKeyboard(BACKWARD, movement_speed);
        if (camera->left_mv)
            camera->ProcessKeyboard(LEFT, movement_speed);
        if (camera->right_mv)
            camera->ProcessKeyboard(RIGHT, movement_speed);

        return glm::vec3(0, 0, 0);
    }


	


};

#endif