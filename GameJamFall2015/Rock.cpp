#include "Rock.h"


Rock::Rock(btDiscreteDynamicsWorld* worldN,Terrain* terrainN)
{
	isGhost = true;
	isStatic = true;

	world = worldN;
	terrain = terrainN;


	std::uniform_real_distribution<float> distro(-KILOMETER*0.4f, KILOMETER *0.4f);
	float x = GetDistribution(distro);
	float y = GetDistribution(distro);
	position = glm::translate(glm::mat4(), glm::vec3(x, terrain->GetHeight(x,y), y));


	
	ExtractFromFile("rock1.obj");
	NormalizeScale(glm::vec3(2.0f*METER));

	textureName = "dirt.jpg";

	shape = new btBoxShape(btVector3(0.1f*METER, 0.5f*METER, 0.1f*METER));

	Load(); //loads drawing related stuff. Call after vertices/indices have been defined
}


Rock::~Rock()
{
}
