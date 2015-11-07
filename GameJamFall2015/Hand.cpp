#include "Hand.h"


Hand::Hand(btDiscreteDynamicsWorld* worldN)
{

	world = worldN;

	position = glm::scale(glm::mat4(), glm::vec3(METER));

	float height = METER*1.0f;
	ExtractFromFile("Box.obj");
	textureName = "dirt.jpg";
	shape = new btBoxShape(btVector3(height / 2.0f, height / 2.0f, height / 2.0f));

	Load();
}


Hand::~Hand()
{
}
