#include "Alien.h"


Alien::Alien(btDiscreteDynamicsWorld* worldN,Terrain* terrianN,Camera* cameraN)
{
	translate = glm::vec3(0, 0, 0);
	rotate = glm::vec3(1.0f, 0.0f, 0.0f);
	terrain = terrianN;
	world = worldN;
	camera = cameraN;
	fragmentName = "fragment-shader[none].txt";
	float height = 5.0f*METER;
	GetVertices().push_back({ { -height / 4.0f, height, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 4.0f, height, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { -height / 4.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 4.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });

	GetIndices().push_back({ glm::uvec3(2, 0, 1)});
	GetIndices().push_back({ glm::uvec3(2, 1, 3) });



	textureName = "alien.png";


	shape = new btBoxShape(btVector3(0.1f*METER, 0.5f*METER, 0.1f*METER));

	Load(); //loads drawing related stuff. Call after vertices/indices have been defined
}
void Alien::Update(double){

}
glm::mat4 billboard(glm::vec3 position, glm::vec3 cameraPos, glm::vec3 cameraUp) {
	glm::vec3 look = normalize(cameraPos - position);
	glm::vec3 right = cross(cameraUp, look);
	glm::vec3 up2 = cross(look, right);
	glm::mat4 transform;
	transform[0] = glm::vec4(right, 0);
	transform[1] = glm::vec4(up2, 0);
	transform[2] = glm::vec4(look, 0);
	// Uncomment this line to translate the position as well
	// (without it, it's just a rotation)
	//transform[3] = vec4(position, 0);
	return transform;
}
void Alien::UpdatePosition(){
	position = glm::translate(glm::mat4(),glm::vec3(translate.x,terrain->GetHeight(0,0)+0.5f*METER,translate.z));

	glm::mat4 rotat = billboard(glm::vec3(translate.x, 0.0f, translate.z), glm::vec3(camera->position().x, 0.0f, camera->position().z), glm::vec3(0, 1, 0));

	//glm::vec2 ang1 = glm::normalize(glm::vec2(rotate.x, rotate.z));
	//glm::vec2 ang2 = glm::normalize(glm::vec2(camera->position().x - translate.x, camera->position().z-translate.z));
	//float angle = glm::atan(ang2.y - ang1.y, ang2.x - ang1.x)*glm::atan(ang2.y - ang1.y, ang2.x - ang1.x);
	position = position*rotat;
}
Alien::~Alien()
{
}
