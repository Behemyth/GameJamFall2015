#include "Alien.h"

const float TIMEMAX = 15.0f;
Alien::Alien(btDiscreteDynamicsWorld* worldN, Terrain* terrianN, Camera* cameraN, irrklang::ISoundEngine* soundN, uint speciesN)
{
	float radius = 0.4*KILOMETER;
	species = speciesN;
	std::normal_distribution<float> distro78(0.0f,5.0f);
	timer = GetDistribution(distro78);
	sound = soundN;
	
	prevAngle = 0.0f;
	prevVeloc = 0.0f;
	std::normal_distribution<float> startDistro(0.0f, 100*METER);
	translate = glm::vec3(GetDistribution(startDistro), GetDistribution(startDistro), GetDistribution(startDistro));
	//translate = glm::vec3(0.0f, 0.0f, 0.0f);

	rotate = glm::vec3(1.0f, 0.0f, 0.0f);
	terrain = terrianN;
	world = worldN;
	camera = cameraN;

	fragmentName = "fragment-shader[none].txt";
	float height = 5.0f*METER;
	GetVertices().push_back({ { -height / 3.0f, height, 0.0f }, { 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 3.0f, height, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { -height / 3.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });
	GetVertices().push_back({ { height / 3.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } });

	GetIndices().push_back({ glm::uvec3(2, 0, 1)});
	GetIndices().push_back({ glm::uvec3(2, 1, 3) });


	if (species == 1){
		textureName = "alien.png";
	}
	else if (species == 2){
		textureName = "alien2.png";
	}
	else if (species == 3){
		textureName = "alien3.png";
	}
	else if (species == 4){
		textureName = "alien4.png";
	}
	else if (species == 5){
		textureName = "alien5.png";
	}
	else if (species == 6){
		textureName = "alien6.png";
	}
	else if (species == 7){
		textureName = "alien7.png";
	}

	shape = new btBoxShape(btVector3(0.1f*METER, 0.5f*METER, 0.1f*METER));

	Load(); //loads drawing related stuff. Call after vertices/indices have been defined
}
void Alien::Update(double dt){
	
	timer += dt;
	if (timer >= TIMEMAX){
		timer = TIMEMAX;
		if (glm::distance(camera->position(), translate) < 20.0f*METER){
				char* filename = "";
				if (species == 1){
					filename = "lisa.wav";
				}
				else if (species == 2){
					filename = "ohmy.wav";
				}
				else if (species == 3){
					filename = "nyehehe.mp3";
				}
				else if (species == 4){
					filename = "kawaii.mp3";
				}
				else if (species == 5){
					filename = "ohmy.wav";
				}
				else if (species == 6){
					filename = "ohmy.wav";
				}
				else if (species == 7){
					filename = "ohmy.wav";
				}
				irrklang::ISound* s = sound->play3D(filename, irrklang::vec3df(translate.x, translate.y, translate.z), false, false, true);

				if (s){
					s->setVolume(1.0f);
					s->setMinDistance(1.0f*KILOMETER);
					s->setPosition(irrklang::vec3df(0, 0, 0));
				}
				timer = 0;
		}
	}
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
	translate = alienMovement(translate, camera->position());
	position = glm::translate(glm::mat4(),glm::vec3(translate.x,translate.y,translate.z));

	glm::mat4 rotat = billboard(glm::vec3(translate.x, 0.0f, translate.z), glm::vec3(camera->position().x, 0.0f, camera->position().z), glm::vec3(0, 1, 0));

	//glm::vec2 ang1 = glm::normalize(glm::vec2(rotate.x, rotate.z));
	//glm::vec2 ang2 = glm::normalize(glm::vec2(camera->position().x - translate.x, camera->position().z-translate.z));
	//float angle = glm::atan(ang2.y - ang1.y, ang2.x - ang1.x)*glm::atan(ang2.y - ang1.y, ang2.x - ang1.x);
	position = position*rotat;


}
Alien::~Alien()
{
}


glm::vec3 Alien::alienMovement(glm::vec3 alien, glm::vec3 player) {

	float radius = 0.4*KILOMETER;
	glm::vec3 center(0, 0, 0);
	//std::normal_distribution<float> velDistro(1, 2);
	float velocity = (1 / glm::length(alien - player)) * METER*100;
	float angle = 0.0f;


	glm::vec2 alien2D(alien.x, alien.z);
	if (abs(radius - length(alien2D)) < (radius/8)) {
		float currentLoc = glm::degrees(atan2(alien.z, alien.x));
		if (currentLoc < 0) {
			currentLoc = 360 + currentLoc;
		}
		std::normal_distribution<float> distro1(currentLoc-15.0f, currentLoc+15.0f);
		angle = GetDistribution(distro1);
		if (angle < 0) {
			angle = 360 + angle;
		}
		else if (angle > 360) {
			angle = angle - 360;
		}
	}

	else if (glm::length(alien - player) > (radius/4)) {
		std::normal_distribution<float> distro2(prevAngle-30.0f, prevAngle+30.0f);
		angle = GetDistribution(distro2);
		if (angle < 0) {
			angle = 360 + angle;
		}
		else if (angle > 360) {
			angle = angle - 360;
		}
	}
	else {
		float playerLoc = glm::degrees(atan2((alien.z-player.z), (alien.x-player.x)));
		playerLoc = playerLoc*-1;
		if (playerLoc < 0) {
			playerLoc = 360 + playerLoc;
		}
		std::normal_distribution<float> distro3(playerLoc-15.0f, playerLoc + 15.0f);
		angle = GetDistribution(distro3);
		if (angle < 0) {
			angle = 360 + angle;
		}
		else if (angle > 360) {
			angle = angle - 360;
		}
	}

	if ((rand() % 100) < 98) {
		angle = prevAngle;
		velocity = prevVeloc;
	}


	prevVeloc = velocity;
	prevAngle = (float)angle;
	alien.x = alien.x + (velocity * cos(glm::radians(angle)));
	alien.z = alien.z + (velocity * sin(glm::radians(angle))); 
	alien.y = terrain->GetHeight(alien.x, alien.z);
	
	return alien;

}

