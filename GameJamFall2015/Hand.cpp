#include "Hand.h"


Hand::Hand(btDiscreteDynamicsWorld* worldN)
{

	world = worldN;

	float height = METER*1.0f;
	ExtractFromFile("Box.obj");
	NormalizeScale(glm::vec3(METER));


	textureName = "dirt.jpg";
	shape = new btBoxShape(btVector3(height / 2.0f, height / 2.0f, height / 2.0f));

	Load();
}

void Hand::Update(double dt){

	Object::Update(dt);
}
Hand::~Hand()
{
}
