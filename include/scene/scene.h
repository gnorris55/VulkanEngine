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

private:

	Camera *camera;
    Light* light;
    PoolSet* poolBalls;

    float deltaTime = 0.0f;
    float lastTime = 0.0f;
    int frameCount = 0;
    float lastFrameTime = 0;


public:

    //TODO: probably going to be more lights
    Scene(unsigned int SCR_WIDTH, unsigned int SCR_HEIGHT, Camera *camera, Light *light) : camera(camera), light(light) {
        poolBalls = new PoolSet(1, glm::vec3(0, 1, 0));

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




    Camera * getCamera() {
        return camera;
    }
    
    Light * getLight() {
        return light;
    }

	


};

#endif