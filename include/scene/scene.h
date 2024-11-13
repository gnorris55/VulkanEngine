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
#include <objects/cube.h>
#include <objects/plane.h>
#include <objects/poolCue.h>
#include <scene/physicsManager.h>
#include <objects/constraintEntity.h>


class Scene {

public:

    //TODO: probably going to be more lights
    Scene(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Camera *camera, Light *light) : camera(camera), light(light) {
        poolSet = new PoolSet(15, glm::vec3(0, 2, 0));
        
        addPhysicsObjects();
    }


    ~Scene() {
        delete poolSet;
    }

    void loadModels(VkDevice& device, VkCommandPool& commandPool, VkPhysicalDevice& physicalDevice, VkQueue& graphicsQueue) {
        poolSet->loadModel(device, commandPool, physicalDevice, graphicsQueue);
        plane.loadModel(device, commandPool, physicalDevice, graphicsQueue);
        poolCue.loadModel(device, commandPool, physicalDevice, graphicsQueue);
        cube.loadModel(device, commandPool, physicalDevice, graphicsQueue);
    }

    // where we add our buffers for our objects
    void draw(VkCommandBuffer &commandBuffer, VkPipelineLayout &pipelineLayout, std::vector<VkDescriptorSet> &descriptorSets, uint32_t &currentFrame, VkExtent2D swapChainExtent, std::vector<void*>& uniformBuffersMapped) {

        UniformBufferObject ubo{};
        ubo.model = glm::mat4(1.0f);
        ubo.view = camera->GetViewMatrix();
        ubo.proj = camera->GetProjection();
        ubo.lightPos = glm::vec4(light->getPosition(), 1.0);
        ubo.cameraPos = glm::vec4(camera->Position, 1.0);

        // TODO: change vertex buffer implementation, opengl way is slow
        VkBuffer vertexBuffers[] = { poolSet->vertexBuffer};
        VkBuffer vertexBuffers2[] = { plane.vertexBuffer};
        VkBuffer vertexBuffers3[] = { poolCue.vertexBuffer};
        VkBuffer vertexBuffers4[] = { cube.vertexBuffer };

        VkDeviceSize offsets[] = {0};

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentFrame], 0, nullptr);
        updateUniforms(ubo, swapChainExtent, uniformBuffersMapped, currentFrame, glm::vec3(0, 0, 0));

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, poolSet->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(poolSet->indices.size()), 16, 0, 0, 0);

        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers4, offsets);
        vkCmdBindIndexBuffer(commandBuffer, cube.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(cube.indices.size()), 2, 0, 0, 16);

        //vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets);
        //vkCmdBindIndexBuffer(commandBuffer, plane.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(plane.indices.size()), 1, 0, 0, 16);
        
        //vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers3, offsets);
        //vkCmdBindIndexBuffer(commandBuffer, poolCue.indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(poolCue.indices.size()), 1, 0, 0, 16);

    }

    void updateUniforms(UniformBufferObject ubo, VkExtent2D swapChainExtent, std::vector<void*> &uniformBuffersMapped, uint32_t currentImage, glm::vec3 position) {

                 
        glm::mat4 model = glm::mat4(1.0f);
        for (int i = 0; i < poolSet->poolBalls.size(); i++) {
            PoolBall poolBall = poolSet->poolBalls[i];

            ubo.instanceTransform[i] = poolBall.getTransform();

        }
        ubo.instanceTransform[15] = poolSet->cueBall->getTransform();
        ubo.instanceTransform[16] = constrainEntity.cube1.getTransform();
        ubo.instanceTransform[17] = constrainEntity.cube2.getTransform();
        ubo.proj[1][1] *= -1;

        memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
    }

    void setScene(GLFWwindow* window) {
        float currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;

        getFPS(frameCount, currentTime);

        //update physics
        processInput(window);

        simulatePhysics(currentTime, 1/60.0f);

        lastTime = currentTime;
        //std::cout << glm::to_string(camera.Position) << std::endl;
    }


    void cleanUp(VkDevice& device) {

        poolSet->cleanUp(device);
        plane.cleanUp(device);
        poolCue.cleanUp(device);
        cube.cleanUp(device);
    }


    Camera * getCamera() {
        return camera;
    }
    
    Light * getLight() {
        return light;
    }

    VkBuffer getVertexBuffer() {
        return poolSet->vertexBuffer;
    }
    
    VkBuffer getPlaneVertexBuffer() {
        return plane.vertexBuffer;
    }

private:

    PhysicsManager physicsManager = PhysicsManager();
	Camera *camera;
    Light* light;
    PoolSet* poolSet;
    Plane plane = Plane(glm::vec3(0, 1, 0), glm::vec3(0.0, 1, 0));
    PoolCue poolCue = PoolCue(glm::vec3(0, 1, 0));

    Cube cube = Cube(glm::vec3(0, 5, 15), glm::vec3(1, 1, 1));
    ConstraintEntity constrainEntity = ConstraintEntity(glm::vec3(1, 1, 1), &physicsManager);

    bool shot = false;

    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    int frameCount = 0;
    float lastFrameTime = 0;
    int nbFrames = 0;
    float accumulator = 0.0f;

    void addPhysicsObjects() {
        for (int i = 0; i < poolSet->numBalls; i++) {
            physicsManager.addObject(&(poolSet->poolBalls[i]));
        }
        physicsManager.addObject(poolSet->cueBall);
        physicsManager.addObject(&plane);
        //physicsManager.addObject(&cube);

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

    
    void simulatePhysics(float time, float freq) {

        nbFrames++;
        float deltaTime = time - lastTime;
        //lastTime = time;
        accumulator += deltaTime;
        //int count = 0;
        //std::cout << "count: " << count++ << "\n";
        while (accumulator >= freq) {
            //std::cout << "acc: " << accumulator << "\n";
            physicsManager.simulationLoop(freq);

            nbFrames = 0;
            accumulator -= freq;
        }
    }
    


    glm::vec3 processInput(GLFWwindow* window)
    {


        float movement_speed = deltaTime * 5.0;
        // TODO: fix physics not adding force when we are adding force
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !shot) {
            shot = true;
            std::cout << "we have a SHOT" << std::endl;
            poolSet->cueBall->addForce(glm::vec3(0, 0, 400));

        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE && shot) {
            shot = false;
        }

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