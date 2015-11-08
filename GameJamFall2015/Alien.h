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

	~Alien();
private:
	glm::vec3 translate;
	glm::vec3 rotate;
	Camera* camera;
	Terrain* terrain;
};

