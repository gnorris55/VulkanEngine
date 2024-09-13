#define STB_IMAGE_IMPLEMENTATION
//#include <stb/stb_image.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <vector>
#include <iostream>
#include <triangleApplication.h>


glm::vec3 lightPos = glm::vec3(0, 5, 0);

int main () {

	int texWidth, texHeight, texChannels;
	unsigned char* pixels = stbi_load(RESOURCES_PATH"textures/texture.jpg", &texWidth, &texHeight, &texChannels, 0);
	TriangleApplication app;
	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
