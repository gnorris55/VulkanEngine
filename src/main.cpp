#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
//#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <vector>
#include <iostream>
#include <triangleApplication.h>
#include <scene/camera.h>
#include <scene/scene.h>


void initWindow();
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;

glm::vec3 lightPos = glm::vec3(0, 5, 0);
Camera camera(glm::vec3(0.0f, 15.0f, -15.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, glm::vec3(0, 1, 0), 45.0f, 0.0f);

GLFWwindow* window;

int main () {

	initWindow();

	Light light = Light(glm::vec3(0, 5, 0), glm::vec3(1.0, 0.0, 0.0));
	Scene scene = Scene(WINDOW_WIDTH, WINDOW_HEIGHT, &camera, &light);

	int texWidth, texHeight, texChannels;
	unsigned char* pixels = stbi_load(RESOURCES_PATH"textures/texture.jpg", &texWidth, &texHeight, &texChannels, 0);
	VulkanApplication app(window, &scene);
	glfwSetCursorPosCallback(window, mouse_callback);

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

void initWindow() {

	if (enableValidationLayers) {
		std::cout << "in debug mode\n";
	}
	glfwInit();
	// we need to tell glfw that we are not using opengl
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan", nullptr, nullptr);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}


