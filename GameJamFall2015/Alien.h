#pragma once
#include "BasicIncludes.h"
#include "Object.h"
#include "Terrain.h"
#include "Camera.h"
class Alien :public Object
{
public:
	Alien(btDiscreteDynamicsWorld*, Terrain*, Camera*, irrklang::ISoundEngine*,uint);

	void Update(double);
	void UpdatePosition();

	~Alien();
private:
	uint species;
	float timer;
	irrklang::ISoundEngine* sound;
	glm::vec3 translate;
	glm::vec3 rotate;
	Camera* camera;
	Terrain* terrain;
};

