#pragma once
#include "BasicIncludes.h"
#include "Object.h"
#include "Terrain.h"
#include "Camera.h"
class Alien :public Object
{
public:
	Alien(btDiscreteDynamicsWorld*,Terrain*,Camera*);

	void Update(double);
	void UpdatePosition();

	glm::vec3 alienMovement(glm::vec3, glm::vec3);

	float prevAngle;
	float prevVeloc;

	~Alien();
private:
	glm::vec3 translate;
	glm::vec3 rotate;
	Camera* camera;
	Terrain* terrain;
};

