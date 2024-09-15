#ifndef POOL_BALLS
#define POOL_BALLS

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

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

	PoolSet(int numBalls, glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f)) : numBalls(numBalls), center(center) {
		setInitialPositions();
	}




};


#endif