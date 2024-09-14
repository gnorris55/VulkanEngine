#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

class Light {

private:

	glm::vec3 position;
	glm::vec3 color;

public:

	Light(glm::vec3 position, glm::vec3 color) : position(position), color(color) {}

	glm::vec3 getPosition() {
		return position;
	}
	
	void setPosition(glm::vec3 newPosition) {
		position = newPosition;
	}



};


#endif