#include "Hand.h"


Hand::Hand(btDiscreteDynamicsWorld* worldN)
{

	world = worldN;

	position = glm::translate(glm::mat4(),glm::vec3(0,5*METER,0));

	float height = METER*1.0f;
	ExtractFromFile("Box.obj");
	NormalizeScale(glm::vec3(METER));


	textureName = "dirt.jpg";
	shape = new btBoxShape(btVector3(height, height, height));

	Load();
}

void Hand::Update(double dt){

	Object::Update(dt);
}
Hand::~Hand()
{
}
